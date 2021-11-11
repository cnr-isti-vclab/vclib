/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_COORDINATE_H
#define MGP_MESH_COMPONENTS_COORDINATE_H

#include <mgp/space/point.h>

namespace mgp::comp {

class CoordinateTriggerer {};

template<class Scalar, int N>
class Coordinate : public CoordinateTriggerer
{
public:
	using CoordinateType =  Point<Scalar, N>;

	const Point<Scalar, N>& coordinate() const { return p; }
	Point<Scalar, N>&       coordinate() { return p; }

private:
	Point<Scalar, N> p;
};

template<class Scalar>
class Coordinate3 : public CoordinateTriggerer
{
public:
	using CoordinateType = Point3<Scalar>;

	const Point3<Scalar>& coordinate() const { return p; }
	Point3<Scalar>&       coordinate() { return p; }

private:
	Point3<Scalar> p;
};

using Coordinate3f = Coordinate3<float>;
using Coordinate3d = Coordinate3<double>;

/**
 * Detector to check if a class has (inherits) Coordinate
 */

template <typename  T>
using hasCoordinateT = std::is_base_of<CoordinateTriggerer, T>;

template <typename  T>
bool constexpr hasCoordinate() {return hasCoordinateT<T>::value;}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_COORDINATE_H
