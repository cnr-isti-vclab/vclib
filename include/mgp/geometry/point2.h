/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POINT2_H
#define MGP_POINT2_H

#include "point_t.h"

namespace mgp {

/**
 * @brief The Point3 class represents a 3-dimensional point containing 3 scalar values.
 * Specializes the Point class, providing some additional member functions useful for 3D points.
 */
template<class Scalar>
class Point2 : public Point<Scalar, 2>
{
public:
	Point2();
	Point2(const Point<Scalar, 2>& p);
	Point2(const Scalar& x, const Scalar& y);

	using Point<Scalar, 2>::x;
	using Point<Scalar, 2>::y;

	// operators
	using Point<Scalar, 2>::operator();
	using Point<Scalar, 2>::operator[];
	using Point<Scalar, 2>::operator<<;

	template<class S>
	friend std::ostream& operator<<(std::ostream& out, const Point2<S>& p);
private:
	using Point<Scalar, 2>::cross; //disable cross function for this class
};

template<class Scalar>
std::ostream& operator<<(std::ostream& out, const Point2<Scalar>& p);

} // namespace mgp

#include "point2.tpp"

#endif // MGP_POINT2_H
