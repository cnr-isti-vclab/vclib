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
	Point<ScalarType, 2>::p << p(0), p(1);
}

template<class ScalarType>
Point2<ScalarType>::Point2(const ScalarType& x, const ScalarType& y)
{
	Point<ScalarType, 2>::p << x, y;
}

template<class ScalarType>
ScalarType Point2<ScalarType>::x() const
{
	return Point<ScalarType, 2>::p.x();
}

template<class ScalarType>
ScalarType Point2<ScalarType>::y() const
{
	return Point<ScalarType, 2>::p.y();
}

template<class ScalarType>
ScalarType& Point2<ScalarType>::x()
{
	return Point<ScalarType, 2>::p.x();
}

template<class ScalarType>
ScalarType& Point2<ScalarType>::y()
{
	return Point<ScalarType, 2>::p.y();
}

} // namespace mgp
