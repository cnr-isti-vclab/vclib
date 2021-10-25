/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point2.h"

namespace mgp {

template<class ScalarType>
template<class S>
Point2<ScalarType>::Point2(const Point<S, 2>& p)
{
	*this << p(0), p(1);
}

template<class ScalarType>
Point2<ScalarType>::Point2(const ScalarType& x, const ScalarType& y)
{
	*this << x, y;
}

template<class ScalarType>
std::ostream& operator<<(std::ostream& out, const Point2<ScalarType>& p)
{
	out << static_cast<const Point<ScalarType, 2>&>(p);
	return out;
}

} // namespace mgp
