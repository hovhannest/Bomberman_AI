#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "../../Core/Math/Rectangle.h"
#include "../../Core/Timer.h"


namespace Bomberman {
	class Explosion;
	class Enemy;
	class Bomb;
	class Brick;
	class Item;
	class Player;
	class TileMapBuilder;

	class TileMap : public std::enable_shared_from_this<TileMap> {
	public:
		TileMap(std::shared_ptr<TileMapBuilder> builder);
		~TileMap();

		Rectangle area() const;
		int width() const;
		int height() const;
		std::string name() const;
		std::list<Bomb> bombs() const;
		std::vector<Brick> bricks() const;
		std::list<Enemy> enemies() const;
		std::list<Explosion> explosions() const;
		std::list<Item> items() const;
		std::shared_ptr<Player> player() const;

		void update();

		void addBomb(Bomb bomb);
		int bombCount() const;
		void explodeBomb();
		void endGame();

		bool tileHasBrick(Coordinate tile) const;
		bool tileHasBomb(Coordinate tile) const;
		bool gameOver() const;
		bool playerWins() const;
		bool doorDestroyed() const;
		Coordinate doorPosition() const;
		int enemiesLeft() const;

	private:
		static const int playerRespawnTime;

		Rectangle _area;
		bool _doorDestroyed;
		Coordinate _doorPosition;
		std::string _name;
		std::shared_ptr<Player> _player;
		std::vector<Brick> _bricks;
		std::list<Bomb> _bombs;
		std::list<Enemy> _enemies;
		std::list<Item> _items;
		std::list<Explosion> _explosions;
		Timer playerRespawn;
		bool _explodeBomb;
		bool _gameOver;
		bool _playerWins;

		void updateEnemies();
		void updateBombs();
		void doDamage(std::vector<Coordinate> area);
		void updateExplosions();
		void clearExplosions();
		void enemiesAttack();
	};
}
