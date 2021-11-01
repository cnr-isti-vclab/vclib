/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point_t.h"

namespace mgp {

template<class Scalar, int N>
template<class S>
Point<Scalar, N>::Point(const Point<S, N>& p1)
{
	for (size_t i = 0; i < p.size(); ++i)
		p(i) = p1(i);
}

template<class Scalar, int N>
Point<Scalar, N>::Point(const Eigen::Matrix<Scalar, 1, N>& v)
{
	p << v;
}

template<class Scalar, int N>
Scalar Point<Scalar, N>::norm() const
{
	return p.norm();
}

template<class Scalar, int N>
Scalar Point<Scalar, N>::squaredNorm() const
{
	return p.squaredNorm();
}

template<class Scalar, int N>
unsigned int Point<Scalar, N>::size() const
{
	return p.size();
}

template<class Scalar, int N>
void Point<Scalar, N>::setConstant(Scalar s)
{
	p.setConstant(s);
}

template<class Scalar, int N>
void Point<Scalar, N>::setZero()
{
	p.setZero();
}

template<class Scalar, int N>
void Point<Scalar, N>::setOnes()
{
	p.setOnes();
}

template<class Scalar, int N>
Scalar& Point<Scalar, N>::operator()(unsigned int i)
{
	return p(i);
}

template<class Scalar, int N>
bool Point<Scalar, N>::operator==(const Point& p1) const
{
	return p == p1.p;
}

template<class Scalar, int N>
bool Point<Scalar, N>::operator!=(const Point& p1) const
{
	return p != p1.p;
}

template<class Scalar, int N>
bool Point<Scalar, N>::operator<(const Point& p1) const
{
	unsigned int i = 0;
	while (p[i] == p1.p[i] && i < DIM) {
		++i;
	}
	return i != DIM && p[i] < p1.p[i];
}

template<class Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator+(const Scalar& s) const
{
	return Point<Scalar, N>(p + s);
}

template<class Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator+(const Point<Scalar, N>& p1) const
{
	return Point<Scalar, N>(p + p1.p);
}

template<class Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator-(const Scalar& s) const
{
	return Point<Scalar, N>(p - s);
}

template<class Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator-(const Point<Scalar, N>& p1) const
{
	return Point<Scalar, N>(p - p1.p);
}

template<class Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator*(const Scalar& s) const
{
	return Point<Scalar, N>(p * s);
}

template<class Scalar, int N>
Scalar Point<Scalar, N>::operator*(const Point<Scalar, N>& p1) const
{
	return dot(p1);
}

template<class Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::operator/(const Scalar& s) const
{
	return Point<Scalar, N>(p / s);
}

template<class Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator+=(const Point<Scalar, N>& p1)
{
	p += p1.p;
	return *this;
}

template<class Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator-=(const Point<Scalar, N>& p1)
{
	p -= p1.p;
	return *this;
}

template<class Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator*=(const Scalar& s)
{
	p *= s;
	return *this;
}

template<class Scalar, int N>
Point<Scalar, N>& Point<Scalar, N>::operator/=(const Scalar& s)
{
	p /= s;
	return *this;
}

template<class Scalar, int N>
const Scalar& Point<Scalar, N>::operator()(unsigned int i) const
{
	return p(i);
}

template<class Scalar, int N>
template<class S>
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

template<class Scalar, int N>
template<class S>
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

template<class Scalar, int N>
Scalar Point<Scalar, N>::operator[](size_t i) const
{
	return p(i);
}

template<class Scalar, int N>
Scalar& Point<Scalar, N>::operator[](size_t i)
{
	return p(i);
}

template<class Scalar, int N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p1)
{
	out << p1.p;
	return out;
}

} // namespace mgp
