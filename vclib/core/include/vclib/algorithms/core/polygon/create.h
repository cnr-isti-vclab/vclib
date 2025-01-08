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

#ifndef VCL_ALGORITHMS_CORE_POLYGON_CREATE_H
#define VCL_ALGORITHMS_CORE_POLYGON_CREATE_H

#include <vclib/space/core/polygon.h>

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
