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

#ifndef VCL_ALGORITHMS_MESH_TYPE_NAME_H
#define VCL_ALGORITHMS_MESH_TYPE_NAME_H

#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief Returns the name of the mesh type as a string.
 * @return the name of the mesh type as a string.
 *
 * @ingroup algorithms_mesh
 */
template<MeshConcept MeshType>
constexpr std::string meshTypeName()
{
    using ScalarType = MeshType::VertexType::PositionType::ScalarType;

    std::string name;

    if constexpr (!HasFaces<MeshType> && !HasEdges<MeshType>) {
        name = "PointCloud";
    }
    else {
        if constexpr (vcl::HasTriangles<MeshType>)
            name = "Tri";
        else if constexpr (vcl::HasQuads<MeshType>)
            name = "Quad";
        else
            name = "Poly";

        if constexpr (vcl::HasEdges<MeshType>)
            name += "Edge";

        name += "Mesh";

        if constexpr (HasFaces<MeshType>) {
            if constexpr (MeshType::FaceType::INDEXED) {
                name += "Indexed";
            }
        }
        if constexpr (HasEdges<MeshType>) {
            if constexpr (MeshType::EdgeType::INDEXED) {
                name += "Indexed";
            }
        }
    }

    if constexpr (std::same_as<ScalarType, float>)
        name += "f";

    return name;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_TYPE_NAME_H
