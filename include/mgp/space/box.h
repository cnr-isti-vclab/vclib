/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_BOX_H
#define MGP_BOX_H

#include "point.h"

namespace mgp {

template<class Scalar, int N>
class Box
{
public:
	Box();

	void setNull();

	Point<Scalar, N> min;
	Point<Scalar, N> max;
};

typedef Box<int, 2>    Box2i;
typedef Box<float, 2>  Box2f;
typedef Box<double, 2> Box2d;
typedef Box<int, 3>    Box3i;
typedef Box<float, 3>  Box3f;
typedef Box<double, 3> Box3d;

} // namespace mgp

#include "box.cpp"

#endif // MGP_BOX_H
