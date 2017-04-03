#pragma once

#include "..\..\Core\Math\Coordinate.h"

namespace Bomberman {
	class Brick {
	public:
		Brick(Coordinate position, bool destructible = false) {}

		inline bool destructible() const {
			return _destructible;
		}

		inline Coordinate position() const {
			return _position;
		}

	private:
		bool _destructible;
		Coordinate _position;
	};
}