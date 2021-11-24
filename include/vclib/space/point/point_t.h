/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_POINT_T_H
#define VCL_POINT_T_H

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vclib/math/base.h>
#include <vclib/misc/vcl_types.h>

namespace vcl {

/**
 * @brief The Point class represents an N-dimensional point containing
 * N scalar values.
 */
template<typename Scalar, int N>
class Point
{
public:
	using ScalarType      = Scalar;
	static const int DIM  = N;
	Point()               = default; // default empty constructor
	Point(const Point& p) = default; // default copy constructor
	Point(Point&& p)      = default; // default move constructor

	Point& operator=(const Point& p) = default; // default copy assignment operator
	Point& operator=(Point&& p) = default;      // default move assignment operator

	template<typename S>
	Point(const Point<S, N>& p);                 // constructor from different scalar Point
	Point(const Eigen::Matrix<Scalar, 1, N>& v); // constructor from 1xN eigen matrix

	bool isDegenerate() const;

	template<typename S>
	Scalar dot(const Point<S, N>& p1) const;

	template<typename S>
	Scalar angle(const Point<S, N>& p1) const;

	template<int U = N>
	VCL_ENABLE_IF(U==3, Point<Scalar VCL_COMMA N>)
	cross(const Point<Scalar, N>& p1) const;

	Scalar       norm() const;
	Scalar       squaredNorm() const;
	unsigned int size() const;

	void setConstant(Scalar s);
	void setZero();
	void setOnes();

	void normalize();

	// operators
	Scalar&       operator()(unsigned int i);
	const Scalar& operator()(unsigned int i) const;

	bool operator==(const Point& p1) const;
	bool operator!=(const Point& p1) const;
	bool operator<(const Point& p1) const;

	Point  operator+(const Scalar& s) const;
	Point  operator+(const Point& p1) const;
	Point  operator-(const Scalar& s) const;
	Point  operator-(const Point& p1) const;
	Point  operator*(const Scalar& s) const;
	Scalar operator*(const Point& p1) const;
	Point  operator/(const Scalar& s) const;
	Point& operator+=(const Point& p1);
	Point& operator-=(const Point& p1);
	Point& operator*=(const Scalar& s);
	Point& operator/=(const Scalar& s);

	Scalar  operator[](size_t i) const;
	Scalar& operator[](size_t i);

	template<typename S, int M>
	friend std::ostream& operator<<(std::ostream& out, const Point<S, M>& p);

protected:
	Eigen::Matrix<Scalar, 1, N> p;
};

template<typename Scalar, int N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p);

} // namespace vcl

#include "point_t.cpp"

#endif // VCL_POINT_T_H
