/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POINT3_H
#define MGP_POINT3_H

#include "point_t.h"

namespace mgp {

/**
 * @brief The Point3 class represents a 3-dimensional point containing 3 scalar values.
 * Specializes the Point class, providing some additional member functions useful for 3D points.
 */
template<class Scalar>
class Point3 : public Point<Scalar, 3>
{
public:
	Point3()                = default; // default empty constructor
	Point3(const Point3& p) = default; // default copy constructor

	template<class S>
	Point3(const Point<S, 3>& p); // constructor from base class - will include all its constructors
	Point3(const Scalar& x, const Scalar& y, const Scalar& z);

	using Point<Scalar, 3>::x;
	using Point<Scalar, 3>::y;
	using Point<Scalar, 3>::z;
	using Point<Scalar, 3>::cross;

	// operators
	using Point<Scalar, 3>::operator();
	using Point<Scalar, 3>::operator[];
	using Point<Scalar, 3>::operator<<;

	template<class S>
	friend std::ostream& operator<<(std::ostream& out, const Point3<S>& p);
};

using Point3i = Point3<int>;
using Point3f = Point3<float>;
using Point3d = Point3<double>;

template<class Scalar>
std::ostream& operator<<(std::ostream& out, const Point3<Scalar>& p);

} // namespace mgp

#include "point3.cpp"

#endif // MGP_POINT3_H
