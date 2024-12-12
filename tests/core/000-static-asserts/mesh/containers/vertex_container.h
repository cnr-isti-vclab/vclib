/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef CONT_VERTEX_CONTAINER_H
#define CONT_VERTEX_CONTAINER_H

#include <vclib/meshes.h>

void vertexContainerStaticAsserts()
{
    using namespace vcl;

    using VertexContainer =
        mesh::VertexContainer<trimesh::Vertex<float, false>>;
    using FaceContainer = mesh::FaceContainer<trimesh::Face<float, false>>;
    using EdgeContainer = mesh::EdgeContainer<edgemesh::Edge<float, false>>;

    static_assert(
        mesh::HasVertexContainer<VertexContainer>,
        "VertexContainer does not satisfy the HasVertexContainer");
    static_assert(
        mesh::HasVertexContainer<const VertexContainer>,
        "const VertexContainer does not satisfy the HasVertexContainer");
    static_assert(
        mesh::HasVertexContainer<VertexContainer&>,
        "VertexContainer& does not satisfy the HasVertexContainer");
    static_assert(
        mesh::HasVertexContainer<const VertexContainer&>,
        "const VertexContainer& does not satisfy the HasVertexContainer");
    static_assert(
        mesh::HasVertexContainer<VertexContainer&&>,
        "VertexContainer&& does not satisfy the HasVertexContainer");

    static_assert(
        !mesh::HasVertexContainer<FaceContainer>,
        "FaceContainer does satisfy the HasVertexContainer");
    static_assert(
        !mesh::HasVertexContainer<const FaceContainer>,
        "const FaceContainer does satisfy the HasVertexContainer");
    static_assert(
        !mesh::HasVertexContainer<FaceContainer&>,
        "FaceContainer& does satisfy the HasVertexContainer");
    static_assert(
        !mesh::HasVertexContainer<const FaceContainer&>,
        "const FaceContainer& does satisfy the HasVertexContainer");
    static_assert(
        !mesh::HasVertexContainer<FaceContainer&&>,
        "FaceContainer&& does satisfy the HasVertexContainer");

    static_assert(
        !mesh::HasVertexContainer<EdgeContainer>,
        "EdgeContainer does satisfy the HasVertexContainer");
    static_assert(
        !mesh::HasVertexContainer<const EdgeContainer>,
        "const EdgeContainer does satisfy the HasVertexContainer");
    static_assert(
        !mesh::HasVertexContainer<EdgeContainer&>,
        "EdgeContainer& does satisfy the HasVertexContainer");
    static_assert(
        !mesh::HasVertexContainer<const EdgeContainer&>,
        "const EdgeContainer& does satisfy the HasVertexContainer");
    static_assert(
        !mesh::HasVertexContainer<EdgeContainer&&>,
        "EdgeContainer&& does satisfy the HasVertexContainer");

    static_assert(
        HasVertices<VertexContainer>,
        "VertexContainer does not satisfy the HasVertices concept");
    static_assert(
        HasVertices<const VertexContainer>,
        "const VertexContainer does not satisfy the HasVertices concept");
    static_assert(
        HasVertices<VertexContainer&>,
        "VertexContainer& does not satisfy the HasVertices concept");
    static_assert(
        HasVertices<const VertexContainer&>,
        "const VertexContainer& does not satisfy the HasVertices concept");
    static_assert(
        HasVertices<VertexContainer&&>,
        "VertexContainer&& does not satisfy the HasVertices concept");

    static_assert(
        !HasVertices<EdgeContainer>,
        "VertexContainer does satisfy the HasVertices concept");

    static_assert(
        !HasVertices<FaceContainer>,
        "FaceContainer does satisfy the HasVertices concept");

    static_assert(
        HasVertices<VertexContainer, FaceContainer, EdgeContainer>,
        "Multiple types do not satisfy the HasVertices concept");
}

#endif // CONT_VERTEX_CONTAINER_H
