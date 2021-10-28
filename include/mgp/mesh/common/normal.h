/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_NORMAL_H
#define MGP_MESH_COMMON_NORMAL_H

#include <mgp/space/point.h>

#include "info.h"

namespace mgp::common {

class NormalTrigger {};

template<typename Scalar, int N>
class Normal : public NormalTrigger, virtual public Info
{
public:
	typedef Point<Scalar, N> NormalType;

	const Point<Scalar, N>& normal() const { return n; }
	Point<Scalar, N>&       normal() { return n; }

private:
	Point<Scalar, N> n;
};

template<typename Scalar>
class Normal3 : public NormalTrigger, virtual public Info
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

/**
 * Detector to check if a class has (inherits) Normal
 */

template <typename  T>
using hasNormalT = std::is_base_of<NormalTrigger, T>;

template <typename  T>
bool constexpr hasNormal() {return hasNormalT<T>::value;}

} // namespace mgp::common

#endif // MGP_MESH_COMMON_NORMAL_H
