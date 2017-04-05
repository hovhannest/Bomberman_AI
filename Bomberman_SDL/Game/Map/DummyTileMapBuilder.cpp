
#include "DummyTileMapBuilder.h"

#include "../Constants.h"
#include "../../Core/Log/Log.h"
#include "../../Core/Utils/Exception.h"
#include "../Elements/Item.h"
#include "../Elements/Enemy.h"
#include "../Elements/Brick.h"

using namespace std;
using namespace Bomberman::Constants;

namespace Bomberman {
	int DummyTileMapBuilder::width() const {
		return _width;
	}

	int DummyTileMapBuilder::height() const {
		return _height;
	}

	int DummyTileMapBuilder::enemyRange(string enemyType) const {
		if (ENEMY_EASY == enemyType) {
			return enemiesRange.easy;
		}
		else if (ENEMY_MEDIUM == enemyType) {
			return enemiesRange.medium;
		}
		else if (ENEMY_HARD == enemyType) {
			return enemiesRange.hard;
		}

		Log::get() << "Invalid enemy type: \"" << enemyType << "\"." << InvalidArgumentException();

		return -1;
	}

	Coordinate DummyTileMapBuilder::getDoorPosition() const {
		return doorPosition;
	}

	int DummyTileMapBuilder::enemySpeed(string enemyType) const {
		if (ENEMY_EASY == enemyType) {
			return enemiesSpeed.easy;
		}
		else if (ENEMY_MEDIUM == enemyType) {
			return enemiesSpeed.medium;
		}
		else if (ENEMY_HARD == enemyType) {
			return enemiesSpeed.hard;
		}

		Log::get() << "Invalid enemy type: \"" << enemyType << "\"." << InvalidArgumentException();

		return -1;
	}

	string DummyTileMapBuilder::name() const {
		return _name;
	}

	vector<Brick> DummyTileMapBuilder::bricks() const {
		return _bricks;
	}

	vector<Enemy> DummyTileMapBuilder::enemies() const {
		return _enemies;
	}

	vector<Item> DummyTileMapBuilder::items() const {
		return _items;
	}

	shared_ptr<Player> DummyTileMapBuilder::player() const {
		return _player;
	}
}
