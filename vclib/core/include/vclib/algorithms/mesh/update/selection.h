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
        for (uint i = 0; i < f.vertexNumber(); ++i) {
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
