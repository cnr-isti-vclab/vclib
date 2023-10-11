/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#include <vclib/concepts/space/plane.h>
#include <vclib/exceptions/misc_exceptions.h>

#include "point.h"

namespace vcl {

/**
 * @brief The Plane class represent a 2D plane in 3D space.
 *
 * This is the class for infinite planes in 3D space. A Plane is stored just as
 * a Point3 and a scalar:
 * - a direction (not necessarily normalized),
 * - an offset from the origin
 *
 * Just to be clear, given a point p on a plane it always holds:
 *    plane.direction().dot(p) == plane.offset()
 *
 * A vcl::Plane, once initialized, cannot be changed.
 *
 * @ingroup space
 */
template<typename Scalar, bool NORM=true>
class Plane
{
    Point3<Scalar> dir;
    Scalar off;

public:
    using ScalarType = Scalar;
    using PointType = vcl::Point3<Scalar>;

    /**
     * @brief Empty constructor. The plane is uninitialized.
     */
    Plane() {}

    /**
     * @brief Constructor of a plane given a direction and an offset.
     * @param direction
     * @param offset
     */
    Plane(const Point3<Scalar>& direction, Scalar offset) :
            dir(direction), off(offset)
    {
        if constexpr(NORM) {
            Scalar n = dir.norm();
            dir /= n;
            off /= n;
        }
    }

    /**
     * @brief Constructor of a plane given a point lying to the plane and the
     * normal of the plane.
     * @param p0
     * @param normal
     */
    Plane(const Point3<Scalar>& p0, const Point3<Scalar>& normal)
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
    Plane(
        const Point3<Scalar>& p0,
        const Point3<Scalar>& p1,
        const Point3<Scalar>& p2):
            Plane<Scalar, NORM>(p0, (p2 - p0).cross(p1 - p0))
    {
    }

    template<typename S>
    Plane<S, NORM> cast() const
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
    const Point3<Scalar>& direction() const { return dir; }

    /**
     * @brief Returns the offset component of the plane.
     * @return
     */
    Scalar offset() const { return off; }

    /**
     * @brief Given a point, returns the point projected to this plane.
     * @param p
     * @return
     */
    Point3<Scalar> projectPoint(const Point3<Scalar>& p) const
    {
        Scalar k = p.dot(dir) - off;
        return p - dir * k;
    }

    /**
     * @brief Given a point, returns the point mirrored w.r.t. this plane.
     * @param p
     * @return
     */
    Point3<Scalar> mirrorPoint(const Point3<Scalar>& p) const
    {
        Point3<Scalar> mirr=projectPoint(p);
        mirr += mirr-p;
        return mirr;
    }

    bool operator==(const Plane& p) const
    {
        return off == p.off && dir == p.dir;
    }

    bool operator!=(const Plane& p) const { return !(*this == p); }
};

/* Specialization Aliases */

using Planef = Plane<float>;
using Planed = Plane<double>;

} // namespace vcl

#endif // VCL_SPACE_PLANE_H
