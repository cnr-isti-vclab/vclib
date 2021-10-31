/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_NORMAL_H
#define MGP_MESH_COMPONENTS_NORMAL_H

#include <mgp/space/point.h>

namespace mgp::components {

class NormalTrigger
{
};

template<typename Scalar, int N>
class Normal : public NormalTrigger
{
public:
	using NormalType = Point<Scalar, N>;

	const Point<Scalar, N>& normal() const { return n; }
	Point<Scalar, N>&       normal() { return n; }

private:
	Point<Scalar, N> n;
};

template<typename Scalar>
class Normal3 : public NormalTrigger
{
public:
	using NormalType = Point3<Scalar>;

	const Point3<Scalar>& normal() const { return n; }
	Point3<Scalar>&       normal() { return n; }

private:
	Point3<Scalar> n;
};

using Normal3f = Normal3<float>;
using Normal3d = Normal3<double>;

/**
 * Detector to check if a class has (inherits) Normal
 */

template<typename T>
using hasNormalT = std::is_base_of<NormalTrigger, T>;

template<typename T>
bool constexpr hasNormal()
{
	return hasNormalT<T>::value;
}

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_NORMAL_H
