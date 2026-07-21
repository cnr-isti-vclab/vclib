// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_COMPLEX_MESH_EDGE_UTIL_H
#define VCL_SPACE_COMPLEX_MESH_EDGE_UTIL_H

#include <vclib/base.h>
#include <vclib/mesh.h>

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

    MeshEdgeUtil(
        VertexType* v0,
        VertexType* v1,
        FaceType*   pf = nullptr,
        int         ne = -1) : f(pf), e(ne)
    {
        assert(v0 != v1);
        if (v0 < v1) {
            v[0] = v0;
            v[1] = v1;
        }
        else {
            v[0] = v1;
            v[1] = v0;
        }
    }

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
