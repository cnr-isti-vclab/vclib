// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_STAT_BOUNDING_BOX_H
#define VCL_ALGORITHMS_MESH_STAT_BOUNDING_BOX_H

#include <vclib/mesh.h>
#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief Compute the bounding box of a mesh
 *
 * Given a mesh `m`, this function computes and returns the bounding
 * box of the mesh. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam MeshType: The type of the mesh. It must satisfy the MeshConcept.
 *
 * @param[in] m: The input mesh to compute the bounding box of
 * @return The bounding box of the input mesh
 *
 * @ingroup mesh_stat
 */
template<MeshConcept MeshType>
auto boundingBox(const MeshType& m)
{
    using VertexType = MeshType::VertexType;
    Box<typename VertexType::PositionType> b;

    for (const VertexType& v : m.vertices()) {
        b.add(v.position());
    }

    return b;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_BOUNDING_BOX_H
