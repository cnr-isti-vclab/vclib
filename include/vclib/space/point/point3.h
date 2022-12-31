/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_SPACE_POINT_POINT3_H
#define VCL_SPACE_POINT_POINT3_H

#include "point_t.h"

namespace vcl {

/**
 * @brief The Point3 class represents a 3-dimensional point containing 3 ScalarType values.
 * Specializes the Point class, providing some additional member functions useful for 3D points.
 */
template<typename ScalarType>
class Point3 : public Point<ScalarType, 3>
{
public:
	using Point<ScalarType, 3>::Point;

	Point3() = default; // default empty constructor

	Point3(const Point<ScalarType, 3>& p1); // from base class - will include all its constructors
	Point3(const ScalarType& x, const ScalarType& y, const ScalarType& z);

	ScalarType x() const;
	ScalarType y() const;
	ScalarType z() const;
	ScalarType& x();
	ScalarType& y();
	ScalarType& z();
};

using Point3i = Point3<int>;
using Point3f = Point3<float>;
using Point3d = Point3<double>;

// makes sure that Point3 classes satisfy PointConcept
static_assert(PointConcept<Point3i>, "Point3i is not a valid Point");
static_assert(PointConcept<Point3f>, "Point3f is not a valid Point");
static_assert(PointConcept<Point3d>, "Point3d is not a valid Point");

} // namespace vcl

// inject vcl::Point3 hash function in std namespace
namespace std {
template <typename Scalar>
struct hash<vcl::Point3<Scalar> >
{
	size_t operator()(const vcl::Point3<Scalar>& id) const noexcept;
};
} // namespace std

#include "point3.cpp"

#endif // VCL_SPACE_POINT_POINT3_H
