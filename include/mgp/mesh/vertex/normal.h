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
	typedef Point<Scalar, N> NormalType;

	const Point<Scalar, N>& normal() const { return p; }
	Point<Scalar, N>&       normal() { return p; }

private:
	Point<Scalar, N> p;
};

template<class Scalar>
class Normal3
{
public:
	typedef Point3<Scalar> NormalType;

	const Point3<Scalar>& normal() const { return p; }
	Point3<Scalar>&       normal() { return p; }

private:
	Point3<Scalar> p;
};

typedef Normal3<float>  Normal3f;
typedef Normal3<double> Normal3d;

} // namespace vert
} // namespace mgp

#endif // MGP_MESH_VERTEX_NORMAL_H
