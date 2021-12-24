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

#include "plane.h"

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
template<typename S>
Plane<Scalar, NORM>::Plane(const Point3<S>& direction, S offset) :
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
template<typename S>
Plane<Scalar, NORM>::Plane(const Point3<S>& p0, const Point3<S>& normal)
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
template<typename S>
Plane<Scalar, NORM>::Plane(const Point3<S>& p0, const Point3<S>& p1, const Point3<S>& p2) :
		Plane<Scalar, NORM>(p0, (p2 - p0).cross(p1 - p0))
{
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
template<typename S>
Point3<S> Plane<Scalar, NORM>::projectPoint(const Point3<S>& p) const
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
template<typename S>
Point3<S> Plane<Scalar, NORM>::mirrorPoint(const Point3<S>& p) const
{
	Point3<S> mirr=projectPoint(p);
	mirr += mirr-p;
	return mirr;
}

/**
 * @brief Given a point, returns the distance between the point and this plane.
 * @param p
 * @return
 */
template<typename Scalar, bool NORM>
template<typename S>
Scalar Plane<Scalar, NORM>::dist(const Point3<S>& p) const
{
	return dir.dot(p) - off;
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
