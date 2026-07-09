// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_SELECTION_H
#define VCL_ALGORITHMS_MESH_UPDATE_SELECTION_H

#include <vclib/algorithms/mesh/clean.h>
#include <vclib/algorithms/mesh/stat.h>

#include <vclib/mesh.h>

namespace vcl {

template<uint ELEM_ID, MeshConcept MeshType>
void clearElementSelection(MeshType& mesh)
{
    for (auto&& e : mesh.template elements<ELEM_ID>()) {
        e.selected() = false;
    }
}

template<MeshConcept MeshType>
void clearVertexSelection(MeshType& m)
{
    clearElementSelection<ElemId::VERTEX>(m);
}

template<FaceMeshConcept MeshType>
void clearFaceSelection(MeshType& m)
{
    clearElementSelection<ElemId::FACE>(m);
}

template<FaceMeshConcept MeshType>
void clearFaceEdgesSelection(MeshType& m)
{
    for (auto& f : m.faces()) {
        for (uint i = 0; i < f.vertexCount(); ++i) {
            f.edgeSelected(i) = false;
        }
    }
}

template<EdgeMeshConcept MeshType>
void clearEdgeSelection(MeshType& m)
{
    clearElementSelection<ElemId::EDGE>(m);
}

template<FaceMeshConcept MeshType>
void selectNonManifoldVertices(MeshType& m, bool clearSelectionFirst = true)
{
    std::vector<bool> nonManifoldVertices =
        detail::nonManifoldVerticesVectorBool(m);

    using VertexType = MeshType::VertexType;

    for (VertexType& v : m.vertices()) {
        if (nonManifoldVertices[m.index(v)]) {
            v.selected() = true;
        }
        else if (clearSelectionFirst) {
            v.selected() = false;
        }
    }
}

template<FaceMeshConcept MeshType>
void selectCreaseFaceEdges(
    MeshType& m,
    double    angleRadNeg,
    double    angleRadPos,
    bool      alsoBorderEdges = false)
{
    clearFaceEdgesSelection(m);

    std::vector<std::pair<uint, uint>> creaseEdges =
        creaseFaceEdges(m, angleRadNeg, angleRadPos, alsoBorderEdges);

    for (const auto& [fi, ei] : creaseEdges) {
        m.face(fi).edgeSelected(ei) = true;
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_SELECTION_H
