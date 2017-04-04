
#include "Enemy.h"

#include <array>
#include <climits>
#include <cstdlib>

#include "Player.h"
#include "../Map/TileMap.hpp"
#include "../../Core/Math/Random.h"
#include "../../Core/Utils/VectorUtils.h"

using namespace std;

namespace Bomberman {
	bool validNewPosition(shared_ptr<TileMap> tileMap, Coordinate position) {
		return tileMap->area().contains(position) && !tileMap->tileHasBomb(position) && !tileMap->tileHasBrick(position);
	}

	Enemy::Enemy(string type, Coordinate position) : type(type), position(position), range(0), speed(INT_MAX) {
		timer.start();
	}

	void Enemy::update(std::shared_ptr<TileMap> tileMap) {
		if (timer.getTime() >= speed) {
			if (playerInsideRange(tileMap->player()) && !tileMap->player()->isDead()) {
				followPlayer(tileMap);
			}
			else {
				randomMove(tileMap);
			}

			timer.clear();
			timer.start();
		}
	}

	string Enemy::getType() const {
		return type;
	}

	Coordinate Enemy::getPosition() const {
		return position;
	}

	void Enemy::setPosition(Coordinate position) {
		this->position = position;
	}

	void Enemy::setRange(int range) {
		this->range = range;
	}

	void Enemy::setSpeed(int speed) {
		this->speed = speed;
	}

	bool Enemy::playerInsideRange(shared_ptr<Player> player) const {
		Coordinate delta = (player->position() - position).absolute();

		return delta.i <= range && delta.j <= range;
	}

	void Enemy::followPlayer(shared_ptr<TileMap> tileMap) {
		Coordinate delta = tileMap->player()->position() - position;

		bool moveH = delta.i != 0 && abs(delta.i) <= range;
		Coordinate nextH = position;
		if (moveH) {
			nextH.i += delta.i / abs(delta.i);
			moveH = validNewPosition(tileMap, nextH);
		}

		bool moveV = delta.j != 0 && abs(delta.j) <= range;
		Coordinate nextV = position;
		if (moveV) {
			nextV.j += delta.j / abs(delta.j);
			moveV = validNewPosition(tileMap, nextV);
		}

		if (moveH && moveV) {
			if (Random::randomFloat(1) <= .5) {
				position = nextH;
			}
			else {
				position = nextV;
			}
		}
		else if (moveH) {
			position = nextH;
		}
		else if (moveV) {
			position = nextV;
		}
	}

	void Enemy::randomMove(shared_ptr<TileMap> tileMap) {
		vector<Coordinate> availablePositions;

		array<Coordinate, 4> adjacents = position.cross();
		for (Coordinate adjacent : adjacents) {
			if (validNewPosition(tileMap, adjacent)) {
				availablePositions.push_back(adjacent);
			}
		}

		if (availablePositions.empty()) {
			return;
		}

		position = VectorUtils::randomElement(availablePositions);
	}
}
