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

#ifndef VCL_ALGORITHMS_MESH_STAT_BOUNDING_BOX_H
#define VCL_ALGORITHMS_MESH_STAT_BOUNDING_BOX_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/core/box.h>

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
