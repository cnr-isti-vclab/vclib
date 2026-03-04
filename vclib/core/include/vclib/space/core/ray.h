/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_SPACE_CORE_RAY_H
#define VCL_SPACE_CORE_RAY_H

#include "point.h"

namespace vcl {

/**
 * @brief A class representing a ray in n-dimensional space. The class
 * is parameterized by a `PointConcept`, which must provide the `DIM` constant
 * and the `[]` operator for accessing the point coordinates.
 *
 * @tparam PointT The type of the two endpoint points of the ray.
 *
 * @ingroup space_core
 */
template<PointConcept PointT>
class Ray
{
    PointT mOrigin;
    PointT mDirection;

public:
    /**
     * @brief The type of point used to represent the origin and direction of
     * the ray.
     */
    using PointType = PointT;

    /**
     * @brief The scalar type of the ray.
     */
    using ScalarType = PointT::ScalarType;

    /**
     * @brief The dimensionality of the ray.
     */
    static const uint DIM = PointT::DIM;

    /**
     * @brief Default constructor. Creates an invalid Ray at the origin
     * (direction is zero).
     */
    Ray() {}

    /**
     * @brief Creates a ray with the given origin and direction.
     *
     * @param[in] origin: The origin of the ray.
     * @param[in] direction: The direction of the ray.
     */
    Ray(const PointT& origin, const PointT& direction) :
            mOrigin(origin), mDirection(direction)
    {
    }

    /**
     * @brief Returns the origin of the ray.
     *
     * @return A reference to the origin of the ray.
     */
    PointT& origin() { return mOrigin; }

    /**
     * @brief Returns the origin of the ray.
     *
     * @return A const reference to the origin of the ray.
     */
    const PointT& origin() const { return mOrigin; }

    /**
     * @brief Returns the direction of the ray.
     *
     * @return A reference to the direction of the ray.
     */
    PointT& direction() { return mDirection; }

    /**
     * @brief Returns the direction of the ray.
     *
     * @return A reference to the direction of the ray.
     */
    const PointT& direction() const { return mDirection; }

    PointT normalizedDirection() const { return mDirection.normalized(); }

    bool operator==(const Ray<PointT>& s) const = default;

    bool operator!=(const Ray<PointT>& s) const = default;
};

/* Specialization Aliases */

template<typename S>
using Ray2 = Ray<Point2<S>>;

using Ray2i = Ray<Point2i>;
using Ray2f = Ray<Point2f>;
using Ray2d = Ray<Point2d>;

template<typename S>
using Ray3 = Ray<Point3<S>>;

using Ray3i = Ray<Point3i>;
using Ray3f = Ray<Point3f>;
using Ray3d = Ray<Point3d>;

/* Concepts */

/**
 * @brief A concept representing a Ray.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Ray class having any Point type.
 *
 * @tparam T: The type to be tested for conformity to the RayConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept RayConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    Ray<typename RemoveRef<T>::PointType>>;

/**
 * @brief A concept representing a 2D Ray.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Ray class having a Point type with dimension 2.
 *
 * @tparam T: The type to be tested for conformity to the Ray2Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept Ray2Concept = RayConcept<T> && RemoveRef<T>::DIM == 2;

/**
 * @brief A concept representing a 3D Ray.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Ray class having a Point type with dimension 3.
 *
 * @tparam T: The type to be tested for conformity to the Ray3Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept Ray3Concept = RayConcept<T> && RemoveRef<T>::DIM == 3;

} // namespace vcl

#endif // VCL_SPACE_CORE_RAY_H
