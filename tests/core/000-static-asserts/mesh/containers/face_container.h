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

    using VertexContainer =
        mesh::VertexContainer<trimesh::Vertex<float, false>>;
    using FaceContainer = mesh::FaceContainer<trimesh::Face<float, false>>;
    using EdgeContainer = mesh::EdgeContainer<edgemesh::Edge<float, false>>;

    static_assert(
        !mesh::HasFaceContainer<VertexContainer>,
        "VertexContainer does satisfy the HasFaceContainer");
    static_assert(
        !mesh::HasFaceContainer<const VertexContainer>,
        "const VertexContainer does satisfy the HasFaceContainer");
    static_assert(
        !mesh::HasFaceContainer<VertexContainer&>,
        "VertexContainer& does satisfy the HasFaceContainer");
    static_assert(
        !mesh::HasFaceContainer<const VertexContainer&>,
        "const VertexContainer& does satisfy the HasFaceContainer");
    static_assert(
        !mesh::HasFaceContainer<VertexContainer&&>,
        "VertexContainer&& does satisfy the HasFaceContainer");

    static_assert(
        mesh::HasFaceContainer<FaceContainer>,
        "FaceContainer does not satisfy the HasFaceContainer");
    static_assert(
        mesh::HasFaceContainer<const FaceContainer>,
        "const FaceContainer does not satisfy the HasFaceContainer");
    static_assert(
        mesh::HasFaceContainer<FaceContainer&>,
        "FaceContainer& does not satisfy the HasFaceContainer");
    static_assert(
        mesh::HasFaceContainer<const FaceContainer&>,
        "const FaceContainer& does not satisfy the HasFaceContainer");
    static_assert(
        mesh::HasFaceContainer<FaceContainer&&>,
        "FaceContainer&& does not satisfy the HasFaceContainer");

    static_assert(
        !mesh::HasFaceContainer<EdgeContainer>,
        "EdgeContainer does satisfy the HasFaceContainer");
    static_assert(
        !mesh::HasFaceContainer<const EdgeContainer>,
        "const EdgeContainer does satisfy the HasFaceContainer");
    static_assert(
        !mesh::HasFaceContainer<EdgeContainer&>,
        "EdgeContainer& does satisfy the HasFaceContainer");
    static_assert(
        !mesh::HasFaceContainer<const EdgeContainer&>,
        "const EdgeContainer& does satisfy the HasFaceContainer");
    static_assert(
        !mesh::HasFaceContainer<EdgeContainer&&>,
        "EdgeContainer&& does satisfy the HasFaceContainer");

    static_assert(
        HasFaces<FaceContainer>,
        "FaceContainer does not satisfy the HasFaces concept");
    static_assert(
        HasFaces<const FaceContainer>,
        "const FaceContainer does not satisfy the HasFaces concept");
    static_assert(
        HasFaces<FaceContainer&>,
        "FaceContainer& does not satisfy the HasFaces concept");
    static_assert(
        HasFaces<const FaceContainer&>,
        "const FaceContainer& does not satisfy the HasFaces concept");
    static_assert(
        HasFaces<FaceContainer&&>,
        "FaceContainer&& does not satisfy the HasFaces concept");

    static_assert(
        !HasFaces<VertexContainer>,
        "VertexContainer does satisfy the HasFaces concept");

    static_assert(
        !HasFaces<EdgeContainer>,
        "EdgeContainer does satisfy the HasFaces concept");

    static_assert(
        HasFaces<VertexContainer, FaceContainer, EdgeContainer>,
        "Multiple types do not satisfy the HasFaces concept");
}

#endif // CONT_FACE_CONTAINER_H
