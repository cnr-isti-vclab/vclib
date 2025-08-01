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

#ifndef VCL_SPACE_COMPLEX_MESH_EDGE_UTIL_H
#define VCL_SPACE_COMPLEX_MESH_EDGE_UTIL_H

#include <vclib/mesh.h>
#include <vclib/base.h>

namespace vcl {

/**
 * @brief Utility class that represents a edge in a Mesh having Vertices and
 * Faces.
 *
 * An instance of MeshEdgeUtil stores:
 * - the index of the edge in the face
 * - the pointer of the face
 * - the pointers of the vertices of the edge.
 *
 *
 * This class allows to compare edges in a Mesh. The ordering is done using the
 * vertex pointers of the edge (the face pointer or the edge index are not
 * used).
 *
 * @tparam MeshType: The type of the mesh. It must satisfy the FaceMeshConcept.
 *
 * @ingroup space_complex
 */
template<FaceMeshConcept MeshType, bool CNST = false>
class MeshEdgeUtil
{
    using VertexType = std::conditional_t<
        CNST,
        const typename MeshType::VertexType,
        typename MeshType::VertexType>;
    using FaceType = std::conditional_t<
        CNST,
        const typename MeshType::FaceType,
        typename MeshType::FaceType>;

public:
    VertexType* v[2]; // Pointer to the two (ordered) vertices of the edge
    FaceType*   f;    // Pointer to the face of the edge
    int         e;    // Index of the edge inside the face

    MeshEdgeUtil() : v {nullptr, nullptr}, f(nullptr), e(-1) {}

    MeshEdgeUtil(FaceType& pf, uint ne)
    {
        v[0] = pf.vertex(ne);
        v[1] = pf.vertexMod(ne + 1);
        assert(v[0] != v[1]);

        if (v[0] > v[1])
            std::swap(v[0], v[1]);
        f = &pf;
        e = ne;
    }

    bool operator<(const MeshEdgeUtil& pe) const
    {
        if (v[0] < pe.v[0])
            return true;
        else if (v[0] > pe.v[0])
            return false;
        else
            return v[1] < pe.v[1];
    }

    bool operator==(const MeshEdgeUtil& pe) const
    {
        return v[0] == pe.v[0] && v[1] == pe.v[1];
    }

    bool operator!=(const MeshEdgeUtil& pe) const
    {
        return v[0] != pe.v[0] || v[1] != pe.v[1];
    }
};

/**
 * @brief Const version of MeshEdgeUtil.
 *
 * @ingroup space_complex
 */
template<FaceMeshConcept MeshType>
using ConstMeshEdgeUtil = MeshEdgeUtil<MeshType, true>;

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_MESH_EDGE_UTIL_H
