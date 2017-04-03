
#include "Coordinate.h"

#include <cmath>
#include <sstream>

using namespace std;

namespace Bomberman {
	Coordinate::Coordinate() : i(0), j(0) {

	}

	Coordinate::Coordinate(int val) : i(val), j(val) {

	}

	Coordinate::Coordinate(int i, int j) : i(i), j(j) {

	}

	Coordinate Coordinate::absolute() const {
		return Coordinate(abs(i), abs(j));
	}

	Coordinate Coordinate::left() const {
		return Coordinate(i - 1, j);
	}

	Coordinate Coordinate::right() const {
		return Coordinate(i + 1, j);
	}

	Coordinate Coordinate::up() const {
		return Coordinate(i, j - 1);
	}

	Coordinate Coordinate::down() const {
		return Coordinate(i, j + 1);
	}

	Coordinate Coordinate::leftUp() const {
		return Coordinate(i - 1, j - 1);
	}

	Coordinate Coordinate::rightUp() const {
		return Coordinate(i + 1, j - 1);
	}

	Coordinate Coordinate::leftDown() const {
		return Coordinate(i - 1, j + 1);
	}

	Coordinate Coordinate::rightDown() const {
		return Coordinate(i + 1, j + 1);
	}

	Coordinate Coordinate::canonize() const {
		Coordinate canonicalVector;

		if (i > 0) {
			canonicalVector.i = 1;
		}
		else if (i < 0) {
			canonicalVector.i = -1;
		}

		if (j > 0) {
			canonicalVector.j = 1;
		}
		else if (j < 0) {
			canonicalVector.j = -1;
		}

		return canonicalVector;
	}

	array<Coordinate, 4> Coordinate::cross() const {
		return{
			left(), right(), up(), down()
		};
	}

	array<Coordinate, 8> Coordinate::adjacents() const {
		return{
			left(), right(), up(), down(),
			leftUp(), rightUp(), leftDown(), rightDown()
		};
	}

	bool Coordinate::operator==(Coordinate other) const {
		return i == other.i && j == other.j;
	}

	bool Coordinate::operator!=(Coordinate other) const {
		return !(*this == other);
	}

	Coordinate& Coordinate::operator+=(Coordinate other) {
		i += other.i;
		j += other.j;

		return *this;
	}

	Coordinate& Coordinate::operator-=(Coordinate other) {
		i -= other.i;
		j -= other.j;

		return *this;
	}

	Coordinate& Coordinate::operator*=(Coordinate other) {
		i *= other.i;
		j *= other.j;

		return *this;
	}

	Coordinate& Coordinate::operator/=(Coordinate other) {
		i /= other.i;
		j /= other.j;

		return *this;
	}

	string Coordinate::toString() const {
		stringstream s;

		s << "[" << i << ", " << j << "]";

		return s.str();
	}

	Coordinate operator-(Coordinate coordinate) {
		coordinate.i = -coordinate.i;
		coordinate.j = -coordinate.j;

		return coordinate;
	}

	Coordinate operator+(Coordinate left, Coordinate right) {
		left += right;

		return left;
	}

	Coordinate operator-(Coordinate left, Coordinate right) {
		left -= right;

		return left;
	}

	Coordinate operator*(Coordinate left, Coordinate right) {
		left *= right;

		return left;
	}

	Coordinate operator/(Coordinate left, Coordinate right) {
		left /= right;

		return left;
	}

	const Coordinate Coordinate::ZERO = Coordinate(0, 0);
	const Coordinate Coordinate::ONE = Coordinate(1, 1);

	const Coordinate Coordinate::RIGHT = Coordinate(1, 0);
	const Coordinate Coordinate::LEFT = Coordinate(-1, 0);

	const Coordinate Coordinate::UP = Coordinate(0, 1);
	const Coordinate Coordinate::DOWN = Coordinate(0, -1);
}
