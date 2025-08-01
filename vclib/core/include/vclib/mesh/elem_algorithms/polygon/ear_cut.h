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

#ifndef VCL_MESH_ELEM_ALGORITHMS_POLYGON_EAR_CUT_H
#define VCL_MESH_ELEM_ALGORITHMS_POLYGON_EAR_CUT_H

#include <vclib/mesh/elements.h>
#include <vclib/mesh/views/components.h>

#include <vclib/algorithms/core.h>

namespace vcl {

/**
 * @brief Computes the earcut algorithm of a 3D *planar* polygonal face, that
 * returns a triangulation of the polygon.
 *
 * Returns a list of indices in which each index is the index of a point of the
 * 3D input polygon, organized in triplets, each one of these is a triangle of
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
 *
 * @ingroup core_polygon
 */
template<FaceConcept Face>
std::vector<uint> earCut(const Face& polygon)
{
    using PositionType = Face::VertexType::PositionType;
    return earCut(polygon.vertices() | views::positions);
}

} // namespace vcl

#endif // VCL_MESH_ELEM_ALGORITHMS_POLYGON_EAR_CUT_H
