// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_CORE_BOX_BOX2_H
#define VCL_ALGORITHMS_CORE_BOX_BOX2_H

#include <vclib/space/core.h>

#include <stdexcept>

/**
 * @defgroup core_box2 Core Box2 Algorithms
 *
 * @ingroup core_box
 *
 * @brief List of utility functions for boxes having 2 dimensions.
 *
 * The order of the vertices, and edges is as follows:
 *
 * @code
 *    v3__________e2__________v2
 *     |                      |
 *     |                      |
 *     |                      |
 *     |                      |
 *   e3|                      |e1
 *     |                      |
 *     |                      |
 *     |                      |
 *     |______________________|
 *    v0          e0          v1
 * @endcode
 */

namespace vcl {

/**
 * @brief Returns the ith vertex of a 2D box.
 *
 * @code
 *    v3______________________v2
 *     |                      |
 *     |                      |
 *     |                      |
 *     |                      |
 *     |                      |
 *     |                      |
 *     |                      |
 *     |                      |
 *     |______________________|
 *    v0                      v1
 * @endcode
 *
 * @tparam BoxType: The type of the box.
 * @param[in] box: The box.
 * @param[in] i: The index of the vertex.
 * @return The ith vertex of the box.
 *
 * @ingroup core_box2
 */
template<Box2Concept BoxType>
auto boxVertex(const BoxType& box, uint i) -> BoxType::PointType
{
    using PointType = BoxType::PointType;

    switch (i) {
    case 0: return box.min();
    case 1: return PointType(box.max().x(), box.min().y());
    case 2: return box.max();
    case 3: return PointType(box.min().x(), box.max().y());
    default:
        throw std::out_of_range("Invalid vertex index");
        return PointType();
    }
}

/**
 * @brief Returns the ith edge of a 2D box.
 *
 * @code
 *      __________e2__________
 *     |                      |
 *     |                      |
 *     |                      |
 *     |                      |
 *   e3|                      |e1
 *     |                      |
 *     |                      |
 *     |                      |
 *     |______________________|
 *                e0
 * @endcode
 *
 * @tparam BoxType: The type of the box.
 * @param[in] box: The box.
 * @param[in] i: The index of the edge.
 * @return The ith edge of the box.
 *
 * @ingroup core_box2
 */
template<Box2Concept BoxType>
auto boxEdge(const BoxType& box, uint i) -> Segment<typename BoxType::PointType>
{
    using PointType = BoxType::PointType;

    switch (i) {
    case 0: return Segment<PointType>(boxVertex(box, 0), boxVertex(box, 1));
    case 1: return Segment<PointType>(boxVertex(box, 1), boxVertex(box, 2));
    case 2: return Segment<PointType>(boxVertex(box, 2), boxVertex(box, 3));
    case 3: return Segment<PointType>(boxVertex(box, 3), boxVertex(box, 0));
    default:
        throw std::out_of_range("Invalid edge index");
        return Segment<PointType>();
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_BOX_BOX2_H
