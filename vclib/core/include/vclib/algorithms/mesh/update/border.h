// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_BORDER_H
#define VCL_ALGORITHMS_MESH_UPDATE_BORDER_H

#include <vclib/algorithms/mesh/sort.h>

#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief Computes per-face border flags without requiring any kind of
 * topology info.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * Complexity: O(NF log (NF))
 *
 * @param m: the mesh on which the border flags will be updated
 */
template<FaceMeshConcept MeshType>
void updateBorder(MeshType& m)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    for (FaceType& f : m.faces())
        for (uint i = 0; i < f.vertexCount(); ++i)
            f.edgeOnBorder(i) = false;

    if (m.faceCount() == 0)
        return;

    std::vector<MeshEdgeUtil<MeshType>> e = fillAndSortMeshEdgeUtilVector(m);

    typename std::vector<MeshEdgeUtil<MeshType>>::iterator pe, ps;
    ps = e.begin();
    pe = e.begin();
    do {
        if (pe == e.end() || *pe != *ps) { // Trovo blocco di edge uguali
            if (pe - ps == 1) {
                ps->f->edgeOnBorder(ps->e) = true;
            }
            ps = pe;
        }
        if (pe != e.end())
            ++pe;
    } while (pe != e.end());
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_BORDER_H
