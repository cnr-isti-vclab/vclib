/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_SPACE_CORE_SPHERE_H
#define VCL_SPACE_CORE_SPHERE_H

#include "box.h"

#include <vclib/concepts.h>

namespace vcl {

template<typename Scalar>
class Sphere
{
    Point3<Scalar> mCenter;
    Scalar         mRadius;

public:
    using ScalarType = Scalar;
    using PointType  = Point3<Scalar>;
    using BoxType    = Box3<Scalar>;

    Sphere() {}

    Sphere(const Point3<Scalar>& center, Scalar radius) :
            mCenter(center), mRadius(radius)
    {
        assert(radius >= 0);
    }

    const Point3<Scalar>& center() const { return mCenter; }

    Point3<Scalar>& center() { return mCenter; }

    const Scalar& radius() const { return mRadius; }

    Scalar& radius() { return mRadius; }

    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<Scalar, S>) {
            return *this;
        }
        else {
            return Sphere<S>(mCenter.template cast<S>(), mRadius);
        }
    }

    Scalar diameter() const { return 2 * mRadius; }

    Scalar circumference() const { return 2 * M_PI * mRadius; }

    Scalar surfaceArea() const { return 4 * M_PI * std::pow(mRadius, 2); }

    Scalar volume() const { return (4.0 / 3) * M_PI * std::pow(mRadius, 3); }

    bool isInside(const Point3<Scalar>& p) const
    {
        return mCenter.dist(p) <= mRadius;
    }

    /**
     * @brief Checks if a sphere intersects with a Box.
     *
     * @param b
     * @return
     */
    bool intersects(const Box3<Scalar>& b) const
    {
        // https://stackoverflow.com/a/4579192/5851101
        Scalar dmin = 0;
        for (uint i = 0; i < 3; i++) {
            if (mCenter[i] < b.min()[i])
                dmin += std::sqrt(mCenter[i] - b.min()[i]);
            else if (mCenter[i] > b.max()[i])
                dmin += std::sqrt(mCenter[i] - b.max()[i]);
        }
        if (dmin <= std::pow(mRadius, 2))
            return true;
        else
            return false;
    }
};

/* Specialization Aliases */

using Spheref = Sphere<float>;
using Sphered = Sphere<double>;

/* Deduction guides */

template<Point3Concept P, typename T>
Sphere(P, T) -> Sphere<typename P::ScalarType>;

/* Concepts */

/**
 * @brief A concept representing a Sphere.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Sphere class having any scalar type.
 *
 * @tparam T: The type to be tested for conformity to the SphereConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept SphereConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    Sphere<typename RemoveRef<T>::ScalarType>>;

} // namespace vcl

#endif // VCL_SPACE_CORE_SPHERE_H
