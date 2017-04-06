#pragma once

#include <memory>
#include <SDL.h>

#include "../../Core/Drawable.h"
#include "../../Core/SignalHandler.h"
#include "../../Core/SignalSender.h"
#include "../../Core/Texture.h"


namespace Bomberman {
	class Player;
	class TileMap;

	class HudLayer : public Drawable, public SignalHandler {
	public:
		void draw();
		void handleSignal(Signal signal);

		void load(std::shared_ptr<SDL_Renderer> renderer);

		void screenSizeChanged(Rectangle previousSize, Rectangle newSize);

		void setPlayer(std::weak_ptr<Player> player);
		void setTileMap(std::weak_ptr<TileMap> tileMap);

	private:
		Texture hud;
		Texture life;
		Texture extraBomb, remote, explosion;
		Texture background;
		Texture gameOver, continueText;
		Texture digits[10];
		Texture enemiesLeft;
		std::weak_ptr<TileMap> tileMap;
		std::weak_ptr<Player> player;

		void drawNumber(int numberToDraw, Rectangle area);
	};
}
