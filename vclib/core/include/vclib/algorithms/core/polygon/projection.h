// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_CORE_POLYGON_PROJECTION_H
#define VCL_ALGORITHMS_CORE_POLYGON_PROJECTION_H

#include <vclib/space/core.h>

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
template<Range R>
auto project(R&& polygon) requires Point3Concept<std::ranges::range_value_t<R>>
{
    return project(polygon.begin(), polygon.end());
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_PROJECTION_H
