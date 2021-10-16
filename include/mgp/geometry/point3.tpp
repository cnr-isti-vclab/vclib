/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point3.h"

namespace mgp {

template<class Scalar>
Point3<Scalar>::Point3()
{
}

template<class Scalar>
Point3<Scalar>::Point3(const Scalar& x, const Scalar& y, const Scalar& z)
{
	*this << x, y, z;
}

template<class Scalar>
std::ostream& operator<<(std::ostream& out, const Point3<Scalar>& p)
{
	out << static_cast<const Point<Scalar, 3>&>(p);
	return out;
}

} // namespace mgp
