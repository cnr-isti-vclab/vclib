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
	const Point<Scalar, N>& position() const { return p; }
	Point<Scalar, N>&       position() { return p; }

private:
	Point<Scalar, N> p;
};

typedef Position<float, 3>  Position3f;
typedef Position<double, 3> Position3d;

} // namespace vert
} // namespace mgp

#endif // MGP_MESH_VERTEX_POSITION_H
