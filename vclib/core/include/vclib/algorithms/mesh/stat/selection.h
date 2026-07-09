// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_STAT_SELECTION_H
#define VCL_ALGORITHMS_MESH_STAT_SELECTION_H

#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief Return the number of elements identified by `ELEM_ID` of the mesh that
 * are marked as selected.
 *
 * @tparam ELEM_ID: The element ID to be checked (e.g., vertex, edge, face).
 *
 * @param[in] m: The input mesh.
 * @return The number of selected elements.
 */
template<uint ELEM_ID, MeshConcept MeshType>
uint elementSelectionCount(const MeshType& m)
{
    return std::ranges::distance(
        m.template elements<ELEM_ID>() | views::selected);
}

/**
 * @brief Return the number of vertices of the mesh that are marked as selected.
 * @param[in] m: The input mesh.
 * @return The number of selected vertices.
 */
template<MeshConcept MeshType>
uint vertexSelectionCount(const MeshType& m)
{
    return elementSelectionCount<ElemId::VERTEX>(m);
}

/**
 * @brief Return the number of faces of the mesh that are marked as selected.
 * @param[in] m: The input mesh.
 * @return The number of selected faces.
 */
template<FaceMeshConcept MeshType>
uint faceSelectionCount(const MeshType& m)
{
    return elementSelectionCount<ElemId::FACE>(m);
}

/**
 * @brief Return the number of face edges of the mesh that are marked as
 * selected.
 *
 * With 'face edges' it is intended the edges that are part of the faces of the
 * mesh (e.g. if a face has 4 vertices, it has 4 edges).
 *
 * @param[in] m: The input mesh.
 * @return The number of selected edges.
 */
template<FaceMeshConcept MeshType>
uint faceEdgesSelectionCount(const MeshType& m)
{
    uint cnt = 0;
    for (const auto& f : m.faces()) {
        for (uint i = 0; i < f.vertexCount(); ++i) {
            if (f.edgeSelected(i)) {
                ++cnt;
            }
        }
    }
    return cnt;
}

/**
 * @brief Return the number of edges of the mesh that are marked as selected.
 *
 * @param[in] m: The input mesh.
 * @return The number of selected edges.
 */
template<EdgeMeshConcept MeshType>
uint edgeSelectionCount(const MeshType& m)
{
    return elementSelectionCount<ElemId::EDGE>(m);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_SELECTION_H
