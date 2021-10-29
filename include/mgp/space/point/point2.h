/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POINT2_H
#define MGP_POINT2_H

#include "point_t.h"

namespace mgp {

/**
 * @brief The Point2 class represents a 2-dimensional point containing 2 scalar values.
 * Specializes the Point class, providing some additional member functions useful for 2D points.
 */
template<class ScalarType>
class Point2 : public Point<ScalarType, 2>
{
public:
	Point2()                = default; // default empty constructor
	Point2(const Point2& p) = default; // default copy constructor

	template<class S>
	Point2(const Point<S, 2>& p); // constructor from base class - will include all its constructors
	Point2(const ScalarType& x, const ScalarType& y);

	ScalarType x() const;
	ScalarType y() const;
	ScalarType& x();
	ScalarType& y();
};

using Point2i = Point2<int>;
using Point2f = Point2<float>;
using Point2d = Point2<double>;

} // namespace mgp

#include "point2.cpp"

#endif // MGP_POINT2_H
