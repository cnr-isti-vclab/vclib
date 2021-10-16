/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_NORMAL_H
#define MGP_MESH_VERTEX_NORMAL_H

#include <mgp/space/point.h>

namespace mgp {
namespace vert {

template<class Scalar, int N>
class Normal
{
public:
	const Point<Scalar, N>& normal() const { return p; }
	Point<Scalar, N>&       normal() { return p; }

private:
	Point<Scalar, N> p;
};

typedef Normal<float, 3>  Normal3f;
typedef Normal<double, 3> Normal3d;

} // namespace vert
} // namespace mgp

#endif // MGP_MESH_VERTEX_NORMAL_H
