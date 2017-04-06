#pragma once


#include <memory>
#include <string>
#include <vector>
#include <SDL.h>

#include "..\..\Core\Drawable.h"
#include "..\..\Core\EventListener.h"
#include "..\..\Core\Updatable.h"
#include "..\..\Core\SignalHandler.h"
#include "..\..\Core\Texture.h"
#include "..\..\Core\Timer.h"

namespace Bomberman {
	class TileMap;
	class SignalSender;

	class GameLayer : public Drawable, public EventListener, public Updatable, public SignalHandler {
	public:
		GameLayer();

		~GameLayer();

		void listenEvent(SDL_Event event);
		void draw();
		void update();
		void handleSignal(Signal signal);

		void load(std::shared_ptr<SDL_Renderer> renderer);

		void setSignalSender(std::weak_ptr<SignalSender> signalSender);
		void setTileMap(std::shared_ptr<TileMap> tileMap);

		void screenSizeChanged(Rectangle previousSize, Rectangle newSize);

	private:
		class Camera;

		static const int PLAYER_BLINK_TIME;

		bool drawPlayer;
		Texture background;
		Texture bomb;
		Texture brick;
		Texture destructibleBrick;
		std::vector<Texture> enemies;
		Texture explosion;
		Texture player;
		Texture extraBomb;
		Texture increaseRange;
		Texture remote;
		Texture door, destroyedDoor;
		Timer blinkPlayerTimer;

		std::weak_ptr<SignalSender> signalSender;
		std::shared_ptr<TileMap> tileMap;

		std::shared_ptr<Camera> camera;

		void drawBackground();
		void drawTile(Texture texture, Coordinate position);
		bool getEnemyTexture(std::string enemyType, Texture& texture);
	};
}