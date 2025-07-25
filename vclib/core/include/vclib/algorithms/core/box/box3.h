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

#ifndef VCL_ALGORITHMS_CORE_BOX_BOX3_H
#define VCL_ALGORITHMS_CORE_BOX_BOX3_H

#include <vclib/concepts.h>
#include <vclib/space/core.h>

#include <stdexcept>

/**
 * @defgroup core_box3 Core Box3 Algorithms
 *
 * @ingroup core_box
 *
 * @brief List of utility functions for boxes having 3 dimensions.
 *
 * The order of the vertices, and edges is as follows:
 *
 * @code
 *                  v2_______e2_____________v3
 *                  /|                    /|
 *                 / |                   / |
 *             e10/  |               e11/  |
 *               /___|______e6_________/   |
 *            v6|    |                 |v7 |e3
 *              |    |                 |   |
 *              |    |e1               |e5 |
 *            e7|    |                 |   |
 *              |    |_________________|___|
 *              |   / v0      e0       |   /v1          ^ y
 *              |  /                   |  /             |
 *              | /e8                  | /e9            --> x
 *              |/_____________________|/              /
 *              v4         e4          v5             z
 * @endcode
 *
 * The order of the faces is as follows:
 *
 * @code
 * 0: v2, v3, v1, v0
 * 1: v4, v6, v2, v0
 * 2: v1, v5, v4, v0
 * 3: v6, v4, v5, v7
 * 4: v3, v2, v6, v7
 * 5: v5, v1, v3, v7
 * @endcode
 *
 * You can access these algorithms by including
 * `#include <vclib/algorithms/core/box/box3.h>`
 */

namespace vcl {

/**
 * @brief Returns the ith vertex of a 3D box.
 *
 * @code
 *
 *                  v2______________________v3
 *                  /|                    /|
 *                 / |                   / |
 *                /  |                  /  |
 *               /___|_________________/   |
 *            v6|    |                 |v7 |
 *              |    |                 |   |
 *              |    |                 |   |
 *              |    |                 |   |
 *              |    |_________________|___|
 *              |   / v0               |   /v1          ^ y
 *              |  /                   |  /             |
 *              | /                    | /              --> x
 *              |/_____________________|/              /
 *              v4                     v5             z
 * @endcode
 *
 * @tparam BoxType: The type of the box.
 * @param[in] box: The box.
 * @param[in] i: The index of the vertex.
 * @return The ith vertex of the box.
 *
 * @ingroup core_box3
 */
template<Box3Concept BoxType>
auto boxVertex(const BoxType& box, uint i) -> BoxType::PointType
{
    using PointType = BoxType::PointType;

    switch (i) {
    case 0: return box.min();
    case 1: return PointType(box.max().x(), box.min().y(), box.min().z());
    case 2: return PointType(box.min().x(), box.max().y(), box.min().z());
    case 3: return PointType(box.max().x(), box.max().y(), box.min().z());
    case 4: return PointType(box.min().x(), box.min().y(), box.max().z());
    case 5: return PointType(box.max().x(), box.min().y(), box.max().z());
    case 6: return PointType(box.min().x(), box.max().y(), box.max().z());
    case 7: return box.max();
    default:
        throw std::out_of_range("Invalid vertex index");
        return PointType();
    }
}

/**
 * @brief Returns the ith edge of a 3D box.
 *
 * @code
 *                   ________e2_____________
 *                  /|                    /|
 *                 / |                   / |
 *             e10/  |               e11/  |
 *               /___|______e6_________/   |
 *              |    |                 |   |e3
 *              |    |                 |   |
 *              |    |e1               |e5 |
 *            e7|    |                 |   |
 *              |    |_________________|___|
 *              |   /         e0       |   /            ^ y
 *              |  /                   |  /             |
 *              | /e8                  | /e9            --> x
 *              |/_____________________|/              /
 *                         e4                         z
 * @endcode
 *
 * @tparam BoxType: The type of the box.
 * @param[in] box: The box.
 * @param[in] i: The index of the edge.
 * @return The ith edge of the box.
 *
 * @ingroup core_box3
 */
template<Box3Concept BoxType>
auto boxEdge(const BoxType& box, uint i) -> Segment<typename BoxType::PointType>
{
    using PointType = BoxType::PointType;

    switch (i) {
    case 0: return Segment<PointType>(boxVertex(box, 1), boxVertex(box, 0));
    case 1: return Segment<PointType>(boxVertex(box, 0), boxVertex(box, 2));
    case 2: return Segment<PointType>(boxVertex(box, 2), boxVertex(box, 3));
    case 3: return Segment<PointType>(boxVertex(box, 3), boxVertex(box, 1));
    case 4: return Segment<PointType>(boxVertex(box, 4), boxVertex(box, 5));
    case 5: return Segment<PointType>(boxVertex(box, 5), boxVertex(box, 7));
    case 6: return Segment<PointType>(boxVertex(box, 7), boxVertex(box, 6));
    case 7: return Segment<PointType>(boxVertex(box, 6), boxVertex(box, 4));
    case 8: return Segment<PointType>(boxVertex(box, 0), boxVertex(box, 4));
    case 9: return Segment<PointType>(boxVertex(box, 1), boxVertex(box, 5));
    case 10: return Segment<PointType>(boxVertex(box, 2), boxVertex(box, 6));
    case 11: return Segment<PointType>(boxVertex(box, 3), boxVertex(box, 7));
    default:
        throw std::out_of_range("Invalid edge index");
        return Segment<PointType>();
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_BOX_BOX3_H
