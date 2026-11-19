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
 * @brief Utility class that represents an edge in a Mesh having Vertices and
 * Faces.
 *
 * An instance of MeshEdgeUtil stores:
 * - the index of the edge in the face (e)
 * - the index of the face in its container (f)
 * - the indices of the two (ordered) vertices of the edge (v[0], v[1])
 *
 * This class allows to compare edges in a Mesh. The ordering is done using the
 * vertex indices of the edge (the face index or the edge index within the face
 * are not used for comparison).
 *
 * @tparam MeshType: The type of the mesh. It must satisfy the FaceMeshConcept.
 *
 * @ingroup space_complex
 */
template<FaceMeshConcept MeshType>
class MeshEdgeUtil
{
public:
    using IndexType = vcl::IndexType<MeshType>;

    IndexType v[2]; // Indices of the two (ordered) vertices of the edge
    IndexType f;    // Index of the face of the edge
    int       e;    // Index of the edge inside the face

    MeshEdgeUtil() :
            v {MeshType::NULL_INDEX, MeshType::NULL_INDEX},
            f(MeshType::NULL_INDEX), e(-1)
    {
    }

    /**
     * @brief Constructs a MeshEdgeUtil from two vertex indices and an optional
     * face index and edge-in-face index.
     *
     * Vertex indices are stored in sorted (ascending) order so that the same
     * undirected edge always produces the same MeshEdgeUtil, regardless of
     * winding order.
     *
     * @param[in] vi0: Index of the first vertex.
     * @param[in] vi1: Index of the second vertex.
     * @param[in] fi:  Index of the face (default: NULL_INDEX).
     * @param[in] ne:  Index of the edge within the face (default: -1).
     */
    MeshEdgeUtil(
        IndexType vi0,
        IndexType vi1,
        IndexType fi = MeshType::NULL_INDEX,
        int       ne = -1) : f(fi), e(ne)
    {
        assert(vi0 != vi1);
        if (vi0 < vi1) {
            v[0] = vi0;
            v[1] = vi1;
        }
        else {
            v[0] = vi1;
            v[1] = vi0;
        }
    }

    /**
     * @brief Constructs a MeshEdgeUtil from the ne-th edge of face pf.
     *
     * The face proxy is accepted by value (proxies are lightweight) and can be
     * either a mutable or a const face proxy (Face<Container> or
     * Face<const Container>), which is why this constructor is templated on the
     * concrete face proxy type rather than using FaceType<MeshType> directly.
     *
     * @param[in] pf: The face proxy (mutable or const).
     * @param[in] ne: The edge index within the face (0 <= ne < vertexCount()).
     */
    MeshEdgeUtil(FaceConcept auto pf, uint ne)
    {
        v[0] = pf.vertexIndex(ne);
        v[1] = pf.vertexIndexMod(static_cast<int>(ne) + 1);
        assert(v[0] != v[1]);

        if (v[0] > v[1])
            std::swap(v[0], v[1]);
        f = pf.index();
        e = static_cast<int>(ne);
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

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_MESH_EDGE_UTIL_H
