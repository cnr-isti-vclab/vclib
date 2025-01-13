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
