/*
 * vector3d.cc
 * Copyright (C) 2011  Bart Sas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vector3d.h"

#include <cassert>
#include <cmath>
#include <iomanip>



////////////////////////////////////////////////////////////////////////////////
// The matrix class                                                           //
////////////////////////////////////////////////////////////////////////////////

Matrix::Matrix()
		: values() {
	for (int i = 0; i <= NR_DIMS; i++) {
		// Initialise the rotation matrix.
		for (int j = 0; j <= NR_DIMS; j++) {
			values[i][j] = (i == j) ? 1.0 : 0.0;
		}
	}
}

Matrix::Matrix(const Matrix &original)
		: values() {
	operator=(original);
}

Matrix::~Matrix() {
	// Empty...
}

Matrix &Matrix::operator=(const Matrix &original) {
	for (int i = 0; i <= NR_DIMS; ++i) {
		for (int j = 0; j <= NR_DIMS; ++j) {
			values[i][j] = original.values[i][j];
		}
	}

	return *this;
}

double &Matrix::operator()(const int row_ix,
						   const int col_ix) {
	assert(1 <= row_ix && row_ix <= NR_DIMS + 1);
	assert(1 <= col_ix && col_ix <= NR_DIMS + 1);

	return values[row_ix - 1][col_ix - 1];
}

double Matrix::operator()(const int row_ix,
						  const int col_ix) const {
	assert(1 <= row_ix && row_ix <= NR_DIMS + 1);
	assert(1 <= col_ix && col_ix <= NR_DIMS + 1);

	return values[row_ix - 1][col_ix - 1];
}

Matrix &Matrix::operator*=(const Matrix &rhs) {
	double tmp_values[NR_DIMS + 1][NR_DIMS + 1];

	// Copy the current matrix to a temporary array.
	for (int i = 0; i <= NR_DIMS; i++) {
		for (int j = 0; j <= NR_DIMS; j++) {
			tmp_values[i][j] = values[i][j];
		}
	}

	// Multiply both operands.
	for (int i = 0; i <= NR_DIMS; i++) {
		for (int j = 0; j <= NR_DIMS; j++) {
			values[i][j] = 0;

			for (int k = 0; k <= NR_DIMS; k++) {
				values[i][j] += tmp_values[i][k] * rhs.values[k][j];
			}
		}
	}

	return *this;
}

void Matrix::inv() {
	double temp[NR_DIMS + 1][NR_DIMS + 1];

	// Make a copy of the original values and set this matrix to the unity matrix.
	for (int i = 0; i <= NR_DIMS; ++i) {
		for (int j = 0; j <= NR_DIMS; ++j) {
			temp[i][j] = values[i][j];
			values[i][j] = i == j ? 1 : 0;
		}
	}

	// Use Gauss-Jordan elimination to transform the original matrix into reduced
	// row echelon form.
	for (int i = 0; i <= NR_DIMS; ++i) {
		int best_row = i;

		// Find the row with the highest pivot.
		for (int j = i + 1; j <= NR_DIMS; ++j) {
			if (std::abs(temp[j][i]) > std::abs(temp[best_row][i])) {
				best_row = j;
			}
		}

		const double pivot = temp[best_row][i];

		// Swap the current row and the row with the highest pivot such that the row
		// with the highest pivot becomes the current row.  Divide the row afterwards b
		// y the value of the pivot such that the first non-zero coefficient becomes 1.
		for (int j = 0; j <= NR_DIMS; ++j) {
			std::swap(temp[best_row][j], temp[i][j]);
			std::swap(values[best_row][j], values[i][j]);

			temp[i][j] /= pivot;
			values[i][j] /= pivot;
		}

		// Subtract the current row from the remaining rows such that the first
		// non-zero element of the remaining rows becomes 0.
		for (int j = i + 1; j <= NR_DIMS; ++j) {
			const double factor = temp[j][i];

			for (int k = 0; k <= NR_DIMS; ++k) {
				temp[j][k] -= factor * temp[i][k];
				values[j][k] -= factor * values[i][k];
			}
		}
	}

	// Use Gauss-Jordan elimination to transform the matrix into a unity matrix.
	for (int i = NR_DIMS; i >= 0; --i) {
		// Subtract the current row from the preceding rows in order to zero
		// all elements on the row except for the first one.
		for (int j = 0; j < i; ++j) {
			const double factor = temp[j][i];

			for (int k = 0; k <= NR_DIMS; ++k) {
				temp[j][k] -= factor * temp[i][k];
				values[j][k] -= factor * values[i][k];
			}
		}
	}
}

Matrix Matrix::inv(Matrix matrix) {
	matrix.inv();
	return matrix;
}

void Matrix::print_line(std::ostream &output_stream,
						const int row_ix,
						const int elt_width) const {
	for (int j = 0; j <= NR_DIMS; j++) {
		output_stream << ' ' << std::setw(elt_width) << values[row_ix][j];
	}
}

void Matrix::print(std::ostream &output_stream,
				   const int elt_width) const {
	output_stream << '/';
	print_line(output_stream, 0, elt_width);
	output_stream << '\\' << std::endl;

	for (int i = 1; i < NR_DIMS; i++) {
		output_stream << '|';
		print_line(output_stream, i, elt_width);
		output_stream << '|' << std::endl;
	}

	output_stream << '\\';
	print_line(output_stream, NR_DIMS, elt_width);
	output_stream << '/' << std::endl;
}

Matrix operator*(Matrix lhs,
				 const Matrix &rhs) {
	lhs *= rhs;
	return lhs;
}


std::ostream &operator<<(std::ostream &output_stream,
						 const Matrix &matrix) {
	matrix.print(output_stream);
	return output_stream;
}



////////////////////////////////////////////////////////////////////////////////
// The vector class                                                           //
////////////////////////////////////////////////////////////////////////////////

double Vector3D::* const Vector3D::elts[] =
		{
				&Vector3D::x,
				&Vector3D::y,
				&Vector3D::z,
		};

Vector3D::Vector3D()
		: infty(false), x(0), y(0), z(0) {
	// Does nothing...
}

Vector3D::Vector3D(const double x_init,
				   const double y_init,
				   const double z_init,
				   const bool infty_init)
		: infty(infty_init), x(x_init), y(y_init), z(z_init) {
	// Does nothing...
}

Vector3D::Vector3D(const Vector3D &original,
				   const bool infty_init)
		: infty(infty_init), x(original.x), y(original.y), z(original.z) {
	// Does nothing...
}

Vector3D::Vector3D(const Vector3D &original)
		: infty(original.infty), x(original.x), y(original.y), z(original.z) {
	// Does nothing...
}

Vector3D::~Vector3D() {
	// Does nothing...
}

Vector3D Vector3D::point(const double x,
						 const double y,
						 const double z) {
	return Vector3D(x, y, z, false);
}

Vector3D Vector3D::point(const Vector3D &original) {
	return Vector3D(original, false);
}

Vector3D Vector3D::vector(const double x,
						  const double y,
						  const double z) {
	return Vector3D(x, y, z, true);
}

Vector3D Vector3D::vector(const Vector3D &original) {
	return Vector3D(original, true);
}

bool Vector3D::is_point() const {
	return !infty;
}

bool Vector3D::is_vector() const {
	return infty;
}

Vector3D &Vector3D::operator=(const Vector3D &original) {
	for (int i = 0; i < NR_DIMS; i++) {
		this->*elts[i] = original.*elts[i];
	}

	infty = original.infty;
	return *this;
}

Vector3D &Vector3D::operator+=(const Vector3D &rhs) {
	for (int i = 0; i < NR_DIMS; i++) {
		this->*elts[i] += rhs.*elts[i];
	}

	infty = infty && rhs.infty; // The result is only a vector if both operands are vectors themselves.
	return *this;
}

Vector3D &Vector3D::operator-=(const Vector3D &rhs) {
	for (int i = 0; i < NR_DIMS; i++) {
		this->*elts[i] -= rhs.*elts[i];
	}

	infty = infty ==
			rhs.infty; // if we're subtracting two points or two vectors the result is a vector, otherwise it's a point
	return *this;
}

Vector3D &Vector3D::operator*=(const double rhs) {
	for (int i = 0; i < NR_DIMS; i++) {
		this->*elts[i] *= rhs;
	}

	return *this;
}

Vector3D &Vector3D::operator*=(const Matrix &rhs) {
	//In order for the transformation to be valid: this should be true
	assert(rhs(1, 4) == 0);
	assert(rhs(2, 4) == 0);
	assert(rhs(3, 4) == 0);
	assert(rhs(4, 4) == 1);

	double temp[NR_DIMS + 1];

	// Make a copy of the original elements of this vector.
	for (int i = 0; i < NR_DIMS; i++) {
		temp[i] = this->*elts[i];
	}

	temp[NR_DIMS] = infty ? 0 : 1;

	for (int i = 0; i < NR_DIMS; i++) {
		this->*elts[i] = 0;

		// Multiply the original vector with the i-th column of the matrix.
		for (int j = 0; j <= NR_DIMS; j++) {
			this->*elts[i] += temp[j] * rhs.values[j][i];
		}
	}

	return *this;
}

double Vector3D::dot(const Vector3D &rhs) const {
	double result = 0;

	for (int i = 0; i < NR_DIMS; i++) {
		result += this->*elts[i] * rhs.*elts[i];
	}

	return result;
}

Vector3D &Vector3D::cross_equals(const Vector3D &rhs) {
	double temp[NR_DIMS];

	// Make a copy of the original elements of this vector.
	for (int i = 0; i < NR_DIMS; i++) {
		temp[i] = this->*elts[i];
	}

	for (int k = 0; k < NR_DIMS; k++) {
		const int i = (k + 1) % NR_DIMS;
		const int j = (k + 2) % NR_DIMS;

		this->*elts[k] = temp[i] * rhs.*elts[j] - temp[j] * rhs.*elts[i];
	}

	infty = true; // The result is always a vector.
	return *this;
}

double Vector3D::length() const {
	double squared_length = 0;

	for (int i = 0; i < NR_DIMS; ++i) {
		squared_length += this->*elts[i] * this->*elts[i];
	}

	return std::sqrt(squared_length);
}

void Vector3D::normalise() {
	*this /= length();
}

void Vector3D::print(std::ostream &output_stream,
					 const int elt_width) const {
	output_stream << "(";

	for (int i = 0; i < NR_DIMS; ++i) {
		output_stream << std::setw(elt_width) << this->*elts[i] << ", ";
	}

	output_stream << std::setw(elt_width) << (infty ? 0.0 : 1.0) << ")";
}

Vector3D operator+(const Vector3D &arg) {
	return arg;
}

Vector3D operator-(Vector3D arg) {
	return arg *= -1;
}

Vector3D operator+(Vector3D lhs,
				   const Vector3D &rhs) {
	return lhs += rhs;
}

Vector3D operator-(Vector3D lhs,
				   const Vector3D &rhs) {
	return lhs -= rhs;
}

Vector3D operator*(Vector3D lhs,
				   const double rhs) {
	return lhs *= rhs;
}

Vector3D operator*(const double lhs,
				   Vector3D rhs) {
	return rhs * lhs;
}

Vector3D &operator/=(Vector3D &lhs,
					 const double rhs) {
	return lhs *= 1 / rhs;
}

Vector3D operator/(Vector3D lhs,
				   const double rhs) {
	return lhs /= rhs;
}

Vector3D operator*(Vector3D lhs,
				   const Matrix &rhs) {
	return lhs *= rhs;
}

double Vector3D::dot(const Vector3D &lhs,
					 const Vector3D &rhs) {
	return lhs.dot(rhs);
}

Vector3D Vector3D::cross(Vector3D lhs,
						 const Vector3D &rhs) {
	return lhs.cross_equals(rhs);
}

Vector3D Vector3D::normalise(Vector3D arg) {
	arg.normalise();
	return arg;
}

Vector3D Vector3D::point(const std::vector<double> &point) {
	return Vector3D(point[0], point[1], point[2], false);

}

Vector3D Vector3D::vector(const std::vector<double> &vector) {
    return Vector3D(vector[0], vector[1], vector[2], true);
}

std::ostream &operator<<(std::ostream &output_stream,
						 const Vector3D &vector) {
	vector.print(output_stream);
	return output_stream;
}
