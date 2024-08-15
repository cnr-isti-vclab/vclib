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

#ifndef VCL_ALGORITHMS_CORE_POLYGON_PROJECTION_H
#define VCL_ALGORITHMS_CORE_POLYGON_PROJECTION_H

#include <vclib/concepts/space/point.h>
#include <vclib/space/core/polygon.h>

namespace vcl {

/**
 * @brief Project a 3D polygon onto a plane, and return the 2D polygon.
 *
 * @tparam Iterator: Iterator type, it must iterate over 3D points.
 * @param[in] begin: Iterator to the first point of the polygon.
 * @param[in] end: Iterator to the past-the-end point of the polygon.
 * @return The projected polygon.
 *
 * @ingroup core_polygon
 */
template<Point3IteratorConcept Iterator>
auto project(Iterator begin, Iterator end)
{
    using PointType  = Iterator::value_type;
    using ScalarType = PointType::ScalarType;

    Polygon2<ScalarType> projectedPolygon;

    // Calculate the normal vector of the polygon and an orthonormal basis
    // for the plane containing the polygon.
    PointType normal = Polygon3<ScalarType>::normal(begin, end);
    PointType u, v;
    normal.orthoBase(u, v);

    projectedPolygon.reserve(std::distance(begin, end));

    // Project each vertex onto the plane defined by the orthonormal basis.
    for (auto it = begin; it != end; ++it) {
        ScalarType x = (*it).dot(u);
        ScalarType y = (*it).dot(v);
        projectedPolygon.pushBack(Point2<ScalarType>(x, y));
    }

    return projectedPolygon;
}

/**
 * @brief Project a 3D polygon defined by the given range onto a plane, and
 * return the 2D polygon.
 *
 * @tparam R: A range of 3D points.
 * @param[in] polygon: The input polygon.
 * @return The projected polygon.
 *
 * @ingroup core_polygon
 */
template<vcl::Range R>
auto project(R&& polygon) requires Point3Concept<std::ranges::range_value_t<R>>
{
    return project(polygon.begin(), polygon.end());
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_PROJECTION_H
