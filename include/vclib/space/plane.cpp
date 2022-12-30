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

#include "plane.h"

#include <vclib/exception/misc_exception.h>

namespace vcl {

/**
 * @brief Empty constructor. The plane is uninitialized.
 */
template<typename Scalar, bool NORM>
Plane<Scalar, NORM>::Plane()
{
}

/**
 * @brief Constructor of a plane given a direction and an offset.
 * @param direction
 * @param offset
 */
template<typename Scalar, bool NORM>
Plane<Scalar, NORM>::Plane(const Point3<Scalar>& direction, Scalar offset) :
		dir(direction), off(offset)
{
	if constexpr(NORM) {
		Scalar n = dir.norm();
		dir /= n;
		off /= n;
	}
}

/**
 * @brief Constructor of a plane given a point lying to the plane and the normal of the plane.
 * @param p0
 * @param normal
 */
template<typename Scalar, bool NORM>
Plane<Scalar, NORM>::Plane(const Point3<Scalar>& p0, const Point3<Scalar>& normal)
{
	dir = normal;
	if constexpr(NORM) {
		dir.normalize();
	}
	off = p0.dot(dir);
}

/**
 * @brief Constructor of a plane given three points.
 * @param p0
 * @param p1
 * @param p2
 */
template<typename Scalar, bool NORM>
Plane<Scalar, NORM>::Plane(
	const Point3<Scalar>& p0,
	const Point3<Scalar>& p1,
	const Point3<Scalar>& p2) :
		Plane<Scalar, NORM>(p0, (p2 - p0).cross(p1 - p0))
{
}

template<typename Scalar, bool NORM>
template<typename S>
Plane<S, NORM> Plane<Scalar, NORM>::cast() const
{
	if constexpr (std::is_same<Scalar, S>::value) {
		return *this;
	}
	else {
		return Plane<S, NORM>(dir.template cast<S>(), off);
	}
}

/**
 * @brief Returns the direction component of the plane.
 * @return
 */
template<typename Scalar, bool NORM>
const Point3<Scalar>& Plane<Scalar, NORM>::direction() const
{
	return dir;
}

/**
 * @brief Returns the offset component of the plane.
 * @return
 */
template<typename Scalar, bool NORM>
Scalar Plane<Scalar, NORM>::offset() const
{
	return off;
}

/**
 * @brief Given a point, returns the point projected to this plane.
 * @param p
 * @return
 */
template<typename Scalar, bool NORM>
Point3<Scalar> Plane<Scalar, NORM>::projectPoint(const Point3<Scalar>& p) const
{
	Scalar k = p.dot(dir) - off;
	return p - dir * k;
}

/**
 * @brief Given a point, returns the point mirrored w.r.t. this plane.
 * @param p
 * @return
 */
template<typename Scalar, bool NORM>
Point3<Scalar> Plane<Scalar, NORM>::mirrorPoint(const Point3<Scalar>& p) const
{
	Point3<Scalar> mirr=projectPoint(p);
	mirr += mirr-p;
	return mirr;
}

/**
 * @brief Given a point, returns the *signed* distance between the point and this plane.
 * @param p
 * @return
 */
template<typename Scalar, bool NORM>
Scalar Plane<Scalar, NORM>::dist(const Point3<Scalar>& p) const
{
	return dir.dot(p) - off;
}

/**
 * @brief Computes the intersectin between this plane and a segment. Returns `true` if the
 * intersection exists, false otherwise.
 *
 * @param[out] intersection: the resulting intersection between this plane and `s`.
 * @param[in] s: the segment on which compute the intersection.
 * @return `true` if the intersection exists, false otherwise.
 */
template<typename Scalar, bool NORM>
bool Plane<Scalar, NORM>::segmentIntersection(
	Point3<Scalar>&                                  intersection,
	const std::pair<Point3<Scalar>, Point3<Scalar>>& s) const
{
	ScalarType p1_proj = s.second * dir - off;
	ScalarType p0_proj = s.first * dir - off;
	if ( (p1_proj>0)-(p0_proj<0))
		return false;

	if(p0_proj == p1_proj) return false;

	   // check that we perform the computation in a way that is independent with v0 v1 swaps
	if(p0_proj < p1_proj)
		intersection =  s.first + (s.second - s.first) * fabs(p0_proj/(p1_proj-p0_proj));
	if(p0_proj > p1_proj)
		intersection =  s.second + (s.first - s.second) * fabs(p1_proj/(p0_proj-p1_proj));

	return true;
}

/**
 * @brief Computes and returns the intersection between this plane and a segment.
 * If the segment does not exist, a vcl::NoIntersectionException will be thrown.
 *
 * @param[in] s: the segment on which compute the intersection.
 * @return The point intersection between this plane and `s`.
 */
template<typename Scalar, bool NORM>
Point3<Scalar> Plane<Scalar, NORM>::segmentIntersection(
	const std::pair<Point3<Scalar>, Point3<Scalar>>& s) const
{
	Point3<Scalar> res;
	bool b = segmentIntersection(res, s);
	if (!b)
		throw vcl::NoIntersectionException("Plane and Segment");
	return res;
}

template<typename Scalar, bool NORM>
bool Plane<Scalar, NORM>::operator==(const Plane& p) const
{
	return off == p.off && dir == p.dir;
}

template<typename Scalar, bool NORM>
bool Plane<Scalar, NORM>::operator!=(const Plane& p) const
{
	return !(*this == p);
}

} // namespace vcl
