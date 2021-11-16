/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point4.h"

namespace mgp {

template<typename ScalarType>
template<typename S>
Point4<ScalarType>::Point4(const Point<S, 4>& p)
{
	Point<ScalarType, 4>::p << p(0), p(1), p(2), p(3);
}

template<typename ScalarType>
Point4<ScalarType>::Point4(const ScalarType& x, const ScalarType& y, const ScalarType& z, const ScalarType& w)
{
	Point<ScalarType, 4>::p << x, y, z, w;
}

template<typename ScalarType>
ScalarType Point4<ScalarType>::x() const
{
	return Point<ScalarType, 4>::p.x();
}

template<typename ScalarType>
ScalarType Point4<ScalarType>::y() const
{
	return Point<ScalarType, 4>::p.y();
}

template<typename ScalarType>
ScalarType Point4<ScalarType>::z() const
{
	return Point<ScalarType, 4>::p.z();
}

template<typename ScalarType>
ScalarType Point4<ScalarType>::w() const
{
	return Point<ScalarType, 4>::p.w();
}

template<typename ScalarType>
ScalarType& Point4<ScalarType>::x()
{
	return Point<ScalarType, 4>::p.x();
}

template<typename ScalarType>
ScalarType& Point4<ScalarType>::y()
{
	return Point<ScalarType, 4>::p.y();
}

template<typename ScalarType>
ScalarType& Point4<ScalarType>::z()
{
	return Point<ScalarType, 4>::p.z();
}

template<typename ScalarType>
ScalarType& Point4<ScalarType>::w()
{
	return Point<ScalarType, 4>::p.w();
}

} // namespace mgp
