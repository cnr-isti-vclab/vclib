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

#ifndef VCL_ALGORITHMS_CORE_CREATE_H
#define VCL_ALGORITHMS_CORE_CREATE_H

#include "polygon/create.h"

#include <vclib/space/core/image.h>

/**
 * @defgroup core_create Core Create Algorithms
 *
 * @ingroup algorithms_core
 *
 * @brief List core algorithms for creating generic objects.
 *
 * You can access these algorithms by including
 * `#include <vclib/algorithms/core/create.h>`
 */

namespace vcl {

/**
 * @brief Returns a pair of vectors containing the vertices and edges of a 3D
 * Trackball, composed of three circles in the x, y, and z planes.
 *
 * @tparam ScalarType: the type of scalar used for the 3D vertices.
 * @tparam UintType: the type of integer used for the edge indices.
 *
 * @param[in] scale: the scale of the trackball.
 * @param[in] pointsPerCircle: the number of points per circle.
 * @return a pair of vectors containing the vertices and edges of the trackball.
 *
 * @ingroup core_create
 */
template<typename ScalarType = float, std::integral UintType = uint16_t>
std::pair<std::vector<vcl::Point3<ScalarType>>, std::vector<UintType>>
createTrackBall(ScalarType scale = 1.0, uint pointsPerCircle = 64)
{
    using PointType = vcl::Point3<ScalarType>;

    std::vector<PointType> vertices;
    std::vector<UintType>  edges;

    vcl::Polygon2<ScalarType> circle =
        vcl::createCircle<vcl::Polygon2<ScalarType>>(pointsPerCircle, 1.0);

    vertices.reserve(pointsPerCircle * 3);

    // x
    uint first = 0;
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertices.push_back(PointType(0, p.x(), p.y()));
        edges.push_back(i + first);
        edges.push_back((i + 1) % circle.size() + first);
    }

    // y
    first = circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertices.push_back(PointType(p.x(), 0, p.y()));
        edges.push_back(i + first);
        edges.push_back((i + 1) % circle.size() + first);
    }

    // z
    first = 2 * circle.size();
    for (uint i = 0; i < circle.size(); ++i) {
        const auto& p = circle.point(i);
        vertices.push_back(PointType(p.x(), p.y(), 0));
        edges.push_back(i + first);
        edges.push_back((i + 1) % circle.size() + first);
    }

    return std::make_pair(std::move(vertices), std::move(edges));
}

/**
 * @brief Create a checkboard image.
 *
 * @param[in] imageSize: the size of the image.
 * @param[in] checkNum: the number of checks.
 * @return the checkboard image.
 *
 * @ingroup core_create
 */
inline Image createCheckBoardImage(
    uint imageSize,
    uint checkNum = 8)
{
    vcl::Array2<uint> img(imageSize, imageSize);

    for (int y = 0; y < imageSize; ++y) {
        for (int x = 0; x < imageSize; ++x) {
            if (((x / checkNum) % 2) == ((y / checkNum) % 2)) {
                img(x, y) = 0xFFFFFFFF;
            }
            else {
                img(x, y) = 0xFF808080;
            }
        }
    }

    return Image(std::move(img));
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_CREATE_H
