/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POINT4_H
#define MGP_POINT4_H

#include "point_t.h"

namespace mgp {

/**
 * @brief The Point4 class represents a 4-dimensional point containing 4 scalar values.
 * Specializes the Point class, providing some additional member functions useful for 4D points.
 */
template<class Scalar>
class Point4 : public Point<Scalar, 4>
{
public:
	Point4()                = default; // default empty constructor
	Point4(const Point4& p) = default; // default copy constructor

	template<class S>
	Point4(const Point<S, 4>& p); // constructor from base class - will include all its constructors
	Point4(const Scalar& x, const Scalar& y, const Scalar& z, const Scalar& w);

	using Point<Scalar, 4>::x;
	using Point<Scalar, 4>::y;
	using Point<Scalar, 4>::z;
	using Point<Scalar, 4>::w;

	// operators
	using Point<Scalar, 4>::operator();
	using Point<Scalar, 4>::operator[];
	using Point<Scalar, 4>::operator<<;

	template<class S>
	friend std::ostream& operator<<(std::ostream& out, const Point4<S>& p);
};

typedef Point4<int>    Point4i;
typedef Point4<float>  Point4f;
typedef Point4<double> Point4d;

template<class Scalar>
std::ostream& operator<<(std::ostream& out, const Point4<Scalar>& p);

} // namespace mgp

#include "point4.tpp"

#endif // MGP_POINT4_H
