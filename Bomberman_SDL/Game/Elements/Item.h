#pragma once

#include "..\..\Core\Math\Coordinate.h"


namespace Bomberman {
	class Item {
	public:
		static const Item NONE;
		static const Item EXTRA_BOMB;
		static const Item INCREASE_RANGE;
		static const Item REMOTE;

		Item();
		Item(const Item& other);

		bool operator==(const Item& other) const;
		bool operator!=(const Item& other) const;

		int id() const;
		Coordinate getPosition() const;

		static Item create(const Item& item, Coordinate position);

	private:
		Item(int value, Coordinate position = Coordinate::ZERO);
		Coordinate position;
		int _value;
	};
}