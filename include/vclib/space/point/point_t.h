/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_SPACE_POINT_POINT_T_H
#define VCL_SPACE_POINT_POINT_T_H

#include <compare>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <vclib/math/base.h>
#include <vclib/misc/types.h>

namespace vcl {

/**
 * @brief The Point class represents an N-dimensional point containing
 * N scalar values.
 */
template<typename Scalar, int N>
class Point
{
	template<typename S, int M>
	friend class Point;

public:
	using ScalarType      = Scalar;
	static const int DIM  = N;

	Point() = default; // default empty constructor

	Point(const Eigen::Matrix<Scalar, 1, N>& v); // constructor from 1xN eigen matrix

	template<typename S>
	Point<S, N> cast() const;

	bool isDegenerate() const;

	bool epsilonCompare(const Point& p1, Scalar epsilon);

	Scalar dot(const Point& p1) const;
	Scalar angle(const Point& p1) const;
	Scalar dist(const Point& p1) const;
	Scalar squaredDist(const Point& p1) const;

	Point cross(const Point& p1) const requires (N == 3);

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

	Scalar& operator[](uint i);
	const Scalar& operator[](uint i) const;

	Point& operator=(const Eigen::Matrix<Scalar, 1, N>& v);

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

#endif // VCL_SPACE_POINT_POINT_T_H
