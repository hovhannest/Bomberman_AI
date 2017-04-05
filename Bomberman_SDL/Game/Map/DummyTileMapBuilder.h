#pragma once

#include "TileMapBuilder.h"
#include "..\..\Core\Math\Coordinate.h"
#include "..\Elements\Brick.h"
#include "..\Elements\Enemy.h"
#include "..\Elements\Player.h"

namespace Bomberman {
	class DummyTileMapBuilder : public TileMapBuilder {
	public:
		int _width = 0, _height = 0;
		struct {
			int easy = 3;
			int medium = 5;
			int hard = 6;
		} enemiesRange;
		struct {
			int easy = 1500;
			int medium = 1000;
			int hard = 750;
		} enemiesSpeed;
		Coordinate doorPosition;
		std::string _name;
		std::vector<Brick> _bricks;
		std::vector<Enemy> _enemies;
		std::vector<Item> _items;
		std::shared_ptr<Player> _player;

		int width() const;

		int height() const;

		int enemyRange(std::string enemyType) const;

		int enemySpeed(std::string enemyType) const;

		Coordinate getDoorPosition() const;

		std::string name() const;

		std::vector<Brick> bricks() const;

		std::vector<Enemy> enemies() const;

		std::vector<Item> items() const;

		std::shared_ptr<Player> player() const;
	};
}