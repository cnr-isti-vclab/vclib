/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_POSITION_H
#define MGP_MESH_VERTEX_POSITION_H

#include <mgp/space/point.h>

namespace mgp {
namespace vert {

template<class Scalar, int N>
class Position
{
public:
	typedef Point<Scalar, N> PositionType;

	const Point<Scalar, N>& position() const { return p; }
	Point<Scalar, N>&       position() { return p; }

private:
	Point<Scalar, N> p;
};

template<class Scalar>
class Position3
{
public:
	typedef Point3<Scalar> PositionType;

	const Point3<Scalar>& position() const { return p; }
	Point3<Scalar>&       position() { return p; }

private:
	Point3<Scalar> p;
};

typedef Position3<float>  Position3f;
typedef Position3<double> Position3d;

} // namespace vert
} // namespace mgp

#endif // MGP_MESH_VERTEX_POSITION_H
