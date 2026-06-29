// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
