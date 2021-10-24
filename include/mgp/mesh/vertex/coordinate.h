/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_COORDINATE_H
#define MGP_MESH_VERTEX_COORDINATE_H

#include <mgp/space/point.h>

#include "common.h"

namespace mgp::vert {

template<class Scalar, int N>
class Coordinate : virtual public Info
{
public:
	typedef Point<Scalar, N> CoordinateType;

	const Point<Scalar, N>& coordinate() const { return p; }
	Point<Scalar, N>&       coordinate() { return p; }

private:
	Point<Scalar, N> p;
};

template<class Scalar>
class Coordinate3 : virtual public Info
{
public:
	typedef Point3<Scalar> CoordinateType;

	const Point3<Scalar>& coordinate() const { return p; }
	Point3<Scalar>&       coordinate() { return p; }

private:
	Point3<Scalar> p;
};

typedef Coordinate3<float>  Coordinate3f;
typedef Coordinate3<double> Coordinate3d;

} // namespace mgp::vert

#endif // MGP_MESH_VERTEX_COORDINATE_H
