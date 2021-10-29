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
	for (size_t i = 0; i < size(); ++i)
		operator()(i) = p(i);
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
template<class S>
Scalar Point<Scalar, N>::dot(const Point<S, N>& p1) const
{
	if constexpr (std::is_same<Scalar, S>::value) {
		return Eigen::Matrix<Scalar, 1, N>::dot(p1);
	}
	else {
		Point<Scalar, N> tmp(p1);
		return Eigen::Matrix<Scalar, 1, N>::dot(tmp);
	}
}

template<class Scalar, int N>
template<class S>
Scalar Point<Scalar, N>::angle(const Point<S, N>& p1) const
{
	Scalar w = norm()*p1.norm();
	if (w == 0) return -1;
	Scalar t = ((*this)*p1)/w;
	if (t > 1) t = 1;
	else if ( t < -1) t = -1;
	return (Scalar) acos(t);
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
