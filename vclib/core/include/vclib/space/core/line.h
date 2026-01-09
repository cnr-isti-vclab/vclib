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

#ifndef VCL_SPACE_CORE_LINE_H
#define VCL_SPACE_CORE_LINE_H

#include "point.h"

namespace vcl {

/**
 * @brief A class representing a line in n-dimensional space. The class
 * is parameterized by a `PointConcept`, which must provide the `DIM` constant
 * and the `[]` operator for accessing the point coordinates.
 *
 * @tparam PointT The type of the two endpoint points of the line.
 *
 * @ingroup space_core
 */
template<PointConcept PointT>
class Line
{
    PointT mOrigin;
    PointT mDirection;

public:
    /**
     * @brief The type of point used to represent the origin and direction of
     * the line.
     */
    using PointType = PointT;

    /**
     * @brief The scalar type of the line.
     */
    using ScalarType = PointT::ScalarType;

    /**
     * @brief The dimensionality of the line.
     */
    static const uint DIM = PointT::DIM;

    /**
     * @brief Default constructor. Creates an invalid Line at the origin
     * (direction is zero).
     */
    Line() {}

    /**
     * @brief Creates a line with the given origin and direction.
     *
     * @param[in] origin: The origin of the line.
     * @param[in] direction: The direction of the line.
     */
    Line(const PointT& origin, const PointT& direction) :
            mOrigin(origin), mDirection(direction)
    {
    }

    /**
     * @brief Returns the origin of the line.
     *
     * @return A reference to the origin of the line.
     */
    PointT& origin() { return mOrigin; }

    /**
     * @brief Returns the origin of the line.
     *
     * @return A const reference to the origin of the line.
     */
    const PointT& origin() const { return mOrigin; }

    /**
     * @brief Returns the direction of the line.
     *
     * @return A reference to the direction of the line.
     */
    PointT& direction() { return mDirection; }

    /**
     * @brief Returns the direction of the line.
     *
     * @return A reference to the direction of the line.
     */
    const PointT& direction() const { return mDirection; }

    PointT normalizedDirection() const { return mDirection.normalized(); }

    bool operator==(const Line<PointT>& s) const = default;

    bool operator!=(const Line<PointT>& s) const = default;
};

/* Specialization Aliases */

template<typename S>
using Line2 = Line<Point2<S>>;

using Line2i = Line<Point2i>;
using Line2f = Line<Point2f>;
using Line2d = Line<Point2d>;

template<typename S>
using Line3 = Line<Point3<S>>;

using Line3i = Line<Point3i>;
using Line3f = Line<Point3f>;
using Line3d = Line<Point3d>;

/* Concepts */

/**
 * @brief A concept representing a Line.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Line class having any Point type.
 *
 * @tparam T: The type to be tested for conformity to the LineConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept LineConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    Line<typename RemoveRef<T>::PointType>>;

/**
 * @brief A concept representing a 2D Line.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Line class having a Point type with dimension 2.
 *
 * @tparam T: The type to be tested for conformity to the Line2Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept Line2Concept = LineConcept<T> && RemoveRef<T>::DIM == 2;

/**
 * @brief A concept representing a 3D Line.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Line class having a Point type with dimension 3.
 *
 * @tparam T: The type to be tested for conformity to the Line3Concept.
 *
 * @ingroup space_core
 */
template<typename T>
concept Line3Concept = LineConcept<T> && RemoveRef<T>::DIM == 3;

} // namespace vcl

#endif // VCL_SPACE_CORE_LINE_H
