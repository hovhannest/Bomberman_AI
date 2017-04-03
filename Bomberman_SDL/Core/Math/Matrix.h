#pragma once


#include <algorithm>
#include <stdexcept>

namespace Bomberman {
	template <class T>
	class Matrix {
	public:
		Matrix() : Matrix(0) {

		}

		Matrix(int size) : Matrix(size, size) {

		}

		Matrix(int rows, int columns) : _columns(columns), _rows(rows), references(nullptr), values(nullptr) {
			if (columns < 0 || rows < 0) {
				throw std::out_of_range("matrix");
			}

			references = new int;
			*references = 1;

			if (columns == 0 || rows == 0) {
				return;
			}

			values = new T*[_columns];

			for (int j = 0; j < _columns; ++j) {
				values[j] = new T[_rows];
			}
		}

		Matrix(const Matrix& other) : _columns(other._columns), _rows(other._rows), references(other.references), values(other.values) {
			++(*references);
		}

		Matrix(Matrix&& other) : _columns(0), _rows(0), references(nullptr), values(nullptr) {
			Matrix::swap(*this, other);
		}

		~Matrix() {
			if (references == nullptr) return;

			if (--(*references) == 0) {
				for (int j = 0; j < _columns; ++j) {
					delete[] values[j];
				}

				delete[] values;
			}
		}

		Matrix& operator=(Matrix other) {
			swap(*this, other);

			return *this;
		}

		int columns() const {
			return _columns;
		}

		int rows() const {
			return _rows;
		}

		T& pos(int row, int column) {
			if (!validPos(row, column)) {
				throw std::out_of_range("matrix");
			}

			return values[column][row];
		}

		void reset(T value) {
			for (int j = 0; j < _columns; ++j) {
				for (int i = 0; i < _rows; ++i) {
					values[j][i] = value;
				}
			}
		}

		bool validPos(int row, int column) const {
			return (0 <= column) && (column < _columns) && (0 <= row) && (row < _rows) && (_columns != 0) && (_rows != 0);
		}

		static void identity(int size);
		static Matrix<T> multiply(Matrix<T> a, Matrix<T> b);

	private:
		int *references;
		T **values;
		int _columns, _rows;

		void swap(Matrix& first, Matrix& second) {
			using std::swap;

			swap(first.references, second.references);
			swap(first.values, second.values);
			swap(first._columns, second._columns);
			swap(first._rows, second._rows);
		}
	};

	template <class T>
	Matrix<T> Matrix<T>::multiply(Matrix<T> a, Matrix<T> b) {
		if (a.columns() != b.rows()) {
			throw std::invalid_argument("matrix");
		}

		Matrix<T> c(a.rows(), b.columns());

		for (int i = 0; i < c.rows(); ++i) {
			for (int j = 0; j < c.columns(); ++j) {
				c.pos(i, j) = {};

				for (int k = 0; k < a.columns(); ++k) {
					c.pos(i, j) += a.pos(i, k) * b.pos(k, j);
				}
			}
		}

		return c;
	}
}
