/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "point4.h"

namespace mgp {

template<class ScalarType>
template<class S>
Point4<ScalarType>::Point4(const Point<S, 4>& p)
{
	*this << p(0), p(1), p(2), p(3);
}

template<class ScalarType>
Point4<ScalarType>::Point4(const ScalarType& x, const ScalarType& y, const ScalarType& z, const ScalarType& w)
{
	*this << x, y, z, w;
}

} // namespace mgp
