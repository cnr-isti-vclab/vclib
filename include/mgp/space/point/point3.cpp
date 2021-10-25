/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point3.h"

namespace mgp {

template<class ScalarType>
template<class S>
Point3<ScalarType>::Point3(const Point<S, 3>& p)
{
	*this << p(0), p(1), p(2);
}

template<class ScalarType>
Point3<ScalarType>::Point3(const ScalarType& x, const ScalarType& y, const ScalarType& z)
{
	*this << x, y, z;
}

template<class ScalarType>
std::ostream& operator<<(std::ostream& out, const Point3<ScalarType>& p)
{
	out << static_cast<const Point<ScalarType, 3>&>(p);
	return out;
}

} // namespace mgp
