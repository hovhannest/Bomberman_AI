
#include "Rectangle.h"


#include <sstream>

using namespace std;

namespace Bomberman {
	Rectangle::Rectangle() : Coordinate(), width(0), height(0) {

	}

	Rectangle::Rectangle(int i, int j, int width, int height) : Coordinate(i, j), width(width), height(height) {

	}

	int Rectangle::widthHalf() const {
		return width / 2;
	}

	int Rectangle::heightHalf() const {
		return height / 2;
	}

	int Rectangle::widthCenter() const {
		return i + widthHalf();
	}

	int Rectangle::heightCenter() const {
		return j + heightHalf();
	}

	int Rectangle::left() const {
		return i;
	}

	int Rectangle::right() const {
		return i + width - 1;
	}

	int Rectangle::top() const {
		return j;
	}

	int Rectangle::bottom() const {
		return j + height - 1;
	}

	Coordinate Rectangle::center() const {
		return Coordinate(widthCenter(), heightCenter());
	}

	Coordinate Rectangle::leftTop() const {
		return Coordinate(left(), top());
	}

	Coordinate Rectangle::rightTop() const {
		return Coordinate(right(), top());
	}

	Coordinate Rectangle::rightBottom() const {
		return Coordinate(right(), bottom());
	}

	Coordinate Rectangle::leftBottom() const {
		return Coordinate(left(), bottom());
	}

	bool Rectangle::intersects(Rectangle other) const {
		return
			contains(other.leftTop()) ||
			contains(other.rightTop()) ||
			contains(other.leftBottom()) ||
			contains(other.rightBottom()) ||
			other.contains(leftTop()) ||
			other.contains(rightTop()) ||
			other.contains(rightBottom()) ||
			other.contains(leftBottom());
	}

	bool Rectangle::contains(int i, int j) const {
		return
			(i >= this->i) &&
			(i < (this->i + width)) &&
			(j >= this->j) &&
			(j < (this->j + height));
	}

	bool Rectangle::contains(Coordinate coordinate) const {
		return contains(coordinate.i, coordinate.j);
	}

	bool Rectangle::operator==(Rectangle other) const {
		return
			static_cast<Coordinate>(*this) == static_cast<Coordinate>(other) &&
			width == other.width &&
			height == other.height;
	}

	bool Rectangle::operator!=(Rectangle other) const {
		return !(*this == other);
	}

	Rectangle& Rectangle::operator=(Coordinate other) {
		i = other.i;
		j = other.j;

		return *this;
	}

	string Rectangle::toString() const {
		stringstream s;

		s << static_cast<Coordinate>(*this).toString();
		s << " width: " << width;
		s << " height: " << height;

		return s.str();
	}
}
