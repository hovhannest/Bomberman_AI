#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Bomberman {
	class Brick;
	struct Coordinate;
	class Enemy;
	class Item;
	class Player;

	class TileMapBuilder {
	public:
		virtual ~TileMapBuilder() { }

		virtual int width() const = 0;
		virtual int height() const = 0;
		virtual Coordinate getDoorPosition() const = 0;
		virtual int enemyRange(std::string enemyType) const = 0;
		virtual int enemySpeed(std::string enemyType) const = 0;
		virtual std::string name() const = 0;
		virtual std::vector<Brick> bricks() const = 0;
		virtual std::vector<Enemy> enemies() const = 0;
		virtual std::vector<Item> items() const = 0;
		virtual std::shared_ptr<Player> player() const = 0;
	};
}