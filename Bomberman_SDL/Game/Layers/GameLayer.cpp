
#include "GameLayer.h"
#include "../Constants.h"
#include "../../Core/Utils/Pointer.h"
#include "../../Core/SignalSender.h"
#include "../../Core/Log/LogLevel.h"
#include "../../Core/Log/Log.h"
#include "../Elements/Item.h"
#include "../Map/TileMap.h"
#include "../Elements/Explosion.h"
#include "../Elements/Enemy.h"
#include "../Elements/Player.h"
#include "../Elements/Bomb.h"
#include "../Elements/Brick.h"
#include "../Signal.h"


using namespace std;
using namespace Bomberman::Constants;

namespace Bomberman {
	const int GameLayer::PLAYER_BLINK_TIME = 125;

	class GameLayer::Camera {
	public:
		void setScreenSize(Rectangle size) {
			// Camera size in tiles
			cameraWidth = size.width / TILE_WIDTH;
			cameraHeight = size.height / TILE_HEIGHT;
		}

		// Input position is in tile coordinates
		// Output position is in screen coordinates
		Coordinate getScreenPosition(Coordinate position) const {
			position -= static_cast<Coordinate>(tileMapOffset);

			position.i *= TILE_WIDTH;
			position.j *= TILE_HEIGHT;

			return position;
		}

		// Position is in tile coordinates
		bool insideCamera(Coordinate position) const {
			//return rectangle.contains(position + rectangle);
			return true;
		}

		void update(Coordinate newPlayerPos, Rectangle tileMapArea) {
			if (firstUpdate) {
				// Center the player in the camera, taking into account the borders
				tileMapOffset = Rectangle::ZERO;
				Rectangle cameraArea = this->cameraArea();

				tileMapOffset.i = newPlayerPos.i - cameraArea.widthCenter();
				if (tileMapOffset.i < (tileMapArea.left() - LEFT_BORDER)) {
					tileMapOffset.i = -LEFT_BORDER;
				}
				else if (tileMapOffset.i > (tileMapArea.right() - cameraArea.width + RIGHT_BORDER)) {
					tileMapOffset.i = tileMapArea.width - cameraArea.width + RIGHT_BORDER;
				}

				tileMapOffset.j = newPlayerPos.j - cameraArea.heightCenter();
				if (tileMapOffset.j < (tileMapArea.top() - TOP_BORDER)) {
					tileMapOffset.j = -TOP_BORDER;
				}
				else if (tileMapOffset.j > (tileMapArea.bottom() - cameraArea.height + BOTTOM_BORDER)) {
					tileMapOffset.j = tileMapArea.height - cameraArea.height + BOTTOM_BORDER;
				}

				previousPlayerPos = newPlayerPos;
				firstUpdate = false;
			}
			else if (previousPlayerPos != newPlayerPos) {
				Rectangle cameraArea = this->cameraArea();
				Coordinate direction = (newPlayerPos - previousPlayerPos).canonize();

				if (cameraArea.widthCenter() == previousPlayerPos.i) {
					// The player was in the horizontal center.
					// Move the offset if its moving horizontally and if
					// there is still space to move
					if (direction.i > 0 && tileMapOffset.i < (tileMapArea.width - cameraWidth + RIGHT_BORDER)) {
						++tileMapOffset.i;
					}
					else if (direction.i < 0 && tileMapOffset.i >(tileMapArea.i - LEFT_BORDER)) {
						--tileMapOffset.i;
					}
				}

				if (cameraArea.heightCenter() == previousPlayerPos.j) {
					// The player was in the vertical center
					// Move the offset if its moving vertically
					if (direction.j > 0 && tileMapOffset.j < (tileMapArea.height - cameraHeight + BOTTOM_BORDER)) {
						++tileMapOffset.j;
					}
					else if (direction.j < 0 && tileMapOffset.j >(tileMapArea.j - TOP_BORDER)) {
						--tileMapOffset.j;
					}
				}

				previousPlayerPos = newPlayerPos;
			}
		}

		void reset() {
			firstUpdate = true;

			previousPlayerPos = Coordinate::ZERO;
			tileMapOffset = Coordinate::ZERO;
		}

	private:
		const int RIGHT_BORDER = 1;
		const int LEFT_BORDER = 1;
		const int TOP_BORDER = 2;
		const int BOTTOM_BORDER = 1;

		bool firstUpdate = true;

		// The position of the player since last update.
		Coordinate previousPlayerPos;

		// Offset from tile map's [0,0] in tiles.
		Coordinate tileMapOffset;

		// Size of the camera in tiles.
		int cameraWidth, cameraHeight;

		// The rectangle of the camera
		Rectangle cameraArea() const {
			return Rectangle(tileMapOffset.i, tileMapOffset.j, cameraWidth, cameraHeight);
		}
	};

	GameLayer::GameLayer() : camera(new Camera()), drawPlayer(true) {

	}

	GameLayer::~GameLayer() {

	}

	void GameLayer::listenEvent(SDL_Event event) {
		if (SDL_KEYUP == event.type && SDLK_RETURN == event.key.keysym.sym) {
			shared_ptr<SignalSender> signalSender;
			if (lockWeakPointer(this->signalSender, signalSender)) {
				signalSender->sendSignal(Signal::MainMenu);
			}
			else {
				Log::get() << "No SignalSender for GameLayer" << LogLevel::error;
			}
		}
	}

	void GameLayer::draw() {
		Texture texture;

		background.rectangle() = camera->getScreenPosition(Coordinate::ZERO);
		background.draw();

		// Draw door
		drawTile(!tileMap->doorDestroyed() ? door : destroyedDoor, tileMap->doorPosition());

		// Draw Items
		auto items = tileMap->items();
		for (auto item : items) {
			if (Item::INCREASE_RANGE == item) {
				texture = increaseRange;
			}
			else if (Item::EXTRA_BOMB == item) {
				texture = extraBomb;
			}
			else if (Item::REMOTE == item) {
				texture = remote;
			}
			else {
				Log::get() << "No valid texture for item with id: " << item.id() << LogLevel::warning;
				continue;
			}

			drawTile(texture, item.getPosition());
		}

		// Draw bricks
		auto bricks = tileMap->bricks();
		for (auto brick : bricks) {
			texture = (brick.destructible()) ? destructibleBrick : this->brick;

			drawTile(texture, brick.position());
		}

		// Draw bombs
		auto bombs = tileMap->bombs();
		for (auto bomb : bombs) {
			drawTile(this->bomb, bomb.getPosition());
		}

		// Draw player
		auto player = tileMap->player();
		if (!player->isDead() && drawPlayer) {
			drawTile(this->player, player->position());
		}

		// Draw enemies
		auto enemies = tileMap->enemies();
		texture = Texture();
		for (auto enemy : enemies) {
			if (getEnemyTexture(enemy.getType(), texture)) {
				drawTile(texture, enemy.getPosition());
			}
		}

		// Draw explosions
		auto explosions = tileMap->explosions();
		for (auto explosion : explosions) {
			auto hitArea = explosion.hitArea();

			for (auto position : hitArea) {
				if (tileMap->area().contains(position) && !tileMap->tileHasBrick(position)) {
					drawTile(this->explosion, position);
				}
			}
		}
	}

	void GameLayer::update() {
		tileMap->update();
		camera->update(tileMap->player()->position(), tileMap->area());

		// Player blinking stuff
		if (tileMap->player()->isInvincible()) {
			if (!blinkPlayerTimer.isCounting()) {
				blinkPlayerTimer.start();
			}
			else if (blinkPlayerTimer.getTime() >= PLAYER_BLINK_TIME) {
				blinkPlayerTimer.restart();
				drawPlayer = !drawPlayer;
			}
		}
		else {
			drawPlayer = true;
		}

		if (tileMap->gameOver() || tileMap->playerWins()) {
			shared_ptr<SignalSender> signalSender;
			if (lockWeakPointer(this->signalSender, signalSender)) {
				signalSender->sendSignal(Signal::EndGame);
			}
			else {
				Log::get() << "No SignalSender for GameLayer" << LogLevel::error;
			}
		}
	}

	void GameLayer::handleSignal(Signal signal) {
		if (Signal::InGame == signal || Signal::HideConsole == signal) {
			Drawable::enable();
			EventListener::disable();
			Updatable::enable();
		}
		else if (Signal::PauseGame == signal || Signal::ShowConsole == signal) {
			Drawable::enable();
			EventListener::disable();
			Updatable::disable();
		}
		else if (Signal::EndGame == signal) {
			Drawable::enable();
			EventListener::enable();
			Updatable::disable();
		}
		else {
			Drawable::disable();
			EventListener::disable();
			Updatable::disable();
		}
	}

	void GameLayer::load(shared_ptr<SDL_Renderer> renderer) {
		background = Texture("background.png", renderer);
		bomb = Texture("bomb.png", renderer);
		brick = Texture("brick.png", renderer);
		destructibleBrick = Texture("destructibleBrick.png", renderer);
		explosion = Texture("explosion.png", renderer);
		player = Texture("bomberman.png", renderer);
		extraBomb = Texture("extraBomb.png", renderer);
		increaseRange = Texture("increaseRange.png", renderer);
		remote = Texture("remote.png", renderer);

		door = Texture("door.png", renderer);
		destroyedDoor = Texture("destroyedDoor.png", renderer);

		Texture enemy;

		enemy = Texture("enemy1.png", renderer);
		enemies.push_back(enemy);
		enemy = Texture("enemy2.png", renderer);
		enemies.push_back(enemy);
		enemy = Texture("enemy3.png", renderer);
		enemies.push_back(enemy);
	}

	void GameLayer::setSignalSender(weak_ptr<SignalSender> signalSender) {
		this->signalSender = signalSender;
	}

	void GameLayer::setTileMap(shared_ptr<TileMap> tileMap) {
		this->tileMap = tileMap;

		if (tileMap) {
			background.rectangle().width = tileMap->area().width * TILE_WIDTH;
			background.rectangle().height = tileMap->area().height * TILE_HEIGHT;

			camera->reset();
		}
	}

	void GameLayer::screenSizeChanged(Rectangle previousSize, Rectangle newSize) {
		camera->setScreenSize(newSize);
	}

	void GameLayer::drawTile(Texture texture, Coordinate position) {
		if (camera->insideCamera(position)) {
			texture.position() = camera->getScreenPosition(position);
			texture.draw();
		}
	}

	bool GameLayer::getEnemyTexture(string enemyType, Texture& texture) {
		bool result = true;

		if (ENEMY_EASY == enemyType) {
			texture = enemies[0];
		}
		else if (ENEMY_MEDIUM == enemyType) {
			texture = enemies[1];
		}
		else if (ENEMY_HARD == enemyType) {
			texture = enemies[2];
		}
		else {
			result = false;
			Log::get() << "Invalid enemy type: " << enemyType << "." << LogLevel::warning;
		}

		return result;
	}
}
