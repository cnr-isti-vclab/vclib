/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SPACE_PLANE_H
#define VCL_SPACE_PLANE_H

#include <vclib/concepts/space/plane.h>
#include <vclib/exceptions/misc.h>

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
template<typename Scalar, bool NORM = true>
class Plane
{
    Point3<Scalar> mDir;
    Scalar         mOffset;

public:
    using ScalarType = Scalar;
    using PointType  = vcl::Point3<Scalar>;

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
            mDir(direction), mOffset(offset)
    {
        if constexpr (NORM) {
            Scalar n = mDir.norm();
            mDir /= n;
            mOffset /= n;
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
        mDir = normal;
        if constexpr (NORM) {
            mDir.normalize();
        }
        mOffset = p0.dot(mDir);
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
        const Point3<Scalar>& p2) :
            Plane<Scalar, NORM>(p0, (p2 - p0).cross(p1 - p0))
    {
    }

    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same<Scalar, S>::value) {
            return *this;
        }
        else {
            return Plane<S, NORM>(mDir.template cast<S>(), mOffset);
        }
    }

    /**
     * @brief Returns the direction component of the plane.
     * @return
     */
    const Point3<Scalar>& direction() const { return mDir; }

    /**
     * @brief Returns the offset component of the plane.
     * @return
     */
    Scalar offset() const { return mOffset; }

    /**
     * @brief Given a point, returns the point projected to this plane.
     * @param p
     * @return
     */
    Point3<Scalar> projectPoint(const Point3<Scalar>& p) const
    {
        Scalar k = p.dot(mDir) - mOffset;
        return p - mDir * k;
    }

    /**
     * @brief Given a point, returns the point mirrored w.r.t. this plane.
     * @param p
     * @return
     */
    Point3<Scalar> mirrorPoint(const Point3<Scalar>& p) const
    {
        Point3<Scalar> mirr = projectPoint(p);
        mirr += mirr - p;
        return mirr;
    }

    bool operator==(const Plane& p) const
    {
        return mOffset == p.mOffset && mDir == p.mDir;
    }

    bool operator!=(const Plane& p) const { return !(*this == p); }
};

/* Specialization Aliases */

using Planef = Plane<float>;
using Planed = Plane<double>;

} // namespace vcl

#endif // VCL_SPACE_PLANE_H
