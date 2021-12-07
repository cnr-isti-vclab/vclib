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
template<typename S>
Point<Scalar, N>::Point(const Point<S, N>& p1)
{
	for (size_t i = 0; i < p.size(); ++i)
		p(i) = p1(i);
}

template<typename Scalar, int N>
Point<Scalar, N>::Point(const Eigen::Matrix<Scalar, 1, N>& v)
{
	p << v;
}

/**
 * @brief Returns true if at least one of its components is NaN or inf.
 * @return true if the point is degenerate, false otherwise.
 */
template<typename Scalar, int N>
bool Point<Scalar, N>::isDegenerate() const
{
	for (size_t i = 0; i < p.size(); ++i)
		if (isDegenerate(p(i)))
			return true;
	return false;
}

/**
 * @brief Returns the cross product between two points.
 * @note This function is available only on Points having size == 3.
 * @return The cross product between this and the other point.
 */
template<typename Scalar, int N>
template<int U>
VCL_ENABLE_IF(U==3, Point<Scalar VCL_COMMA N>)
Point<Scalar, N>::cross(const Point<Scalar, N>& p1) const
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
Scalar& Point<Scalar, N>::operator()(uint i)
{
	return p(i);
}

template<typename Scalar, int N>
bool Point<Scalar, N>::operator==(const Point& p1) const
{
	return p == p1.p;
}

template<typename Scalar, int N>
bool Point<Scalar, N>::operator!=(const Point& p1) const
{
	return p != p1.p;
}

template<typename Scalar, int N>
bool Point<Scalar, N>::operator<(const Point& p1) const
{
	uint i = 0;
	while (p[i] == p1.p[i] && i < DIM) {
		++i;
	}
	return i != DIM && p[i] < p1.p[i];
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
template<typename SM>
Point<Scalar, N> Point<Scalar, N>::operator*(const Eigen::Matrix<SM, N, N>& m) const
{
	return Point<Scalar, N>(p * m.template cast<Scalar>());
}

template<typename Scalar, int N>
template<typename SM, int U>
VCL_ENABLE_IF(U==3, Point<Scalar VCL_COMMA N>)
Point<Scalar, N>::operator*(const Eigen::Matrix<SM, N+1, N+1>& m) const
{
	Eigen::Matrix<SM, 3, 3> m33 = m.block(0,0,3,3);
	Eigen::Matrix<Scalar, 1, N> pp = p * m33;
	SM w = p(0)*m(3,0) + p(1)*m(3,1) +p(2)*m(3,2) + m(3,3);
	if (w != 0) pp = pp / w;
	return Point<Scalar, N>(pp);
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
template<typename SM>
Point<Scalar, N>& Point<Scalar, N>::operator*=(const Eigen::Matrix<SM, N, N>& m)
{
	p *= m.template cast<Scalar>();
	return *this;
}

template<typename Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator/=(const Scalar& s)
{
	p /= s;
	return *this;
}

template<typename Scalar, int N>
const Scalar& Point<Scalar, N>::operator()(uint i) const
{
	return p(i);
}

template<typename Scalar, int N>
template<typename S>
Scalar Point<Scalar, N>::dot(const Point<S, N>& p1) const
{
	if constexpr (std::is_same<Scalar, S>::value) {
		return p.dot(p1.p);
	}
	else {
		Point<Scalar, N> tmp(p1);
		return p.dot(tmp.p);
	}
}

template<typename Scalar, int N>
template<typename S>
Scalar Point<Scalar, N>::angle(const Point<S, N>& p1) const
{
	Scalar w = p.norm() * p1.p.norm();
	if (w == 0)
		return -1;
	Scalar t = (p.dot(p1.p)) / w;
	if (t > 1)
		t = 1;
	else if (t < -1)
		t = -1;
	return (Scalar) acos(t);
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
