/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_POINT4_H
#define MGP_POINT4_H

#include "point_t.h"

namespace mgp {

/**
 * @brief The Point4 class represents a 4-dimensional point containing 4 ScalarType values.
 * Specializes the Point class, providing some additional member functions useful for 4D points.
 */
template<class ScalarType>
class Point4 : public Point<ScalarType, 4>
{
public:
	Point4()                = default; // default empty constructor
	Point4(const Point4& p) = default; // default copy constructor

	template<class S>
	Point4(const Point<S, 4>& p); // constructor from base class - will include all its constructors
	Point4(const ScalarType& x, const ScalarType& y, const ScalarType& z, const ScalarType& w);

	using Point<ScalarType, 4>::x;
	using Point<ScalarType, 4>::y;
	using Point<ScalarType, 4>::z;
	using Point<ScalarType, 4>::w;

	// operators
	using Point<ScalarType, 4>::operator();
	using Point<ScalarType, 4>::operator[];
	using Point<ScalarType, 4>::operator<<;

	template<class S>
	friend std::ostream& operator<<(std::ostream& out, const Point4<S>& p);
};

using Point4i = Point4<int>;
using Point4f = Point4<float>;
using Point4d = Point4<double>;

template<class ScalarType>
std::ostream& operator<<(std::ostream& out, const Point4<ScalarType>& p);

} // namespace mgp

#include "point4.cpp"

#endif // MGP_POINT4_H
