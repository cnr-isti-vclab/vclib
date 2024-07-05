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

#ifndef VCL_ALGORITHMS_CORE_POLYGON_TOPOLOGY_H
#define VCL_ALGORITHMS_CORE_POLYGON_TOPOLOGY_H

#include <set>

#include <vclib/concepts/mesh.h>
#include <vclib/mesh/views.h>
#include <vclib/space/core/polygon.h>

namespace vcl {

/**
 * @brief Computes the earcut algorithm of a 3D *planar* polygon, that returns a
 * triangulation of the polygon.
 *
 * Returns a list of indices in which each index is the index of a point of the
 * 3D input polgon, organized in triplets, each one of these is a triangle of
 * the resulting triangulation.
 *
 * This algorithm first computes the normal of the given polygon, then projects
 * it in a 2D plane and executes the classic 2D EarCut algorithm.
 *
 * @tparam Face: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] polygon: A (polygonal) face of a vcl::Mesh.
 *
 * @return A vector of indices, representing the triplets of the triangulation
 * of the polygon.
 */
template<FaceConcept Face>
std::vector<uint> earCut(const Face& polygon)
{
    using CoordType = Face::VertexType::CoordType;
    return Polygon<CoordType>::earCut(polygon.vertices() | views::coords);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_TOPOLOGY_H
