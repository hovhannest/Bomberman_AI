#pragma once

#include "Coordinate.h"

namespace Bomberman {
	class Rectangle : public Coordinate {
	public:
		Rectangle();
		Rectangle(int i, int j, int width, int height);

		int width, height;

		int widthHalf() const;
		int heightHalf() const;

		int widthCenter() const;
		int heightCenter() const;
		int left() const;
		int right() const;
		int top() const;
		int bottom() const;

		Coordinate center() const;
		Coordinate leftTop() const;
		Coordinate rightTop() const;
		Coordinate rightBottom() const;
		Coordinate leftBottom() const;

		bool intersects(Rectangle other) const;
		bool contains(int i, int j) const;
		bool contains(Coordinate coordinate) const;

		bool operator==(Rectangle other) const;
		bool operator!=(Rectangle other) const;

		Rectangle& operator=(Coordinate other);

		std::string toString() const;
	};
}
