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
template<typename ScalarType>
class Point3 : public Point<ScalarType, 3>
{
public:
	Point3()                = default; // default empty constructor
	Point3(const Point3& p) = default; // default copy constructor

	template<typename S>
	Point3(const Point<S, 3>& p1); // constructor from base class - will include all its constructors
	Point3(const ScalarType& x, const ScalarType& y, const ScalarType& z);

	ScalarType x() const;
	ScalarType y() const;
	ScalarType z() const;
	ScalarType& x();
	ScalarType& y();
	ScalarType& z();

	Point3<ScalarType> cross(const Point3<ScalarType>& p1) const;
	//using Point<ScalarType, 3>::cross;
};

using Point3i = Point3<int>;
using Point3f = Point3<float>;
using Point3d = Point3<double>;

} // namespace mgp

#include "point3.cpp"

#endif // MGP_POINT3_H
