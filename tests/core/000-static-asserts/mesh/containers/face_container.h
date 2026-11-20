// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef CONT_FACE_CONTAINER_H
#define CONT_FACE_CONTAINER_H

#include <vclib/meshes.h>

void faceContainerStaticAsserts()
{
    using namespace vcl;

    using VertexContainer = mesh::VertexContainer<trimesh::VertexComponents<>>;
    using FaceContainer   = mesh::FaceContainer<trimesh::FaceComponents<>>;
    using EdgeContainer   = mesh::EdgeContainer<edgemesh::EdgeComponents<>>;

    static_assert(
        !mesh::FaceContainerConcept<VertexContainer>,
        "VertexContainer does satisfy the FaceContainerConcept");
    static_assert(
        !mesh::FaceContainerConcept<const VertexContainer>,
        "const VertexContainer does satisfy the FaceContainerConcept");
    static_assert(
        !mesh::FaceContainerConcept<VertexContainer&>,
        "VertexContainer& does satisfy the FaceContainerConcept");
    static_assert(
        !mesh::FaceContainerConcept<const VertexContainer&>,
        "const VertexContainer& does satisfy the FaceContainerConcept");
    static_assert(
        !mesh::FaceContainerConcept<VertexContainer&&>,
        "VertexContainer&& does satisfy the FaceContainerConcept");

    static_assert(
        mesh::FaceContainerConcept<FaceContainer>,
        "FaceContainer does not satisfy the FaceContainerConcept");
    static_assert(
        mesh::FaceContainerConcept<const FaceContainer>,
        "const FaceContainer does not satisfy the FaceContainerConcept");
    static_assert(
        mesh::FaceContainerConcept<FaceContainer&>,
        "FaceContainer& does not satisfy the FaceContainerConcept");
    static_assert(
        mesh::FaceContainerConcept<const FaceContainer&>,
        "const FaceContainer& does not satisfy the FaceContainerConcept");
    static_assert(
        mesh::FaceContainerConcept<FaceContainer&&>,
        "FaceContainer&& does not satisfy the FaceContainerConcept");

    static_assert(
        !mesh::FaceContainerConcept<EdgeContainer>,
        "EdgeContainer does satisfy the FaceContainerConcept");
    static_assert(
        !mesh::FaceContainerConcept<const EdgeContainer>,
        "const EdgeContainer does satisfy the FaceContainerConcept");
    static_assert(
        !mesh::FaceContainerConcept<EdgeContainer&>,
        "EdgeContainer& does satisfy the FaceContainerConcept");
    static_assert(
        !mesh::FaceContainerConcept<const EdgeContainer&>,
        "const EdgeContainer& does satisfy the FaceContainerConcept");
    static_assert(
        !mesh::FaceContainerConcept<EdgeContainer&&>,
        "EdgeContainer&& does satisfy the FaceContainerConcept");
}

#endif // CONT_FACE_CONTAINER_H
