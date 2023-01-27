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

#ifndef VCL_SPACE_PLANE_H
#define VCL_SPACE_PLANE_H

#include "point/point3.h"

namespace vcl {

/**
 * @brief The Plane class represent a 2D plane in 3D space.
 *
 * This is the class for infinite planes in 3D space. A Plane is stored just as a Point3 and a
 * scalar:
 * - a direction (not necessarily normalized),
 * - an offset from the origin
 *
 * Just to be clear, given a point p on a plane it always holds:
 *    plane.direction().dot(p) == plane.offset()
 *
 * A vcl::Plane, once initialized, cannot be changed.
 */
template<typename Scalar, bool NORM=true>
class Plane
{
public:
	using ScalarType = Scalar;

	Plane();
	Plane(const Point3<Scalar>& direction, Scalar offset);
	Plane(const Point3<Scalar>& p0, const Point3<Scalar>& normal);
	Plane(const Point3<Scalar>& p0, const Point3<Scalar>& p1, const Point3<Scalar>& p2);

	template<typename S>
	Plane<S, NORM> cast() const;

	const Point3<Scalar>& direction() const;
	Scalar offset() const;

	Point3<Scalar> projectPoint(const Point3<Scalar>& p) const;
	Point3<Scalar> mirrorPoint(const Point3<Scalar>& p) const;
	Scalar dist(const Point3<Scalar>& p) const;

	bool segmentIntersection(
		Point3<Scalar>& intersection,
		const std::pair<Point3<Scalar>, Point3<Scalar>>& s) const;

	Point3<Scalar> segmentIntersection(const std::pair<Point3<Scalar>, Point3<Scalar>>& s) const;

	bool operator==(const Plane& p) const;
	bool operator!=(const Plane& p) const;

private:
	Point3<Scalar> dir;
	Scalar off;
};

using Planef = Plane<float>;
using Planed = Plane<double>;

} // namespace vcl

#include "plane.cpp"

#endif // VCL_SPACE_PLANE_H
