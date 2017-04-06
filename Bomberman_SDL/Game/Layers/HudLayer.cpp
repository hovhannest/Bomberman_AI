
#include "HudLayer.h"
#include "../Constants.h"
#include "../Elements/Player.h"
#include "../Map/TileMap.h"
#include "../Signal.h"
#include "../../Core/Font.h"
#include "../../Core/Color.h"
#include "../../Core/Math/Rectangle.h"
#include "../../Core/Utils/Pointer.h"
#include "../../Core/Log/Log.h"
#include "../../Core/Log/LogLevel.h"


using namespace Bomberman::Constants;
using namespace std;

namespace Bomberman {
	template <typename T>
	bool _lock(weak_ptr<T> in, shared_ptr<T>& out, string component) {
		bool result = lockWeakPointer(in, out);

		if (!result) {
			Log::get() << "No " << component << " for HudLayer" << LogLevel::error;
		}

		return result;
	}

	void HudLayer::draw() {
		shared_ptr<Player> player;
		int playerLifes = -1;
		int playerMaxLifes = -1;
		bool playerHasRemote = false;
		int playerMaxBombs = -1;
		int explosionSize = -1;
		if (_lock(this->player, player, "Player")) {
			playerLifes = player->getLifesCount();
			playerMaxLifes = player->getMaxLifes();
			playerHasRemote = player->hasRemote();
			playerMaxBombs = player->maxBombs();
			explosionSize = player->getExplosionSize();
		}

		shared_ptr<TileMap> tileMap;
		int mapBombs = -1;
		int mapEnemies = -1;
		if (_lock(this->tileMap, tileMap, "TileMap")) {
			auto tileMap = this->tileMap.lock();

			mapBombs = tileMap->bombCount();
			mapEnemies = tileMap->enemiesLeft();
		}

		int bombsAvailable = playerMaxBombs - mapBombs;
		if (bombsAvailable < 0) {
			bombsAvailable = -1;
		}

		hud.draw();

		life.setAlpha(Texture::OPAQUE);
		for (int n = 0; n < playerLifes; ++n) {
			life.rectangle().i = n * life.rectangle().height;
			life.draw();
		}

		life.setAlpha(static_cast<uint8_t>(Texture::OPAQUE * .25));
		for (int n = playerLifes; n < playerMaxLifes; ++n) {
			life.rectangle().i = n * life.rectangle().height;
			life.draw();
		}

		if (playerHasRemote) {
			remote.setAlpha(static_cast<uint8_t>(Texture::OPAQUE));
		}
		else {
			remote.setAlpha(static_cast<uint8_t>(Texture::OPAQUE * .25));
		}
		remote.rectangle().i = life.rectangle().right() + (life.rectangle().width);
		remote.draw();

		extraBomb.rectangle().i = remote.rectangle().right() + (remote.rectangle().width * 2);
		extraBomb.draw();
		Rectangle area = extraBomb.rectangle();
		area.i -= remote.rectangle().width;
		drawNumber(bombsAvailable, area);

		explosion.rectangle().i = extraBomb.rectangle().right() + (extraBomb.rectangle().width * 2);
		explosion.draw();
		area = explosion.rectangle();
		area.i -= extraBomb.rectangle().width;
		drawNumber(explosionSize, area);

		area.i = hud.rectangle().right() - TILE_WIDTH;
		area.j = hud.rectangle().i;
		area.width = TILE_WIDTH;
		area.height = TILE_HEIGHT;
		drawNumber(mapEnemies, area);
		enemiesLeft.rectangle().i = area.i - enemiesLeft.rectangle().width;
		enemiesLeft.rectangle().j = area.heightCenter() - (enemiesLeft.rectangle().height / 2);
		enemiesLeft.draw();

		if (tileMap->gameOver() || tileMap->playerWins()) {
			background.draw();
			gameOver.draw();
			continueText.draw();
		}
	}

	void HudLayer::handleSignal(Signal signal) {
		if (Signal::InGame == signal || Signal::HideConsole == signal || Signal::PauseGame == signal || Signal::EndGame == signal) {
			Drawable::enable();
		}
		else {
			Drawable::disable();
		}
	}

	void HudLayer::drawNumber(int numberToDraw, Rectangle area) {
		string stringToDraw = to_string(numberToDraw);
		int digitWidth = digits->rectangle().width;
		int totalSize = (int)stringToDraw.length() * digitWidth;
		int start = area.widthCenter() - (totalSize / 2);
		int j = area.heightCenter() - (digits->rectangle().height / 2);

		for (int n = 0; n < stringToDraw.length(); ++n) {
			char digit = stringToDraw[n];
			Texture *digitTexture = &digits[digit - '0'];
			digitTexture->rectangle().i = start + (n * digitWidth);
			digitTexture->rectangle().j = j;
			digitTexture->draw();
		}
	}

	void HudLayer::setPlayer(weak_ptr<Player> player) {
		this->player = player;
	}

	void HudLayer::setTileMap(weak_ptr<TileMap> tileMap) {
		this->tileMap = tileMap;
	}

	void HudLayer::load(shared_ptr<SDL_Renderer> renderer) {
		hud = Texture("hud.png", renderer);

		// HUD height is one tile
		hud.rectangle().height = TILE_HEIGHT;

		life = Texture("life.png", renderer);
		life.rectangle().height = TILE_HEIGHT;

		extraBomb = Texture("extraBomb.png", renderer);
		extraBomb.rectangle().height = TILE_HEIGHT;

		remote = Texture("remote.png", renderer);
		remote.rectangle().height = TILE_HEIGHT;

		explosion = Texture("explosion.png", renderer);
		explosion.rectangle().height = TILE_HEIGHT;

		Font font("PressStart2P.ttf", 75, renderer);
		font.setUnderLine();
		gameOver = font.write("GAME OVER", Color::RED);

		font = Font("PressStart2P.ttf", 25, renderer);
		continueText = font.write("Press enter to return...", Color::RED);

		background = Texture::createRectangle(1, 1, Color::BLACK, renderer);
		background.setAlpha(static_cast<uint8_t>(Texture::OPAQUE * .50));

		font = Font("PressStart2P.ttf", 15, renderer);
		for (int n = 0; n < 10; ++n) {
			digits[n] = font.write(string(1, '0' + n));
		}

		enemiesLeft = font.write("Enemies left:");
	}

	void HudLayer::screenSizeChanged(Rectangle previousSize, Rectangle newSize) {
		hud.rectangle().width = newSize.width;

		gameOver.rectangle().i = newSize.widthCenter() - gameOver.rectangle().widthCenter();
		gameOver.rectangle().j = newSize.heightCenter() - gameOver.rectangle().heightCenter();

		continueText.rectangle().i = newSize.widthCenter() - continueText.rectangle().widthCenter();
		continueText.rectangle().j = gameOver.rectangle().bottom() + 10;

		background.rectangle() = newSize;
	}
}