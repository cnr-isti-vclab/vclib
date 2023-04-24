/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "point.h"

#include <vclib/misc/hash.h>

namespace vcl {

/**
 * @brief Constructs a Point object from a set of scalar values.
 *
 * The constructor takes a variable number of scalar arguments, which are used to initialize the
 * components of the Point object. The number of arguments must be equal to the dimensionality of
 * the Point object, which is determined by the template parameter N.
 *
 * @tparam Scalars: The types of the scalar arguments.
 *
 * @param[in] scalars: The scalar arguments used to initialize the components of the Point object.
 */
template<typename Scalar, uint N>
template<typename... Scalars>
Point<Scalar, N>::Point(Scalars... scalars) requires(sizeof...(scalars) == N)
{
	Scalar args[N]   = {static_cast<Scalar>(scalars)...};
	for (uint i = 0; i < N; i++)
		p(i) = args[i];
}

/**
 * @brief Constructs a Point object from an Eigen row vector.
 *
 * The constructor initializes the components of the Point object from an Eigen row vector with the
 * same dimensionality as the Point object.
 *
 * @param[in] v: An Eigen row vector with the same dimensionality as the Point object.
 */
template<typename Scalar, uint N>
Point<Scalar, N>::Point(const Eigen::Matrix<Scalar, 1, N>& v)
{
	p << v;
}

/**
 * @brief Returns a reference to the x-component of the Point object.
 *
 * The function returns a reference to the first component of the Point object.
 * If the Point object has fewer than one component, calling this member function results in a
 * compile-time error.
 *
 * @return A reference to the x-component of the Point object.
 */
template<typename Scalar, uint N>
Scalar& Point<Scalar, N>::x() requires (N >= 1)
{
	return p(0);
}

/**
 * @brief Returns a const reference to the x-component of the Point object.
 *
 * The function returns a const reference to the first component of the Point object.
 * If the Point object has fewer than one component, calling this member function results in a
 * compile-time error.
 *
 * @return A const reference to the x-component of the Point object.
 */
template<typename Scalar, uint N>
const Scalar& Point<Scalar, N>::x() const requires (N >= 1)
{
	return p(0);
}

/**
 * @brief Returns a reference to the y-component of the Point object.
 *
 * The function returns a reference to the second component of the Point object.
 * If the Point object has fewer than two components, calling this member function results in a
 * compile-time error.
 *
 * @return A reference to the y-component of the Point object.
 */
template<typename Scalar, uint N>
Scalar& Point<Scalar, N>::y() requires (N >= 2)
{
	return p(1);
}

/**
 * @brief Returns a const reference to the y-component of the Point object.
 *
 * The function returns a const reference to the second component of the Point object.
 * If the Point object has fewer than two components, calling this member function results in a
 * compile-time error.
 *
 * @return A const reference to the y-component of the Point object.
 */
template<typename Scalar, uint N>
const Scalar& Point<Scalar, N>::y() const requires (N >= 2)
{
	return p(1);
}

/**
 * @brief Returns a reference to the z-component of the Point object.
 *
 * The function returns a reference to the third component of the Point object.
 * If the Point object has fewer than three components, calling this member function results in a
 * compile-time error.
 *
 * @return A reference to the z-component of the Point object.
 */
template<typename Scalar, uint N>
Scalar& Point<Scalar, N>::z() requires (N >= 3)
{
	return p(2);
}

/**
 * @brief Returns a const reference to the z-component of the Point object.
 *
 * The function returns a const reference to the third component of the Point object.
 * If the Point object has fewer than three components, calling this member function results in a
 * compile-time error.
 *
 * @return A const reference to the z-component of the Point object.
 */
template<typename Scalar, uint N>
const Scalar& Point<Scalar, N>::z() const requires (N >= 3)
{
	return p(2);
}

/**
 * @brief Returns a reference to the w-component of the Point object.
 *
 * The function returns a reference to the fourth component of the Point object.
 * If the Point object has fewer than four components, calling this member function results in a
 * compile-time error.
 *
 * @return A reference to the w-component of the Point object.
 */
template<typename Scalar, uint N>
Scalar& Point<Scalar, N>::w() requires (N >= 4)
{
	return p(3);
}

/**
 * @brief Returns a const reference to the w-component of the Point object.
 *
 * The function returns a const reference to the fourth component of the Point object.
 * If the Point object has fewer than four components, calling this member function results in a
 * compile-time error.
 *
 * @return A const reference to the w-component of the Point object.
 */
template<typename Scalar, uint N>
const Scalar& Point<Scalar, N>::w() const requires (N >= 4)
{
	return p(3);
}

/**
 * @brief Casts the Point object to a different scalar type.
 *
 * The function returns a new Point object with the same dimension as the original object,
 * but with each scalar value casted to a different type.
 *
 * @tparam S: The scalar type to cast to.
 *
 * @return A new Point object with the same dimension as the original object, but with each scalar
 * value casted to a different type.
 */
template<typename Scalar, uint N>
template<typename S>
Point<S, N> Point<Scalar, N>::cast() const
{
	if constexpr (std::is_same_v<Scalar, S>) {
		return *this;
	}
	else {
		return Point<S, N>(p.template cast<S>());
	}
}

/**
 * @brief Returns true if at least one of its components is NaN or inf.
 *
 * The function checks whether at least one of the scalar components of the Point object is NaN
 * (Not-a-Number) or inf (Infinity). If any component is NaN or inf, the function returns true,
 * indicating that the Point object is degenerate. Otherwise, the function returns false, indicating
 * that the Point object is not degenerate.
 *
 * @return true if the Point object is degenerate, false otherwise.
 */
template<typename Scalar, uint N>
bool Point<Scalar, N>::isDegenerate() const
{
	for (size_t i = 0; i < DIM; ++i)
		if (isDegenerate(p(i)))
			return true;
	return false;
}

/**
 * @brief Compares two Point objects within a given epsilon tolerance.
 *
 * The function compares two Point objects component-wise within a given epsilon tolerance.
 * If the difference between the corresponding components of the two Point objects is less than or
 * equal to the epsilon tolerance, the components are considered equal, and the function returns
 * true. Otherwise, the function returns false.
 *
 * @param[in] p1: The Point object to compare against.
 * @param[in] epsilon: The epsilon tolerance to use for the comparison.
 *
 * @return true if the two Point objects are considered equal within the epsilon tolerance, false
 * otherwise.
 */
template<typename Scalar, uint N>
bool Point<Scalar, N>::epsilonCompare(const Point &p1, Scalar epsilon)
{
	bool b = true;
	for (size_t i = 0; i < DIM; ++i)
		b = b && vcl::epsilonCompare(p(i), p1(i), epsilon);
	return b;
}

/**
 * @brief Computes the dot product of two Point objects.
 *
 * The function computes the dot product of two Point objects, which is defined as the sum of the
 * products of the corresponding components of the two Point objects. The two Point objects must
 * have the same dimension.
 *
 * @param[in] p1: The Point object to compute the dot product with.
 *
 * @return The dot product of the two Point objects.
 */
template<typename Scalar, uint N>
Scalar Point<Scalar, N>::dot(const Point& p1) const
{
	return p.dot(p1.p);
}

/**
 * @brief Computes the angle between two Point objects.
 *
 * The function computes the angle between two Point objects, which is defined as the inverse cosine
 * of the dot product of the two Point objects divided by the product of their magnitudes. The two
 * Point objects must have the same dimension.
 *
 * @param[in] p1: The Point object to compute the angle with.
 *
 * @return The angle between the two Point objects in radians, or -1 if the magnitude of one of the
 * Point objects is 0.
 */
template<typename Scalar, uint N>
Scalar Point<Scalar, N>::angle(const Point& p1) const
{
	Scalar w = p.norm() * p1.p.norm();
	if (w == 0)
		return -1;
	Scalar t = dot(p1) / w;
	if (t > 1)
		t = 1;
	else if (t < -1)
		t = -1;
	return (Scalar) acos(t);
}

/**
 * @brief Computes the Euclidean distance between two Point objects.
 *
 * The function computes the Euclidean distance between two Point objects, which is defined as the
 * square root of the sum of the squares of the differences of the corresponding components of the
 * two Point objects. The two Point objects must have the same dimension.
 *
 * @param[in] p1: The Point object to compute the distance to.
 *
 * @return The Euclidean distance between the two Point objects.
 */
template<typename Scalar, uint N>
Scalar Point<Scalar, N>::dist(const Point& p1) const
{
	return (p - p1.p).norm();
}

/**
 * @brief Computes the squared Euclidean distance between two Point objects.
 *
 * The function computes the squared Euclidean distance between two Point objects, which is defined
 * as the sum of the squares of the differences of the corresponding components of the two Point
 * objects. The two Point objects must have the same dimension.
 *
 * @param[in] p1: The Point object to compute the squared distance to.
 *
 * @return The squared Euclidean distance between the two Point objects.
 */
template<typename Scalar, uint N>
Scalar Point<Scalar, N>::squaredDist(const Point& p1) const
{
	return (p - p1.p).squaredNorm();
}

/**
 * @brief Returns the cross product between two points.
 *
 * This function returns the cross product between two points. The two points must have the same
 * dimension, and this function is available only on Points having size == 3.
 *
 * @param[in] p1: The other point to compute the cross product with.
 *
 * @return The cross product between this and the other point.
 *
 * @note This function is available only on Points having size == 3.
 */
template<typename Scalar, uint N>
Point<Scalar,N> Point<Scalar, N>::cross(const Point& p1) const requires (N == 3)
{
	return p.cross(p1.p);
}

/**
 * @brief Multiplies the components of two Point objects.
 *
 * This function multiplies the components of two Point objects element-wise, and returns the result
 * as a new Point object. The two Point objects must have the same dimension.
 *
 * @param[in] p1: The other Point object to multiply with.
 *
 * @return A new Point object that contains the resulting products of the component-wise
 * multiplication of the two Point objects.
 */
template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::mul(const Point& p1) const
{
	Point<Scalar, N> tmp;
	for (size_t i = 0; i < DIM; ++i)
		tmp[i] = p[i] * p1.p[i];
	return tmp;
}

/**
 * @brief Divides the components of two Point objects.
 *
 * This function divides the components of two Point objects element-wise, and returns the result as
 * a new Point object. The two Point objects must have the same dimension. If any component of the
 * second Point object is zero, the function throws a std::runtime_error exception.
 *
 * @param[in] p1: The other Point object to divide with.
 *
 * @return A new Point object that contains the resulting quotients of the component-wise division
 * of the two Point objects.
 *
 * @throws std::runtime_error if any component of the second Point object is zero.
 */
template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::div(const Point& p1) const
{
	Point<Scalar, N> tmp;
	for (size_t i = 0; i < DIM; ++i) {
		if (p1.p[i] == 0)
			throw std::runtime_error("Math error: Attempted to divide by Zero\n");
		tmp[i] = p[i] / p1.p[i];
	}
	return tmp;
}

/**
 * @brief Computes the Euclidean norm of the Point object.
 *
 * This function computes the Euclidean norm (magnitude) of the Point object, which is defined as
 * the square root of the sum of the squares of its components.
 *
 * @return The Euclidean norm of the Point object.
 */
template<typename Scalar, uint N>
Scalar Point<Scalar, N>::norm() const
{
	return p.norm();
}

/**
 * @brief Computes the squared Euclidean norm of the Point object.
 *
 * This function computes the squared Euclidean norm (magnitude squared) of the Point object, which
 * is defined as the sum of the squares of its components.
 *
 * @return The squared Euclidean norm of the Point object.
 */
template<typename Scalar, uint N>
Scalar Point<Scalar, N>::squaredNorm() const
{
	return p.squaredNorm();
}

/**
 * @brief Returns the size of the Point object.
 *
 * This function returns the size (number of components) of the Point object.
 *
 * @return The size of the Point object.
 */
template<typename Scalar, uint N>
constexpr uint Point<Scalar, N>::size() const
{
	return N;
}

/**
 * @brief Sets all the components of the Point object to a constant value.
 *
 * This function sets all the components of the Point object to a constant value specified by the
 * input parameter.
 *
 * @param s The constant value to set all the components of the Point object to.
 */
template<typename Scalar, uint N>
void Point<Scalar, N>::setConstant(Scalar s)
{
	p.setConstant(s);
}

/**
 * @brief Sets all the components of the Point object to zero.
 *
 * This function sets all the components of the Point object to zero.
 */
template<typename Scalar, uint N>
void Point<Scalar, N>::setZero()
{
	p.setZero();
}

/**
 * @brief Sets all the components of the Point object to one.
 *
 * This function sets all the components of the Point object to one.
 */
template<typename Scalar, uint N>
void Point<Scalar, N>::setOnes()
{
	p.setOnes();
}

/**
 * @brief Returns a normalized copy of the Point object.
 *
 * This function returns a normalized copy of the Point object, which has the same direction as the
 * original Point object but a magnitude of 1.
 *
 * @return A normalized copy of the Point object.
 *
 * @throws std::runtime_error if the norm of the Point object is zero.
 *
 * @sa norm()
 */
template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::normalized() const
{
	if (norm() == 0)
		throw std::runtime_error("Math error: Attempted to divide by Zero\n");

	return Point<Scalar, N>(p.array() / norm());
}

/**
 * @brief Normalizes the Point object in-place.
 *
 * This function normalizes the Point object in-place, which means that the direction of the Point
 * object is preserved but its magnitude is changed to 1.
 *
 * @throws std::runtime_error if the norm of the Point object is zero.
 *
 * @sa norm()
 */
template<typename Scalar, uint N>
void Point<Scalar, N>::normalize()
{
	if (norm() == 0)
		throw std::runtime_error("Math error: Attempted to divide by Zero\n");

	p /= norm();
}

/**
 * @brief Returns the outer product between this point p and p1, which is p * p1^T
 *
 * The returned type is a DIM*DIM Eigen Matrix, where DIM is the number of dimensions of the two
 * points.
 *
 * @param p1 The Point object to compute the outer product with.
 *
 * @return A DIM*DIM Eigen Matrix representing the outer product between this point p and p1.
 */
template<typename Scalar, uint N>
auto Point<Scalar, N>::outerProduct(const Point& p1) const
{
	Eigen::Matrix<ScalarType, DIM, DIM> res;
	for (uint i = 0; i < DIM; i++) {
		for (uint j = 0; j < DIM; j++) {
			res(i,j) = p(i) * p1(j);
		}
	}
	return res;
}

/**
 * @brief Computes an [Orthonormal Basis](https://en.wikipedia.org/wiki/Orthonormal_basis) starting
 * from this point n.
 *
 * This function computes an Orthonormal Basis starting from this point n. The orthonormal basis is
 * composed of three vectors: the input vector n and two output vectors u and v. The two output
 * vectors are orthogonal to n and each other, and have unit length.
 *
 * This function is available only on Points having size == 3.
 *
 * @param[out] u The first output vector of the orthonormal basis, orthogonal to n and v.
 * @param[out] v The second output vector of the orthonormal basis, orthogonal to n and u.
 *
 * @note This function uses the cross product operation, which is only defined for 3D vectors.
 */
template<typename Scalar, uint N>
void Point<Scalar, N>::orthoBase(Point& u, Point& v) const requires (N == 3)
{
	const double   LocEps = double(1e-7);
	Point3<Scalar> up(0, 1, 0);
	u = cross(up);

	double len = u.norm();
	if (len < LocEps) {
		if (std::abs(p[0]) < std::abs(p[1])) {
			if (std::abs(p[0]) < std::abs(p[2]))
				up = Point3<Scalar>(1, 0, 0); // x is the min
			else
				up = Point3<Scalar>(0, 0, 1); // z is the min
		}
		else {
			if (std::abs(p[1]) < std::abs(p[2]))
				up = Point3<Scalar>(0, 1, 0); // y is the min
			else
				up = Point3<Scalar>(0, 0, 1); // z is the min
		}
		u = cross(up);
	}
	v = cross(u);
}

/**
 * @brief Returns the Eigen Vector representation of the point.
 *
 * This function returns an Eigen Vector representation of the point, which is a one-row matrix with
 * N columns.
 *
 * @return An Eigen Vector representation of the point.
 */
template<typename Scalar, uint N>
const Eigen::Matrix<Scalar, 1, N>& Point<Scalar, N>::eigenVector() const
{
	return p;
}

/**
 * @brief Computes the hash value of the point.
 *
 * This function computes a hash value of the point, which can be used for hashing and comparison
 * purposes.
 *
 * @return The hash value of the point.
 */
template<typename Scalar, uint N>
std::size_t Point<Scalar, N>::hash() const
{
	std::size_t h = 0;
	for (size_t i = 0; i < DIM; ++i)
		vcl::hashCombine(h, p(i));
	return h;
}

/**
 * @brief Provides access to the i-th coordinate of the point.
 *
 * This operator provides read and write access to the i-th coordinate of the point. The index i is
 * zero-based and must be less than the dimension of the point.
 *
 * @param[in] i: The index of the coordinate to access.
 * @return A reference to the i-th coordinate of the point.
 *
 * @note The behavior of this operator is undefined if i is greater than or equal to the dimension
 * of the point.
 */
template<typename Scalar, uint N>
Scalar& Point<Scalar, N>::operator()(uint i)
{
	return p(i);
}

/**
 * @brief Provides read-only access to the i-th coordinate of the point.
 *
 * This operator provides read-only access to the i-th coordinate of the point. The index i is
 * zero-based and must be less than the dimension of the point.
 *
 * @param[in] i: The index of the coordinate to access.
 * @return A const reference to the i-th coordinate of the point.
 *
 * @note The behavior of this operator is undefined if i is greater than or equal to the dimension
 * of the point.
 */
template<typename Scalar, uint N>
const Scalar& Point<Scalar, N>::operator()(uint i) const
{
	return p(i);
}

/**
 * @brief Provides access to the i-th coordinate of the point.
 *
 * This operator provides read and write access to the i-th coordinate of the point. The index i is
 * zero-based and must be less than the dimension of the point.
 *
 * @param[in] i: The index of the coordinate to access.
 * @return A reference to the i-th coordinate of the point.
 *
 * @note The behavior of this operator is undefined if i is greater than or equal to the dimension
 * of the point.
 */
template<typename Scalar, uint N>
Scalar& Point<Scalar, N>::operator[](uint i)
{
	return p(i);
}

/**
 * @brief Provides read-only access to the i-th coordinate of the point.
 *
 * This operator provides read-only access to the i-th coordinate of the point. The index i is
 * zero-based and must be less than the dimension of the point.
 *
 * @param[in] i: The index of the coordinate to access.
 * @return A const reference to the i-th coordinate of the point.
 *
 * @note The behavior of this operator is undefined if i is greater than or equal to the dimension
 * of the point.
 */
template<typename Scalar, uint N>
const Scalar& Point<Scalar, N>::operator[](uint i) const
{
	return p(i);
}

/**
 * @brief Compares the point with another point using the spaceship operator.
 *
 * This operator compares the point with another point using the spaceship operator. The comparison
 * is performed by comparing the coordinates of the two points in lexicographic order. The
 * comparison stops as soon as a pair of coordinates that are not equal is found. If all coordinates
 * are equal, the comparison returns std::strong_ordering::equal.
 *
 * @param[in] p1: The point to compare with.
 * @return A three-way comparison result indicating the relative order of the two points.
 */
template<typename Scalar, uint N>
auto Point<Scalar, N>::operator<=>(const Point& p1) const
{
	uint i = 0;
	while (i < DIM && p[i] == p1.p[i]) {
		++i;
	}
	return i == DIM ? std::strong_ordering::equal : p[i] <=> p1.p[i];
}

/**
 * @brief Adds a scalar value to each coordinate of the point.
 *
 * This operator adds a scalar value to each coordinate of the point and returns the resulting
 * point. The scalar value is added component-wise to each coordinate of the point.
 *
 * @param[in] s: The scalar value to add.
 * @return The point obtained by adding the scalar value to each coordinate of the point.
 */
template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator+(const Scalar& s) const
{
	return Point<Scalar, N>(p.array() + s);
}

/**
 * @brief Adds another point to this point.
 *
 * This operator adds another point to this point and returns the resulting point. The addition is
 * performed component-wise on each coordinate of the two points.
 *
 * @param[in] p1: The point to add.
 * @return The point obtained by adding the other point to this point.
 */
template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator+(const Point<Scalar, N>& p1) const
{
	return Point<Scalar, N>(p + p1.p);
}

template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator-() const
{
	return Point<Scalar, N>(-p);
}

template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator-(const Scalar& s) const
{
	return Point<Scalar, N>(p.array() - s);
}

template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator-(const Point<Scalar, N>& p1) const
{
	return Point<Scalar, N>(p - p1.p);
}

template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator*(const Scalar& s) const
{
	return Point<Scalar, N>(p * s);
}

template<typename Scalar, uint N>
Scalar Point<Scalar, N>::operator*(const Point<Scalar, N>& p1) const
{
	return dot(p1);
}

template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator*(const Eigen::Matrix<Scalar, N, N>& m) const
{
	return Point<Scalar, N>(p * m);
}

/**
 * @brief Returns a new 3D point/vector on which has been applied an TRS 4x4 matrix.
 *
 * @note This function is available only on Points having size == 3.
 */
template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator*(
	const Eigen::Matrix<Scalar, N+1, N+1>& m) const requires (N == 3)
{
	Eigen::Matrix<Scalar, 1, N> s;

	s(0) = m(0, 0)*p(0) + m(0, 1)*p(1) + m(0, 2)*p(2) + m(0, 3);
	s(1) = m(1, 0)*p(0) + m(1, 1)*p(1) + m(1, 2)*p(2) + m(1, 3);
	s(2) = m(2, 0)*p(0) + m(2, 1)*p(1) + m(2, 2)*p(2) + m(2, 3);

	Scalar w = p(0)*m(3,0) + p(1)*m(3,1) +p(2)*m(3,2) + m(3,3);
	if (w != 0) s = s / w;
	return Point(s);
}

template<typename Scalar, uint N>
Point<Scalar, N> Point<Scalar, N>::operator/(const Scalar& s) const
{
	return Point<Scalar, N>(p / s);
}

template<typename Scalar, uint N>
Point<Scalar, N>& Point<Scalar, N>::operator+=(const Scalar& s)
{
	p = p.array() + s;
	return *this;
}

template<typename Scalar, uint N>
Point<Scalar, N>& Point<Scalar, N>::operator+=(const Point<Scalar, N>& p1)
{
	p += p1.p;
	return *this;
}

template<typename Scalar, uint N>
Point<Scalar, N>& Point<Scalar, N>::operator-=(const Scalar& s)
{
	p = p.array() - s;
	return *this;
}

template<typename Scalar, uint N>
Point<Scalar, N>& Point<Scalar, N>::operator-=(const Point<Scalar, N>& p1)
{
	p -= p1.p;
	return *this;
}

template<typename Scalar, uint N>
Point<Scalar, N>& Point<Scalar, N>::operator*=(const Scalar& s)
{
	p *= s;
	return *this;
}

template<typename Scalar, uint N>
Point<Scalar, N>& Point<Scalar, N>::operator*=(const Eigen::Matrix<Scalar, N, N>& m)
{
	p *= m;
	return *this;
}

/**
 * @brief Applies to the given point/vector an TRS 4x4 matrix.
 * @param m
 *
 * @note This function is available only on Points having size == 3.
 */
template<typename Scalar, uint N>
Point<Scalar,N>& Point<Scalar, N>::operator*=(
	const Eigen::Matrix<Scalar, N+1, N+1>& m) requires (N == 3)
{
	*this = *this * m;
	return *this;
}

template<typename Scalar, uint N>
Point<Scalar, N>& Point<Scalar, N>::operator/=(const Scalar& s)
{
	p /= s;
	return *this;
}

template<typename Scalar, uint N>
Point<Scalar, N>& Point<Scalar, N>::operator=(const Eigen::Matrix<Scalar, 1, N>& v)
{
	p << v;
	return *this;
}

template<typename Scalar, uint N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p1)
{
	out << p1.p;
	return out;
}

} // namespace vcl

namespace std {

template<typename Scalar, uint N>
std::size_t hash<vcl::Point<Scalar, N> >::operator()(const vcl::Point<Scalar, N>& id) const noexcept
{
	return id.hash();
}

} // namespace std
