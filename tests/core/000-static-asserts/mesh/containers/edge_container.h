// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CONT_EDGE_CONTAINER_H
#define CONT_EDGE_CONTAINER_H

#include <vclib/meshes.h>

void edgeContainerStaticAsserts()
{
    using namespace vcl;

    using VertexContainer =
        mesh::VertexContainer<trimesh::Vertex<float, false>>;
    using FaceContainer = mesh::FaceContainer<trimesh::Face<float, false>>;
    using EdgeContainer = mesh::EdgeContainer<edgemesh::Edge<float, false>>;

    static_assert(
        !mesh::HasEdgeContainer<VertexContainer>,
        "VertexContainer does satisfy the HasEdgeContainer");
    static_assert(
        !mesh::HasEdgeContainer<const VertexContainer>,
        "const VertexContainer does satisfy the HasEdgeContainer");
    static_assert(
        !mesh::HasEdgeContainer<VertexContainer&>,
        "VertexContainer& does satisfy the HasEdgeContainer");
    static_assert(
        !mesh::HasEdgeContainer<const VertexContainer&>,
        "const VertexContainer& does satisfy the HasEdgeContainer");
    static_assert(
        !mesh::HasEdgeContainer<VertexContainer&&>,
        "VertexContainer&& does satisfy the HasEdgeContainer");

    static_assert(
        !mesh::HasEdgeContainer<FaceContainer>,
        "FaceContainer does satisfy the HasEdgeContainer");
    static_assert(
        !mesh::HasEdgeContainer<const FaceContainer>,
        "const FaceContainer does satisfy the HasEdgeContainer");
    static_assert(
        !mesh::HasEdgeContainer<FaceContainer&>,
        "FaceContainer& does satisfy the HasEdgeContainer");
    static_assert(
        !mesh::HasEdgeContainer<const FaceContainer&>,
        "const FaceContainer& does satisfy the HasEdgeContainer");
    static_assert(
        !mesh::HasEdgeContainer<FaceContainer&&>,
        "FaceContainer&& does satisfy the HasEdgeContainer");

    static_assert(
        mesh::HasEdgeContainer<EdgeContainer>,
        "EdgeContainer does not satisfy the HasEdgeContainer");
    static_assert(
        mesh::HasEdgeContainer<const EdgeContainer>,
        "const EdgeContainer does not satisfy the HasEdgeContainer");
    static_assert(
        mesh::HasEdgeContainer<EdgeContainer&>,
        "EdgeContainer& does not satisfy the HasEdgeContainer");
    static_assert(
        mesh::HasEdgeContainer<const EdgeContainer&>,
        "const EdgeContainer& does not satisfy the HasEdgeContainer");
    static_assert(
        mesh::HasEdgeContainer<EdgeContainer&&>,
        "EdgeContainer&& does not satisfy the HasEdgeContainer");

    static_assert(
        HasEdges<EdgeContainer>,
        "EdgeContainer does not satisfy the HasEdges concept");
    static_assert(
        HasEdges<const EdgeContainer>,
        "const EdgeContainer does not satisfy the HasEdges concept");
    static_assert(
        HasEdges<EdgeContainer&>,
        "EdgeContainer& does not satisfy the HasEdges concept");
    static_assert(
        HasEdges<const EdgeContainer&>,
        "const EdgeContainer& does not satisfy the HasEdges concept");
    static_assert(
        HasEdges<EdgeContainer&&>,
        "EdgeContainer&& does not satisfy the HasEdges concept");

    static_assert(
        !HasEdges<VertexContainer>,
        "VertexContainer does satisfy the HasEdges concept");

    static_assert(
        !HasEdges<FaceContainer>,
        "FaceContainer does satisfy the HasEdges concept");

    static_assert(
        HasEdges<VertexContainer, FaceContainer, EdgeContainer>,
        "Multiple types do not satisfy the HasEdges concept");
}

#endif // CONT_EDGE_CONTAINER_H
