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

#ifndef CONT_EDGE_CONTAINER_H
#define CONT_EDGE_CONTAINER_H

#include <vclib/meshes.h>

void edgeContainerStaticAsserts()
{
    using namespace vcl;

    using VertexContainer =
        mesh::VertexContainer<trimesh::Vertex<float, false>>;
    using FaceContainer =
        mesh::FaceContainer<trimesh::Face<float, false>>;
    using EdgeContainer =
        mesh::EdgeContainer<edgemesh::Edge<float, false>>;

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
