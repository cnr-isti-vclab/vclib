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

#ifndef VCL_SPACE_SPHERE_H
#define VCL_SPACE_SPHERE_H

#include <vclib/concepts/space/sphere.h>

#include "box.h"

namespace vcl {

template<typename Scalar>
class Sphere
{
    vcl::Point3<Scalar> c;
    Scalar              r;

public:
    using ScalarType = Scalar;
    using PointType  = vcl::Point3<Scalar>;

    Sphere() {}

    Sphere(const vcl::Point3<Scalar>& center, Scalar radius) :
            c(center), r(radius)
    {
    }

    const Point3<Scalar>& center() const { return c; }

    Point3<Scalar>& center() { return c; }

    const Scalar& radius() const { return r; }

    Scalar& radius() { return r; }

    template<typename S>
    auto cast() const
    {
        if constexpr (std::is_same_v<Scalar, S>) {
            return *this;
        }
        else {
            return Sphere<S>(c.template cast<S>(), r);
        }
    }

    Scalar diameter() const { return 2 * r; }

    Scalar circumference() const { return 2 * M_PI * r; }

    Scalar surfaceArea() const { return 4 * M_PI * std::pow(r, 2); }

    Scalar volume() const { return (4.0 / 3) * M_PI * std::pow(r, 3); }

    bool isInside(const vcl::Point3<Scalar>& p) const { return c.dist(p) <= r; }

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
            if (c[i] < b.min()[i])
                dmin += std::sqrt(c[i] - b.min()[i]);
            else if (c[i] > b.max()[i])
                dmin += std::sqrt(c[i] - b.max()[i]);
        }
        if (dmin <= std::pow(r, 2))
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

} // namespace vcl

#endif // VCL_SPACE_SPHERE_H
