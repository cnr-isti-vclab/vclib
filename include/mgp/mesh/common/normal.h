/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_NORMAL_H
#define MGP_MESH_COMMON_NORMAL_H

#include <mgp/space/point.h>

#include "info.h"

namespace mgp::common {

template<typename Scalar, int N>
class Normal : virtual public Info
{
public:
	typedef Point<Scalar, N> NormalType;

	const Point<Scalar, N>& normal() const { return n; }
	Point<Scalar, N>&       normal() { return n; }

private:
	Point<Scalar, N> n;
};

template<typename Scalar>
class Normal3 : virtual public Info
{
public:
	typedef Point3<Scalar> NormalType;

	const Point3<Scalar>& normal() const { return n; }
	Point3<Scalar>&       normal() { return n; }

private:
	Point3<Scalar> n;
};

typedef Normal3<float>  Normal3f;
typedef Normal3<double> Normal3d;

} // namespace mgp::common

#endif // MGP_MESH_COMMON_NORMAL_H
