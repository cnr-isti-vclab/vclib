// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CONT_VERTEX_CONTAINER_H
#define CONT_VERTEX_CONTAINER_H

#include <vclib/meshes.h>

void vertexContainerStaticAsserts()
{
    using namespace vcl;

    using VertexContainer = mesh::VertexContainer<trimesh::VertexComponents<>>;
    using FaceContainer   = mesh::FaceContainer<trimesh::FaceComponents<>>;
    using EdgeContainer   = mesh::EdgeContainer<edgemesh::EdgeComponents<>>;

    static_assert(
        mesh::VertexContainerConcept<VertexContainer>,
        "VertexContainer does not satisfy the VertexContainerConcept");
    static_assert(
        mesh::VertexContainerConcept<const VertexContainer>,
        "const VertexContainer does not satisfy the VertexContainerConcept");
    static_assert(
        mesh::VertexContainerConcept<VertexContainer&>,
        "VertexContainer& does not satisfy the VertexContainerConcept");
    static_assert(
        mesh::VertexContainerConcept<const VertexContainer&>,
        "const VertexContainer& does not satisfy the VertexContainerConcept");
    static_assert(
        mesh::VertexContainerConcept<VertexContainer&&>,
        "VertexContainer&& does not satisfy the VertexContainerConcept");

    static_assert(
        !mesh::VertexContainerConcept<FaceContainer>,
        "FaceContainer does satisfy the VertexContainerConcept");
    static_assert(
        !mesh::VertexContainerConcept<const FaceContainer>,
        "const FaceContainer does satisfy the VertexContainerConcept");
    static_assert(
        !mesh::VertexContainerConcept<FaceContainer&>,
        "FaceContainer& does satisfy the VertexContainerConcept");
    static_assert(
        !mesh::VertexContainerConcept<const FaceContainer&>,
        "const FaceContainer& does satisfy the VertexContainerConcept");
    static_assert(
        !mesh::VertexContainerConcept<FaceContainer&&>,
        "FaceContainer&& does satisfy the VertexContainerConcept");

    static_assert(
        !mesh::VertexContainerConcept<EdgeContainer>,
        "EdgeContainer does satisfy the VertexContainerConcept");
    static_assert(
        !mesh::VertexContainerConcept<const EdgeContainer>,
        "const EdgeContainer does satisfy the VertexContainerConcept");
    static_assert(
        !mesh::VertexContainerConcept<EdgeContainer&>,
        "EdgeContainer& does satisfy the VertexContainerConcept");
    static_assert(
        !mesh::VertexContainerConcept<const EdgeContainer&>,
        "const EdgeContainer& does satisfy the VertexContainerConcept");
    static_assert(
        !mesh::VertexContainerConcept<EdgeContainer&&>,
        "EdgeContainer&& does satisfy the VertexContainerConcept");
}

#endif // CONT_VERTEX_CONTAINER_H
