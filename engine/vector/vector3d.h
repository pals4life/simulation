/*
 * vector3d.hh
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

#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED

#include <iostream>
#include <vector>

#define NR_DIMS 3


/**
 * \brief A class that stores 4x4 matrices.
 *
 * This class can be used to represent transformations of vectors.
 */
class Matrix {
private:

	/**
	 * \brief The elements of the matrix.
	 */
	double values[NR_DIMS + 1][NR_DIMS + 1];

	/**
	 * \brief Prints the row this matrix.
	 *
	 * \param output_stream The output stream to which the matrix is printed.
	 * \param row_index        The index of the row.
	 * \param elt_width     The amount of space that is reserved to print an element.
	 */
	void print_line(std::ostream &output_stream,
					const int row_index,
					const int elt_width) const;

public:

	/**
	 * \brief Vector3D is our friend so it can multiply itself with us.
	 */
	friend class Vector3D;

	/**
	 * \brief Inverts a matrix.
	 *
	 * \param matrix The matrix that is inverted.
	 *
	 * \return The inverted matrix.
	 */
	static Matrix inv(Matrix matrix);

	/**
	 * \brief Constructs a new unity matrix.
	 */
	Matrix();

	/**
	 * \brief Constructs a new matrix by copying its elements from another one.
	 */
	Matrix(const Matrix &original);

	/**
	 * \brief Destructs the matrix.
	 */
	~Matrix();

	/**
	 * \brief Copies the elements from another matrix.
	 *
	 * \param original The matrix whose elements are copied.
	 *
	 * \return A reference to this matrix.
	 */
	Matrix &operator=(const Matrix &original);

	/**
	 * \brief Returns a reference to an element of the matrix.
	 *
	 * \param row_ix The row index of the referenced element, starting from 1.
	 * \param col_ix The column index of the referenced element, starting from 1.
	 *
	 * \return A reference to the referenced element.
	 */
	double &operator()(const int row_ix,
					   const int col_ix);

	/**
	 * \brief Returns the value of an element of the matrix.
	 *
	 * \param row_ix The row index of the referenced element, starting from 1.
	 * \param col_ix The column index of the referenced element, starting from 1.
	 *
	 * \return The value of the referenced element.
	 */
	double operator()(const int row_ix,
					  const int col_ix) const;

	/**
	 * \brief Multiplies a matrix with this matrix.
	 *
	 * \param rhs The matrix with which this matrix is multiplied.
	 *
	 * \return A reference to this matrix.
	 */
	Matrix &operator*=(const Matrix &rhs);

	/**
	 * \brief Inverts this matrix.
	 */
	void inv();

	/**
	 * \brief Prints a human-readable representation of the matrix.
	 *
	 * This method does not print a trailing newline.
	 *
	 * \param output_stream The output stream to which the matrix is printed.
	 * \param elt_width     The amount of space that is reserved to print an element.
	 */
	void print(std::ostream &output_stream,
			   const int elt_width = 8) const;
};

/**
 * \brief Multiplies two matrices
 *
 * Multiplies two matrices and returns the result
 *
 * \param lhs the left-hand side of the operation
 * \param rhs the right-hand side of the operation
 *
 * \returns the resulting matrix
 */
Matrix operator*(Matrix lhs,
				 const Matrix &rhs);

/**
 * \brief Writes a matrix to an output stream.
 *
 * This function uses the Matrix::print method to print the vector.
 *
 * \param output_stream The output stream to which the matrix is written.
 * \param matrix        The matrix that is written.
 *
 * \return A reference to the output stream.
 */
std::ostream &operator<<(std::ostream &output_stream,
						 const Matrix &matrix);


/**
 * \brief A class that represents 3D vectors.
 *
 * This class can both represent points and directions.  A point can be
 * constructed using the Vector3D::point pseudo-constructor.  A vector can be
 * constructed using the Vector3D::vector pseudo-constructor.  Transforming a
 * vector will behave accordingly.
 */
class Vector3D {
private:

	static double Vector3D::* const elts[];

	/**
	 * \brief A flag that indicates whether this vector is a point (\c false) or a
	 * vector (\c true).
	 */
	bool infty;

protected:

	/**
	 * \brief Constructs a new Vector3D object given its coordinates.
	 *
	 * This constructor is made protected to avoid it to be called directly.  In
	 * order to construct a new instance of this class the Vector3D::point or
	 * Vector3D::vector pseudo-constructors should be used.
	 *
	 * \param x_init The x-coordinate.
	 * \param y_init The y-coordinate.
	 * \param z_init The z-coordinate.
	 * \param infty_init \c false if the vector represents a point, \c true if it represents a vector.
	 */
	Vector3D(const double x_init,
			 const double y_init,
			 const double z_init,
			 const bool infty_init);

	/**
	 * \brief Consructs a new Vector3D object by copying another one.
	 *
	 * This constructor is made protected to avoid it to be called direcly.  In order
	 * to construct a new instance of this class the Vector3D::point or
	 * Vector3D::vector pseudo-constructors should be used.
	 *
	 * \param original The vector that is copied.
	 * \param infty_init \c false if the vector represents a point, \c true if it represents a vector.
	 */
	Vector3D(const Vector3D &original,
			 const bool infty_init);

public:

	/**
	 * \brief The x-coordinate of the vector.
	 */
	double x;

	/**
	 * \brief The y-coordinate of the vector.
	 */
	double y;

	/**
	 * \brief The z-coordinate of the vector.
	 */
	double z;

	/**
	 * \brief Constructs a new Vector3D object that represents the origin.
	 */
	Vector3D();

	/**
	 * \brief Constructs a new Vector3D object by copying another one.
	 *
	 * \param original The vector that is copied.
	 */
	Vector3D(const Vector3D &original);

	/**
	 * \brief Destructs a vector.
	 */
	~Vector3D();

	/**
	 * \brief Constructs a new Vector3D object that represents a point.
	 *
	 * \param x The x-coordinate.
	 * \param y The y-coordinate.
	 * \param z The z-coordinate.
	 */
	static Vector3D point(const double x,
						  const double y,
						  const double z);

	static Vector3D point(const std::vector<double> &point);

    static Vector3D vector(const std::vector<double> &vector);

	/**
	 * \brief Constructs a new Vector3D object that represents a point.
	 *
	 * \param original The vector whose coordinates are copied.
	 */
	static Vector3D point(const Vector3D &original);

	/**
	 * \brief Constructs a new Vector3D object that represents a vector.
	 *
	 * \param x The x-coordinate.
	 * \param y The y-coordinate.
	 * \param z The z-coordinate.
	 */
	static Vector3D vector(const double x,
						   const double y,
						   const double z);

	/**
	 * \brief Constructs a new Vector3D object that represents a vector.
	 *
	 * \param original The vector whose coordinates are copied.
	 */
	static Vector3D vector(const Vector3D &original);

	/**
	 * \brief Returns whether this object represents a point.
	 *
	 * \return \c true if this object represents a point, \c false otherwise.
	 */
	bool is_point() const;

	/**
	 * \brief Returns whether this object represents a vector.
	 *
	 * \return \c true if this object represents a vector, \c false otherwise.
	 */
	bool is_vector() const;

	/**
	 * \brief Assignment operator.
	 *
	 * \param original The vector that is copied.
	 *
	 * \return A reference to this vector.
	 */
	Vector3D &operator=(const Vector3D &original);

	/**
	 * \brief Adds another Vector3D object to this one.
	 *
	 * If both objects represent vectors the result will also be a vector.
	 * Otherwise the result is a point.
	 *
	 * \param rhs The vector that is added to this vector.
	 *
	 * \return A reference to this vector.
	 */
	Vector3D &operator+=(const Vector3D &rhs);

	/**
	 * \brief Subtracts another Vector3D object from this one.
	 *
	 * Subtracting a vector from a point or a point from a vector will result
	 * in a point.  Subtracting two vectors or two points will result in a
	 * vector.
	 *
	 * \param rhs The Vector3D object that is subtracted from this one.
	 *
	 * \return A reference to this vector.
	 */
	Vector3D &operator-=(const Vector3D &rhs);

	/**
	 * \brief Multiplies a scalar with this vector or point.
	 *
	 * \param rhs The scalar that is multiplied with this object.
	 *
	 * \return A reference to this vector.
	 */
	Vector3D &operator*=(const double rhs);

	/**
	 * \brief Applies a transformation
	 *
	 * Please note that before the transformation is actually performed, assertions are used to make sure that passed Matrix objects represents a VALID
	 * transformation. To this end, the last column of the matrix MUST equal:
	 * (0)
	 * (0)
	 * (0)
	 * (1)
	 * That is the matrix itself must be of the form:
	 * ( a, b, c, 0 )
	 * ( d, e, f, 0 )
	 * ( g, h, i, 0 )
	 * ( j, k, l, 1 )
	 *
	 * \param rhs The matrix that is multiplied with this Vector3D object.
	 *
	 * \return A reference to this object.
	 */
	Vector3D &operator*=(const Matrix &rhs);

	/**
	 * \brief Calculates the dot-product of this vector and another one.
	 *
	 * This operation will always succeed regardless of whether the operands
	 * represent points or vectors.  In case this operation is applied to a point
	 * it will be treated as a vector from the origin to the point.  Note that
	 * performing the dot product on points does not make much sense.
	 *
	 * \param rhs The Vector3D object to be multiplied with this.
	 *
	 * \return The dot product of this vector and \p rhs.
	 */
	double dot(const Vector3D &rhs) const;

	/**
	 * \brief Calculates the cross-product of this vector and another one.
	 *
	 * This operation will always succeed regardless of whether the operands
	 * represent points or directions.  In case this operation is applied to a point
	 * it will be treated as a vector from the origin to the point.  Note that
	 * performing the dot product on points does not make much sense.
	 *
	 * \param rhs The vector the is multiplied with this vector.
	 *
	 * \return A reference to this vector.
	 */
	Vector3D &cross_equals(const Vector3D &rhs);

	/**
	 * \brief Determines the length of the vector.
	 *
	 * In case the vector represents a point, the distance between the point and the
	 * origin is returned.
	 *
	 * \return The length of the vector
	 */
	double length() const;

	/**
	 * \brief Normalises the vector.
	 *
	 * This operation scales the vector such that it has a length of 1.  If the
	 * vector represents a point the point is translated along the line that connects
	 * it to the origin such that the distance between it and the origin is 1.
	 */
	void normalise();

	/**
	 * \brief Prints a human-readable representation of the vector.
	 *
	 * \param output_stream The output stream to which the vector is printed.
	 * \param elt_width     The amount of space that is reserved to print an element.
	 */
	void print(std::ostream &output_stream,
			   const int elt_width = 8) const;


	/**
	 * \brief Calculates the dot-product of two vectors.
	 *
	 * \param lhs The left factor.
	 * \param rhs The right factor.
	 *
	 * \return The dot-product of \p lhs and \p rhs.
	 */
	static double dot(const Vector3D &lhs,
					  const Vector3D &rhs);

	/**
	 * \brief Calculates the cross-product of two vectors.
	 *
	 * \param lhs The left factor.
	 * \param rhs The right factor.
	 *
	 * \return The cross-product of \p lhs and \p rhs.
	 */
	static Vector3D cross(Vector3D lhs,
						  const Vector3D &rhs);

	/**
	 * \brief Normalises a vector.
	 *
	 * This function uses Vector3D::normalise to normalise a vector.
	 *
	 * \param arg The vector that is normalised.
	 *
	 * \return The normalised vector.
	 */
	static Vector3D normalise(Vector3D arg);
};

/**
 * \brief Returns the passed parameter unchanged.
 *
 * This function is here for convenience.  It allows compilation of expressions
 * like +v.
 *
 * \param arg The parameter that is returned unchanged.
 *
 * \return The parameter that was passed to this function.
 */
Vector3D operator+(const Vector3D &arg);

/**
 * \brief Negates a vector.
 *
 * This operation is performed by multiplying the vector with the scalar -1.
 *
 * \param arg The vector that is negated.
 *
 * \return The negated vector.
 */
Vector3D operator-(Vector3D arg);

/**
 * \brief Adds two vectors.
 *
 * \param lhs The left term.
 * \param rhs The right term.
 *
 * \return The sum of \p lhs and \p rhs.
 */
Vector3D operator+(Vector3D lhs,
				   const Vector3D &rhs);

/**
 * \brief Subtracts two vectors.
 *
 * \param lhs The minuend.
 * \param rhs The subtrahend.
 *
 * \return The difference of \p lhs and \p rhs.
 */
Vector3D operator-(Vector3D lhs,
				   const Vector3D &rhs);

/**
 * \brief Multiplies a vector with a scalar.
 *
 * \param lhs The vector.
 * \param rhs The scalar.
 *
 * \return The multiplied vector.
 */
Vector3D operator*(Vector3D lhs,
				   const double rhs);

/**
 * \brief Multiplies a vector with a scalar.
 *
 * \param lhs The scalar.
 * \param rhs The vector.
 *
 * \return The multiplied vector.
 */
Vector3D operator*(const double lhs,
				   Vector3D rhs);

/**
 * \brief Divides a vector by a scalar.
 *
 * \param lhs The vector.
 * \param rhs The scalar.
 *
 * \return A reference to the vector.
 */
Vector3D &operator/=(Vector3D &lhs,
					 const double rhs);

/**
 * \brief Divides a vector by a scalar.
 *
 * \param lhs The vector.
 * \param rhs The scalar.
 *
 * \return The vector.
 */
Vector3D operator/(Vector3D lhs,
				   const double rhs);

/**
 * \brief Applies a transformation to a vector.
 *
 * \param lhs The vector.
 * \param rhs The matrix that represents the transformation.
 *
 * \return The transformed vector.
 */
Vector3D operator*(Vector3D lhs,
				   const Matrix &rhs);

/**
 * \brief Writes a vector to an output stream.
 *
 * This function uses the Vector3D::print method to print the vector.
 *
 * \param output_stream The output stream to which the vector is written.
 * \param vector        The vector that is written.
 *
 * \return A reference to the output stream.
 */
std::ostream &operator<<(std::ostream &output_stream,
						 const Vector3D &vector);


#endif // VECTOR_INCLUDED
