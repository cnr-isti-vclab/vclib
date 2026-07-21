// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_CORE_POLYGON_CREATE_H
#define VCL_ALGORITHMS_CORE_POLYGON_CREATE_H

#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief Create a 2D circle polygon with n vertices and the given radius.
 * @tparam PolygonType: The polygon type.
 * @param[in] n: The number of vertices.
 * @param[in] radius: The radius of the circle.
 * @return The circle polygon.
 *
 * @ingroup core_polygon
 */
template<Polygon2Concept PolygonType>
PolygonType createCircle(uint n, typename PolygonType::ScalarType radius = 1.0)
{
    using ScalarType = typename PolygonType::ScalarType;
    using PointType  = typename PolygonType::PointType;

    PolygonType poly;
    poly.reserve(n);

    const ScalarType angleStep = 2 * M_PI / n;
    for (uint i = 0; i < n; ++i) {
        const ScalarType angle = i * angleStep;
        const PointType  p =
            PointType(std::cos(angle), std::sin(angle)) * radius;
        poly.pushBack(p);
    }

    return poly;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_CREATE_H
