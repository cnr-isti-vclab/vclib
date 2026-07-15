// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
        if constexpr (HasFaces<MeshType>) {
            if constexpr (vcl::HasTriangles<MeshType>)
                name = "Tri";
            else if constexpr (vcl::HasQuads<MeshType>)
                name = "Quad";
            else
                name = "Poly";
        }

        if constexpr (vcl::HasEdges<MeshType>)
            name += "Edge";

        name += "Mesh";

        bool isIndexed = false;
        if constexpr (HasFaces<MeshType>) {
            if constexpr (MeshType::FaceType::INDEXED) {
                isIndexed = true;
            }
        }
        if constexpr (HasEdges<MeshType>) {
            if constexpr (MeshType::EdgeType::INDEXED) {
                isIndexed = true;
            }
        }

        if (isIndexed) {
            name += "Indexed";
        }
    }

    if constexpr (std::same_as<ScalarType, float>)
        name += "f";

    return name;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_TYPE_NAME_H
