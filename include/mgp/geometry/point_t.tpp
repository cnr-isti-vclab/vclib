/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point_t.h"

namespace mgp {

template<class Scalar, int N>
template<class S>
Point<Scalar, N>::Point(const Point<S, N>& p)
{
	*this << p;
}

template<class Scalar, int N>
Point<Scalar, N>::Point(const Eigen::Matrix<Scalar, 1, N>& v)
{
	*this << v;
}

template<class Scalar, int N>
Point<Scalar, N>::Point(const Eigen::Matrix<Scalar, N, 1>& v)
{
	*this << v;
}

template<class Scalar, int N>
Scalar Point<Scalar, N>::dot(const Point<Scalar, N>& p1) const
{
	return Eigen::Matrix<Scalar, 1, N>::dot(p1);
}

template<class Scalar, int N>
Scalar Point<Scalar, N>::operator[](size_t i) const
{
	return operator()(i);
}

template<class Scalar, int N>
Scalar& Point<Scalar, N>::operator[](size_t i)
{
	return operator()(i);
}

template<class Scalar, int N>
Point<Scalar, N> Point<Scalar, N>::cross(const Point<Scalar, N>& p1) const
{
	return Eigen::Matrix<Scalar, 1, N>::cross(p1);
}

template<class Scalar, int N>
std::ostream& operator<<(std::ostream& out, const Point<Scalar, N>& p)
{
	out << static_cast<const Eigen::Matrix<Scalar, 1, N>&>(p);
	return out;
}

} // namespace mgp
