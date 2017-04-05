#pragma once

#include "Matrix.h"

#include <stack>
#include "Coordinate.h"

namespace Bomberman {

	class Transformer {
	public:
		Transformer();

		Coordinate transform(int i, int j);
		Coordinate transform(Coordinate coordinate);

		void translate(int n);
		void translate(int i, int j);
		void translate(Coordinate vector);

		void saveState();
		void loadState();
	private:
		Matrix<int> matrix;

		std::stack<Matrix<int>> stack;
	};
}