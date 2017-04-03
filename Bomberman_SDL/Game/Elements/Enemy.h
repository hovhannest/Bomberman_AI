#pragma once

#include <memory>
#include <string>
#include "..\..\Core\Math\Coordinate.h"
#include "..\..\Core\Timer.h"

namespace Bomberman {
	class Player;
	class TileMap;

	class Enemy {
	public:
		Enemy(std::string type, Coordinate position = Coordinate());

		void update(std::shared_ptr<TileMap> tileMap);

		std::string getType() const;
		Coordinate getPosition() const;

		void setPosition(Coordinate position);
		void setRange(int range);
		void setSpeed(int speed);

	private:
		Timer timer;
		Coordinate position;
		int range;
		int speed;
		std::string type;

		bool playerInsideRange(std::shared_ptr<Player> player) const;
		void followPlayer(std::shared_ptr<TileMap> tileMap);
		void randomMove(std::shared_ptr<TileMap> tileMap);
	};
}
