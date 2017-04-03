
#include "Explosion.h"
#include "..\Map\TileMap.hpp"


using namespace std;

namespace Bomberman {
	const int Explosion::TTL = 1500;

	Explosion::Explosion(Coordinate position, int size, shared_ptr<TileMap> tileMap) : position(position), _done(false) {
		Coordinate c;

		// The position of the bomb
		_hitArea.push_back(position);

		// To the left
		c = position.left();
		for (int n = 0; n < size; ++n, --c.i) {
			_hitArea.push_back(c);

			if (tileMap->tileHasBomb(c) || tileMap->tileHasBrick(c)) {
				break;
			}
		}

		// To the right
		c = position.right();
		for (int n = 0; n < size; ++n, ++c.i) {
			_hitArea.push_back(c);

			if (tileMap->tileHasBomb(c) || tileMap->tileHasBrick(c)) {
				break;
			}
		}

		// Up
		c = position.up();
		for (int n = 0; n < size; ++n, --c.j) {
			_hitArea.push_back(c);

			if (tileMap->tileHasBomb(c) || tileMap->tileHasBrick(c)) {
				break;
			}
		}

		// Down
		c = position.down();
		for (int n = 0; n < size; ++n, ++c.j) {
			_hitArea.push_back(c);

			if (tileMap->tileHasBomb(c) || tileMap->tileHasBrick(c)) {
				break;
			}
		}
	}

	Explosion::~Explosion() {

	}

	void Explosion::update() {
		if (!timer.isCounting()) {
			timer.start();
		}
		else if (timer.getTime() >= TTL) {
			_done = true;
		}
	}

	bool Explosion::done() const {
		return _done;
	}

	vector<Coordinate> Explosion::hitArea() const {
		return _hitArea;
	}
}
