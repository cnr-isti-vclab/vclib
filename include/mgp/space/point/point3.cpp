/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point3.h"

namespace mgp {

template<class ScalarType>
template<class S>
Point3<ScalarType>::Point3(const Point<S, 3>& p1)
{
	Point<ScalarType, 3>::p << p1(0), p1(1), p1(2);
}

template<class ScalarType>
Point3<ScalarType>::Point3(const ScalarType& x, const ScalarType& y, const ScalarType& z)
{
	Point<ScalarType, 3>::p << x, y, z;
}

template<class ScalarType>
ScalarType Point3<ScalarType>::x() const
{
	return Point<ScalarType, 3>::p.x();
}

template<class ScalarType>
ScalarType Point3<ScalarType>::y() const
{
	return Point<ScalarType, 3>::p.y();
}

template<class ScalarType>
ScalarType Point3<ScalarType>::z() const
{
	return Point<ScalarType, 3>::p.z();
}

template<class ScalarType>
ScalarType& Point3<ScalarType>::x()
{
	return Point<ScalarType, 3>::p.x();
}

template<class ScalarType>
ScalarType& Point3<ScalarType>::y()
{
	return Point<ScalarType, 3>::p.y();
}

template<class ScalarType>
ScalarType& Point3<ScalarType>::z()
{
	return Point<ScalarType, 3>::p.z();
}

template<class ScalarType>
Point3<ScalarType> Point3<ScalarType>::cross(const Point3<ScalarType>& p1) const
{
	return Point<ScalarType, 3>::cross(p1.p);
}

} // namespace mgp
