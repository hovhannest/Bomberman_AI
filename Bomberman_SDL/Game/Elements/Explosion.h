#pragma once

#include <memory>
#include <vector>

#include "..\..\Core\Math\Coordinate.h"
#include "..\..\Core\Timer.h"

namespace Bomberman {
	class TileMap;

	class Explosion {
	public:
		Explosion(Coordinate position, int size, std::shared_ptr<TileMap> tileMap);
		~Explosion();

		void update();

		bool done() const;
		std::vector<Coordinate> hitArea() const;

	private:
		static const int TTL;

		std::vector<Coordinate> _hitArea;
		Coordinate position;
		Timer timer;
		bool _done;
	};
}