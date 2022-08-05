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

#include "point_t.h"

namespace vcl {

template<typename Scalar, int N>
Point<Scalar, N>::Point(const Eigen::Matrix<Scalar, 1, N>& v)
{
	p << v;
}

template<typename Scalar, int N>
template<typename S>
Point<S, N> Point<Scalar, N>::cast() const
{
	if constexpr (std::is_same<Scalar, S>::value) {
		return *this;
	}
	else {
		return Point<S, N>(p.template cast<S>());
	}
}

/**
 * @brief Returns true if at least one of its components is NaN or inf.
 * @return true if the point is degenerate, false otherwise.
 */
template<typename Scalar, int N>
bool Point<Scalar, N>::isDegenerate() const
{
	for (size_t i = 0; i < DIM; ++i)
		if (isDegenerate(p(i)))
			return true;
	return false;
}

template<typename Scalar, int N>
bool Point<Scalar, N>::epsilonCompare(const Point &p1, Scalar epsilon)
{
	bool b = true;
	for (size_t i = 0; i < DIM; ++i)
		b = b && vcl::epsilonCompare(p(i), p1(i), epsilon);
	return b;
}

template<typename Scalar, int N>
Scalar Point<Scalar, N>::dot(const Point& p1) const
{
	return p.dot(p1.p);
}

template<typename Scalar, int N>
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

template<typename Scalar, int N>
Scalar Point<Scalar, N>::dist(const Point& p1) const
{
	return (p - p1.p).norm();
}

template<typename Scalar, int N>
Scalar Point<Scalar, N>::squaredDist(const Point& p1) const
{

	return (p - p1.p).squaredNorm();
}

/**
 * @brief Returns the cross product between two points.
 * @note This function is available only on Points having size == 3.
 * @return The cross product between this and the other point.
 */
template<typename Scalar, int N>
Point<Scalar,N> Point<Scalar, N>::cross(const Point& p1) const requires (N == 3)
{
	return p.cross(p1.p);
}

template<typename Scalar, int N>
Scalar Point<Scalar, N>::norm() const
{
	return p.norm();
}

template<typename Scalar, int N>
Scalar Point<Scalar, N>::squaredNorm() const
{
	return p.squaredNorm();
}

template<typename Scalar, int N>
uint Point<Scalar, N>::size() const
{
	return p.size();
}

template<typename Scalar, int N>
void Point<Scalar, N>::setConstant(Scalar s)
{
	p.setConstant(s);
}

template<typename Scalar, int N>
void Point<Scalar, N>::setZero()
{
	p.setZero();
}

template<typename Scalar, int N>
void Point<Scalar, N>::setOnes()
{
	p.setOnes();
}

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::normalized() const
{
	if (norm() > 0) {
		return Point<Scalar, N>(p.array() / norm());
	}
	return *this;
}

template<typename Scalar, int N>
void Point<Scalar, N>::normalize()
{
	if (norm() > 0) {
		p /= norm();
	}
}

template<typename Scalar, int N>
const Eigen::Matrix<Scalar, 1, N>& Point<Scalar, N>::eigenVector() const
{
	return p;
}

template<typename Scalar, int N>
Scalar& Point<Scalar, N>::operator()(uint i)
{
	return p(i);
}

template<typename Scalar, int N>
const Scalar& Point<Scalar, N>::operator()(uint i) const
{
	return p(i);
}

template<typename Scalar, int N>
auto Point<Scalar, N>::operator<=>(const Point& p1) const
{
	uint i = 0;
	while (p[i] == p1.p[i] && i < DIM) {
		++i;
	}
	return i == DIM ? p[0] <=> p1.p[0] : p[i] <=> p1.p[i];
}

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator+(const Scalar& s) const
{
	return Point<Scalar, N>(p.array() + s);
}

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator+(const Point<Scalar, N>& p1) const
{
	return Point<Scalar, N>(p + p1.p);
}

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator-() const
{
	return Point<Scalar, N>(-p);
}

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator-(const Scalar& s) const
{
	return Point<Scalar, N>(p.array() - s);
}

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator-(const Point<Scalar, N>& p1) const
{
	return Point<Scalar, N>(p - p1.p);
}

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator*(const Scalar& s) const
{
	return Point<Scalar, N>(p * s);
}

template<typename Scalar, int N>
Scalar Point<Scalar, N>::operator*(const Point<Scalar, N>& p1) const
{
	return dot(p1);
}

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator*(const Eigen::Matrix<Scalar, N, N>& m) const
{
	return Point<Scalar, N>(p * m);
}

/**
 * @brief Returns a new 3D point/vector on which has been applied an TRS 4x4 matrix.
 */
template<typename Scalar, int N>
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

template<typename Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator/(const Scalar& s) const
{
	return Point<Scalar, N>(p / s);
}

template<typename Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator+=(const Scalar& s)
{
	p = p.array() + s;
	return *this;
}

template<typename Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator+=(const Point<Scalar, N>& p1)
{
	p += p1.p;
	return *this;
}

template<typename Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator-=(const Scalar& s)
{
	p = p.array() - s;
	return *this;
}

template<typename Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator-=(const Point<Scalar, N>& p1)
{
	p -= p1.p;
	return *this;
}

template<typename Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator*=(const Scalar& s)
{
	p *= s;
	return *this;
}

template<typename Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator*=(const Eigen::Matrix<Scalar, N, N>& m)
{
	p *= m;
	return *this;
}

/**
 * @brief Applies to the given point/vector an TRS 4x4 matrix.
 * @param m
 */
template<typename Scalar, int N>
Point<Scalar,N>& Point<Scalar, N>::operator*=(
	const Eigen::Matrix<Scalar, N+1, N+1>& m) requires (N == 3)
{
	*this = *this * m;
	return *this;
}

template<typename Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator/=(const Scalar& s)
{
	p /= s;
	return *this;
}

template<typename Scalar, int N>
Scalar& Point<Scalar, N>::operator[](uint i)
{
	return p(i);
}

template<typename Scalar, int N>
const Scalar& Point<Scalar, N>::operator[](uint i) const
{
	return p(i);
}

template<typename Scalar, int N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p1)
{
	out << p1.p;
	return out;
}

} // namespace vcl
