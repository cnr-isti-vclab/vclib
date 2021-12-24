/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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
	VCL_ENABLE_IF(U==3, Point)
	cross(const Point<Scalar, N>& p1) const;

	Scalar norm() const;
	Scalar squaredNorm() const;
	uint   size() const;

	void setConstant(Scalar s);
	void setZero();
	void setOnes();

	Point<Scalar, N> normalized() const;
	void             normalize();

	const Eigen::Matrix<Scalar, 1, N>& eigenVector() const;

	// operators
	Scalar&       operator()(uint i);
	const Scalar& operator()(uint i) const;

	bool operator==(const Point& p1) const;
	bool operator!=(const Point& p1) const;
	bool operator<(const Point& p1) const;

	Point operator+(const Scalar& s) const;
	Point operator+(const Point& p1) const;

	Point operator-() const;
	Point operator-(const Scalar& s) const;
	Point operator-(const Point& p1) const;

	Point  operator*(const Scalar& s) const;
	Scalar operator*(const Point& p1) const;
	template<typename SM>
	Point operator*(const Eigen::Matrix<SM, N, N>& m) const;
	template<typename SM, int U = N>
	VCL_ENABLE_IF(U==3, Point)
	operator*(const Eigen::Matrix<SM, N+1, N+1>& m) const;

	Point  operator/(const Scalar& s) const;

	Point& operator+=(const Scalar& s);
	Point& operator+=(const Point& p1);

	Point& operator-=(const Scalar& s);
	Point& operator-=(const Point& p1);

	Point& operator*=(const Scalar& s);
	template<typename SM>
	Point& operator*=(const Eigen::Matrix<SM, N, N>& m);
	template<typename SM, int U = N>
	VCL_ENABLE_IF(U==3, Point&)
	operator*=(const Eigen::Matrix<SM, N+1, N+1>& m);

	Point& operator/=(const Scalar& s);

	Scalar& operator[](uint i);
	const Scalar& operator[](uint i) const;

	/// @private
	template<typename S, int M>
	friend std::ostream& operator<<(std::ostream& out, const Point<S, M>& p);

protected:
	Eigen::Matrix<Scalar, 1, N> p;
};

template<typename Scalar, int N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p1);

} // namespace vcl

#include "point_t.cpp"

#endif // VCL_POINT_T_H
