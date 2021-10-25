/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POINT3_H
#define MGP_POINT3_H

#include "point_t.h"

namespace mgp {

/**
 * @brief The Point3 class represents a 3-dimensional point containing 3 ScalarType values.
 * Specializes the Point class, providing some additional member functions useful for 3D points.
 */
template<class ScalarType>
class Point3 : public Point<ScalarType, 3>
{
public:
	Point3()                = default; // default empty constructor
	Point3(const Point3& p) = default; // default copy constructor

	template<class S>
	Point3(const Point<S, 3>& p); // constructor from base class - will include all its constructors
	Point3(const ScalarType& x, const ScalarType& y, const ScalarType& z);

	using Point<ScalarType, 3>::x;
	using Point<ScalarType, 3>::y;
	using Point<ScalarType, 3>::z;
	using Point<ScalarType, 3>::cross;
};

using Point3i = Point3<int>;
using Point3f = Point3<float>;
using Point3d = Point3<double>;

} // namespace mgp

#include "point3.cpp"

#endif // MGP_POINT3_H
