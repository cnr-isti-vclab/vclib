/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point2.h"

namespace mgp {

template<class Scalar>
template<class S>
Point2<Scalar>::Point2(const Point<S, 2>& p)
{
	*this << p(0), p(1);
}

template<class Scalar>
Point2<Scalar>::Point2(const Scalar& x, const Scalar& y)
{
	*this << x, y;
}

template<class Scalar>
std::ostream& operator<<(std::ostream& out, const Point2<Scalar>& p)
{
	out << static_cast<const Point<Scalar, 2>&>(p);
	return out;
}

} // namespace mgp
