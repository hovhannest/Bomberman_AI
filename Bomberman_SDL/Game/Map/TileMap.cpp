
#include "TileMap.hpp"

#include <stack>

#include "TileMapBuilder.h"
#include "../Constants.h"
#include "../../tinyxml2/tinyxml2.h"
#include "../../Core/Log/Log.h"
#include "../../Core/Utils/Exception.h"
#include "../../Core/Math/Coordinate.h"
#include "../../Core/Utils/VectorUtils.h"
#include "../Elements/Player.h"
#include "../Elements/Brick.h"
#include "../Elements/Bomb.h"
#include "../Elements/Enemy.h"
#include "../Elements/Item.h"
#include "../Elements/Explosion.h"

using namespace std;
using namespace tinyxml2;
using namespace Bomberman::Constants;

namespace Bomberman {
	const int TileMap::playerRespawnTime = 2000;

	TileMap::TileMap(shared_ptr<TileMapBuilder> builder) : _explodeBomb(false), _gameOver(false), _doorDestroyed(false), _playerWins(false) {
		if (!builder) {
			Log::get() << "Invalid information to create map." << NullArgumentException();
		}

		if (builder->width() > 0) {
			_area.width = builder->width();
		}
		else {
			Log::get() << "Invalid width for map." << LogLevel::error;
		}

		if (builder->height() > 0) {
			_area.height = builder->height();
		}
		else {
			Log::get() << "Invalid height for map." << LogLevel::error;
		}

		if (!builder->name().empty()) {
			_name = builder->name();
		}
		else {
			Log::get() << "Empty name for map." << LogLevel::warning;
		}

		if (builder->player()) {
			_player = builder->player();
		}
		else {
			Log::get() << "No player information for map." << LogLevel::error;
			_player.reset(new Player());
		}

		auto bricks = builder->bricks();
		for (auto brick = bricks.begin(); brick != bricks.end(); ++brick) {
			bool found = false;

			if (!_area.contains(brick->position()))
			{
				Log::get() << "Brick out of map at position: " << brick->position().toString() << "." << LogLevel::warning;
				continue;
			}

			for (int n = 0; n < int(_bricks.size()); ++n) {
				if (brick->position() == _bricks[n].position()) {
					Log::get() << "Duplicate bricks with the same position: " << brick->position().toString() << "." << LogLevel::warning;

					_bricks[n] = *brick;
					found = true;
				}
			}

			if (!found) {
				_bricks.push_back(*brick);
			}
		}

		vector<Enemy> enemies = builder->enemies();
		for (auto enemy : enemies) {
			if (!_area.contains(enemy.getPosition())) {
				Log::get() << "Enemy out of map at position: " << enemy.getPosition().toString() << "." << LogLevel::warning;
				continue;
			}

			if (tileHasBrick(enemy.getPosition())) {
				Log::get() << "Enemy overlapping a brick at position: " << enemy.getPosition().toString() << "." << LogLevel::warning;
				continue;
			}

			enemy.setSpeed(builder->enemySpeed(enemy.getType()));
			enemy.setRange(builder->enemyRange(enemy.getType()));

			_enemies.push_back(enemy);
		}

		auto items = builder->items();
		for (auto item : items) {
			if (!_area.contains(item.getPosition())) {
				Log::get() << "Invalid position " << item.getPosition().toString() << " for item with id: " << item.id() << LogLevel::warning;
				continue;
			}

			bool invalidBrick = false;
			for (auto brick : _bricks) {
				if (!brick.destructible() && brick.position() == item.getPosition()) {
					Log::get() << "Item with id: " << item.id() << " under indestructible brick at position: " << item.getPosition().toString() << LogLevel::warning;
					invalidBrick = true;
					break;
				}
			}

			if (!invalidBrick) {
				_items.push_back(item);
			}
		};

		_doorPosition = builder->getDoorPosition();
	}

	TileMap::~TileMap() {

	}

	Rectangle TileMap::area() const {
		return _area;
	}

	int TileMap::width() const {
		return _area.width;
	}

	int TileMap::height() const {
		return _area.height;
	}

	string TileMap::name() const {
		return _name;
	}

	list<Bomb> TileMap::bombs() const {
		return _bombs;
	}

	vector<Brick> TileMap::bricks() const {
		return _bricks;
	}

	list<Enemy> TileMap::enemies() const {
		return _enemies;
	}

	list<Explosion> TileMap::explosions() const {
		return _explosions;
	}

	list<Item> TileMap::items() const {
		return _items;
	}

	shared_ptr<Player> TileMap::player() const {
		return _player;
	}

	void TileMap::update() {
		_player->update();
		updateEnemies();
		updateBombs();

		_items.remove_if([this](Item item) {
			if (item.getPosition() == _player->position()) {
				_player->addItem(item);
				return true;
			}

			return false;
		});

		stack<Coordinate> blownBombs;
		do {
			_bombs.remove_if([&blownBombs](Bomb bomb) {
				if (bomb.exploded()) {
					blownBombs.push(bomb.getPosition());
				}

				return bomb.exploded();
			});

			while (!blownBombs.empty()) {
				auto tileMap = shared_from_this();
				auto position = blownBombs.top();
				blownBombs.pop();

				Explosion explosion(position, _player->getExplosionSize(), shared_from_this());
				_explosions.push_back(explosion);
			}

			for (Explosion& explosion : _explosions) {
				doDamage(explosion.hitArea());
			}
		} while (!blownBombs.empty());

		updateExplosions();
		clearExplosions();
		enemiesAttack();

		if (_player->isDead()) {
			if (_player->getLifesCount() <= 0) {
				_gameOver = true;

				Log::get() << "Game over" << LogLevel::info;
			}
			else if (playerRespawn.getTime() >= playerRespawnTime) {
				_player->respawn();
				playerRespawn.stop();
			}
		}
		else if (_player->position() == _doorPosition && _enemies.empty()) {
			_playerWins = true;

			Log::get() << "Player wins" << LogLevel::info;
		}
	}

	void TileMap::addBomb(Bomb bomb) {
		_bombs.push_back(bomb);
	}

	void TileMap::endGame() {
		while (_player->getLifesCount() > 0) {
			_player->die();
		}

		_gameOver = true;
	}

	bool TileMap::tileHasBrick(Coordinate tile) const {
		for (auto brick = _bricks.begin(); brick != _bricks.end(); ++brick) {
			if (brick->position() == tile) {
				return true;
			}
		}

		return false;
	}

	bool TileMap::tileHasBomb(Coordinate tile) const {
		for (auto it = _bombs.begin(); it != _bombs.end(); ++it) {
			if (it->getPosition() == tile) {
				return true;
			}
		}

		return false;
	}

	bool TileMap::gameOver() const {
		return _gameOver;
	}

	bool TileMap::playerWins() const {
		return _playerWins;
	}

	bool TileMap::doorDestroyed() const {
		return _doorDestroyed;
	}

	Coordinate TileMap::doorPosition() const {
		return _doorPosition;
	}

	int TileMap::enemiesLeft() const {
		return (int)_enemies.size();
	}

	void TileMap::updateEnemies() {
		shared_ptr<TileMap> ptr = shared_from_this();

		for (auto& enemy : _enemies) {
			enemy.update(ptr);
		}
	}

	void TileMap::updateBombs() {
		for (Bomb& bomb : _bombs) {
			bomb.update(_player->hasRemote(), _explodeBomb);
			_explodeBomb = false;
		}
	}

	int TileMap::bombCount() const {
		return (int)_bombs.size();
	}

	void TileMap::explodeBomb() {
		if (!_bombs.empty()) {
			_explodeBomb = true;
		}
	}

	void TileMap::doDamage(vector<Coordinate> hitArea) {
		for (Coordinate position : hitArea) {
			for (Bomb& bomb : _bombs) {
				if (position == bomb.getPosition()) {
					bomb.forceExplosion();
					break;
				}
			}

			_enemies.remove_if([position](Enemy enemy) {
				return position == enemy.getPosition();
			});

			VectorUtils::removeIf(_bricks, [position](Brick& brick) {
				return brick.destructible() && position == brick.position();
			});

			if (_player->position() == position && !_player->isDead() && !_player->isInvincible()) {
				_player->die();
				playerRespawn.start();
			}
		}
	}

	void TileMap::updateExplosions() {
		for (Explosion& explosion : _explosions) {
			explosion.update();
		}
	}

	void TileMap::clearExplosions() {
		_explosions.remove_if([](Explosion explosion) {
			return explosion.done();
		});
	}

	void TileMap::enemiesAttack() {
		if (_player->isDead() || _player->isInvincible()) {
			return;
		}

		for (auto enemy : _enemies) {
			if (_player->position() == enemy.getPosition()) {
				_player->die();
				playerRespawn.start();
			}
		}
	}
}
