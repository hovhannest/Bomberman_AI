#pragma once
#include "..\..\Core\Math\Coordinate.h"
#include "..\..\Core\Timer.h"


namespace Bomberman {
	class Bomb {
	public:
		Bomb(Coordinate position);

		Coordinate getPosition() const;

		void update(bool remote, bool forceExplosion);
		void forceExplosion();

		bool exploded() const;

	private:
		static const int TTL;

		Timer timer;
		Coordinate _position;
		bool _exploded;
	};
}
