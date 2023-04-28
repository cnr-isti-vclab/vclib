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

#ifndef VCL_SPACE_POINT_H
#define VCL_SPACE_POINT_H

#include <compare>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vclib/concepts/space/point.h>
#include <vclib/math/base.h>

namespace vcl {

/**
 * @brief The Point class represents an N-dimensional point containing N scalar values.
 *
 * The Point class template represents an N-dimensional point containing N scalar values. The scalar
 * type and the number of dimensions are template parameters of the class. The class provides a
 * number of member functions for accessing, manipulating, and comparing points, as well as
 * arithmetic and assignment operators for points.
 *
 * The Point class is implemented using an Eigen matrix of size 1xN to store the point components.
 * The class also defines a number of type aliases and static constants for convenience.
 *
 * @tparam Scalar: The scalar type of the point components.
 * @tparam N: The number of dimensions of the point.
 *
 * @ingroup space
 */
template<typename Scalar, uint N>
class Point
{
	template<typename S, uint M>
	friend class Point;

public:
	/**
	 * @brief the Scalar type of the Point
	 */
	using ScalarType      = Scalar;

	/**
	 * @brief DIM: the number of dimensions of the Point
	 */
	static const uint DIM  = N;

	Point() = default; // default empty constructor

	template<typename... Scalars>
	Point(Scalars... scalars) requires(sizeof...(scalars) == N);

	Point(const Eigen::Matrix<Scalar, 1, N>& v); // constructor from 1xN eigen matrix

	ScalarType& x() requires (N >= 1);
	const ScalarType& x() const requires (N >= 1);
	ScalarType& y() requires (N >= 2);
	const ScalarType& y() const requires (N >= 2);
	ScalarType& z() requires (N >= 3);
	const ScalarType& z() const requires (N >= 3);
	ScalarType& w() requires (N >= 4);
	const ScalarType& w() const requires (N >= 4);

	template<typename S>
	Point<S, N> cast() const;

	bool isDegenerate() const;

	bool epsilonCompare(const Point& p1, Scalar epsilon);

	Scalar dot(const Point& p1) const;
	Scalar angle(const Point& p1) const;
	Scalar dist(const Point& p1) const;
	Scalar squaredDist(const Point& p1) const;

	Point cross(const Point& p1) const requires (N == 3);

	Point mul(const Point& p1) const;
	Point div(const Point& p1) const;

	Scalar norm() const;
	Scalar squaredNorm() const;
	constexpr uint size() const;

	void setConstant(Scalar s);
	void setZero();
	void setOnes();

	Point<Scalar, N> normalized() const;
	void             normalize();

	auto outerProduct(const Point& p1) const;
	void orthoBase(Point& u, Point& v) const requires (N == 3);

	const Eigen::Matrix<Scalar, 1, N>& eigenVector() const;

	std::size_t hash() const;

	// operators
	Scalar&       operator()(uint i);
	const Scalar& operator()(uint i) const;
	Scalar& operator[](uint i);
	const Scalar& operator[](uint i) const;

	bool operator==(const Point& p1) const = default;
	auto operator<=>(const Point& p1) const;

	Point operator+(const Scalar& s) const;
	Point operator+(const Point& p1) const;

	Point operator-() const;
	Point operator-(const Scalar& s) const;
	Point operator-(const Point& p1) const;

	Point  operator*(const Scalar& s) const;
	Scalar operator*(const Point& p1) const;
	Point operator*(const Eigen::Matrix<Scalar, N, N>& m) const;
	Point operator*(const Eigen::Matrix<Scalar, N+1, N+1>& m) const requires (N == 3);

	Point  operator/(const Scalar& s) const;

	Point& operator+=(const Scalar& s);
	Point& operator+=(const Point& p1);

	Point& operator-=(const Scalar& s);
	Point& operator-=(const Point& p1);

	Point& operator*=(const Scalar& s);
	Point& operator*=(const Eigen::Matrix<Scalar, N, N>& m);
	Point& operator*=(const Eigen::Matrix<Scalar, N+1, N+1>& m) requires (N == 3);

	Point& operator/=(const Scalar& s);

	Point& operator=(const Eigen::Matrix<Scalar, 1, N>& v);

	/// @private
	template<typename S, uint M>
	friend std::ostream& operator<<(std::ostream& out, const Point<S, M>& p);

protected:
	Eigen::Matrix<Scalar, 1, N> p = Eigen::Matrix<Scalar, 1, N>::Zero();
};

// Specializations

/**
 * @brief A convenience alias for a 2-dimensional Point.
 *
 * The Point2 alias is a shorthand for a Point class template specialization with a scalar type of
 * Scalar and two dimensions. It is implemented as an alias template for the Point class template.
 *
 * @tparam Scalar: The scalar type of the point components.
 *
 * @ingroup space
 */
template<typename Scalar>
using Point2 = Point<Scalar, 2>;

/**
 * @brief A convenience alias for a 2-dimensional Point with integer components.
 *
 * The Point2i alias is a shorthand for a Point class template specialization with integer
 * components and two dimensions. It is implemented as an alias template for the Point2 alias
 * template.
 *
 * @ingroup space
 */
using Point2i = Point2<int>;

/**
 * @brief A convenience alias for a 2-dimensional Point with floating-point components.
 *
 * The Point2f alias is a shorthand for a Point class template specialization with floating-point
 * components and two dimensions. It is implemented as an alias template for the Point2 alias
 * template.
 *
 * @ingroup space
 */
using Point2f = Point2<float>;

/**
 * @brief A convenience alias for a 2-dimensional Point with double-precision floating-point
 * components.
 *
 * The Point2d alias is a shorthand for a Point class template specialization with double-precision
 * floating-point components and two dimensions. It is implemented as an alias template for the
 * Point2 alias template.
 *
 * @ingroup space
 */
using Point2d = Point2<double>;

/**
 * @brief A convenience alias for a 3-dimensional Point.
 *
 * The Point3 alias is a shorthand for a Point class template specialization with a scalar type of
 * Scalar and three dimensions. It is implemented as an alias template for the Point class template.
 *
 * @tparam Scalar: The scalar type of the point components.
 *
 * @ingroup space
 */
template<typename Scalar>
using Point3 = Point<Scalar, 3>;

/**
 * @brief A convenience alias for a 3-dimensional Point with integer components.
 *
 * The Point3i alias is a shorthand for a Point class template specialization with integer
 * components and three dimensions. It is implemented as an alias template for the Point3 alias
 * template.
 *
 * @ingroup space
 */
using Point3i = Point3<int>;

/**
 * @brief A convenience alias for a 3-dimensional Point with floating-point components.
 *
 * The Point3f alias is a shorthand for a Point class template specialization with floating-point
 * components and three dimensions. It is implemented as an alias template for the Point3 alias
 * template.
 *
 * @ingroup space
 */
using Point3f = Point3<float>;

/**
 * @brief A convenience alias for a 3-dimensional Point with double-precision floating-point
 * components.
 *
 * The Point3d alias is a shorthand for a Point class template specialization with double-precision
 * floating-point components and three dimensions. It is implemented as an alias template for the
 * Point3 alias template.
 *
 * @ingroup space
 */
using Point3d = Point3<double>;

/**
 * @brief A convenience alias for a 4-dimensional Point.
 *
 * The Point4 alias is a shorthand for a Point class template specialization with a scalar type of
 * Scalar and four dimensions. It is implemented as an alias template for the Point class template.
 *
 * @tparam Scalar: The scalar type of the point components.
 *
 * @ingroup space
 */
template<typename Scalar>
using Point4 = Point<Scalar, 4>;

/**
 * @brief A convenience alias for a 4-dimensional Point with integer components.
 *
 * The Point4i alias is a shorthand for a Point class template specialization with integer
 * components and four dimensions. It is implemented as an alias template for the Point4 alias
 * template.
 *
 * @ingroup space
 */
using Point4i = Point4<int>;

/**
 * @brief A convenience alias for a 4-dimensional Point with floating-point components.
 *
 * The Point4f alias is a shorthand for a Point class template specialization with floating-point
 * components and four dimensions. It is implemented as an alias template for the Point4 alias
 * template.
 *
 * @ingroup space
 */
using Point4f = Point4<float>;

/**
 * @brief A convenience alias for a 4-dimensional Point with double-precision floating-point
 * components.
 *
 * The Point4d alias is a shorthand for a Point class template specialization with double-precision
 * floating-point components and four dimensions. It is implemented as an alias template for the
 * Point4 alias template.
 *
 * @ingroup space
 */
using Point4d = Point4<double>;


// Deduction guides
template<typename S, typename... Scalars>
Point(S, Scalars... scalars) -> Point<S, sizeof...(Scalars) + 1>;

template<typename Scalar,  uint N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p1);

} // namespace vcl

// inject vcl::Point hash function in std namespace
namespace std {
template <typename Scalar, uint N>
struct hash<vcl::Point<Scalar, N> >
{
	size_t operator()(const vcl::Point<Scalar, N>& id) const noexcept;
};
} // namespace std

#include "point.cpp"

#endif // VCL_SPACE_POINT_H
