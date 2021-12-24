/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_PLANE_H
#define VCL_PLANE_H

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
	template<typename S>
	Plane(const Point3<S>& direction, S offset);
	template<typename S>
	Plane(const Point3<S>& p0, const Point3<S>& normal);
	template<typename S>
	Plane(const Point3<S>& p0, const Point3<S>& p1, const Point3<S>& p2);

	const Point3<Scalar>& direction() const;
	Scalar offset() const;

	template<typename S>
	Point3<S> projectPoint(const Point3<S>& p) const;

	template<typename S>
	Point3<S> mirrorPoint(const Point3<S>& p) const;

	template<typename S>
	Scalar dist(const Point3<S>& p) const;

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

#endif // VCL_PLANE_H
