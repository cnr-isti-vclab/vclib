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

    using VertexContainer = mesh::VertexContainer<trimesh::VertexComponents<>>;
    using FaceContainer   = mesh::FaceContainer<trimesh::FaceComponents<>>;
    using EdgeContainer   = mesh::EdgeContainer<edgemesh::EdgeComponents<>>;

    static_assert(
        !mesh::EdgeContainerConcept<VertexContainer>,
        "VertexContainer does satisfy the EdgeContainerConcept");
    static_assert(
        !mesh::EdgeContainerConcept<const VertexContainer>,
        "const VertexContainer does satisfy the EdgeContainerConcept");
    static_assert(
        !mesh::EdgeContainerConcept<VertexContainer&>,
        "VertexContainer& does satisfy the EdgeContainerConcept");
    static_assert(
        !mesh::EdgeContainerConcept<const VertexContainer&>,
        "const VertexContainer& does satisfy the EdgeContainerConcept");
    static_assert(
        !mesh::EdgeContainerConcept<VertexContainer&&>,
        "VertexContainer&& does satisfy the EdgeContainerConcept");

    static_assert(
        !mesh::EdgeContainerConcept<FaceContainer>,
        "FaceContainer does satisfy the EdgeContainerConcept");
    static_assert(
        !mesh::EdgeContainerConcept<const FaceContainer>,
        "const FaceContainer does satisfy the EdgeContainerConcept");
    static_assert(
        !mesh::EdgeContainerConcept<FaceContainer&>,
        "FaceContainer& does satisfy the EdgeContainerConcept");
    static_assert(
        !mesh::EdgeContainerConcept<const FaceContainer&>,
        "const FaceContainer& does satisfy the EdgeContainerConcept");
    static_assert(
        !mesh::EdgeContainerConcept<FaceContainer&&>,
        "FaceContainer&& does satisfy the EdgeContainerConcept");

    static_assert(
        mesh::EdgeContainerConcept<EdgeContainer>,
        "EdgeContainer does not satisfy the EdgeContainerConcept");
    static_assert(
        mesh::EdgeContainerConcept<const EdgeContainer>,
        "const EdgeContainer does not satisfy the EdgeContainerConcept");
    static_assert(
        mesh::EdgeContainerConcept<EdgeContainer&>,
        "EdgeContainer& does not satisfy the EdgeContainerConcept");
    static_assert(
        mesh::EdgeContainerConcept<const EdgeContainer&>,
        "const EdgeContainer& does not satisfy the EdgeContainerConcept");
    static_assert(
        mesh::EdgeContainerConcept<EdgeContainer&&>,
        "EdgeContainer&& does not satisfy the EdgeContainerConcept");
}

#endif // CONT_EDGE_CONTAINER_H
