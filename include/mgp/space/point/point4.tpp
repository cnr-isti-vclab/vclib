/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point4.h"

namespace mgp {

template<class Scalar>
template<class S>
Point4<Scalar>::Point4(const Point<S, 4>& p)
{
	*this << p(0), p(1), p(2), p(3);
}

template<class Scalar>
Point4<Scalar>::Point4(const Scalar& x, const Scalar& y, const Scalar& z, const Scalar& w)
{
	*this << x, y, z, w;
}

template<class Scalar>
std::ostream& operator<<(std::ostream& out, const Point4<Scalar>& p)
{
	out << static_cast<const Point<Scalar, 4>&>(p);
	return out;
}

} // namespace mgp
