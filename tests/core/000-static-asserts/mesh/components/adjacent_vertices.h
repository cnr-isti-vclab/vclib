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

#ifndef COMP_ADJACENT_VERTICES_H
#define COMP_ADJACENT_VERTICES_H

#include <vclib/meshes.h>

void adjacentVerticesComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex  = trimesh::Vertex<float, false>;
    using TriMeshVertexI = trimesh::Vertex<float, true>;

    using AVPComponent = vert::AdjacentVertices<false, TriMeshVertex>;
    using AVIComponent = vert::AdjacentVertices<true, TriMeshVertexI>;

    // test only the adjacent vertices indices/pointers component
    static_assert(
        comp::HasAdjacentVertices<AVPComponent>,
        "vert::AdjacentVertices does not satisfy the HasAdjacentVertices "
        "concept");
    static_assert(
        comp::HasAdjacentVertices<const AVPComponent>,
        "const vert::AdjacentVertices does not satisfy the HasAdjacentVertices "
        "concept");
    static_assert(
        comp::HasAdjacentVertices<AVPComponent&>,
        "vert::AdjacentVertices& does not satisfy the HasAdjacentVertices "
        "concept");
    static_assert(
        comp::HasAdjacentVertices<const AVPComponent&>,
        "const vert::AdjacentVertices& does not satisfy the "
        "HasAdjacentVertices concept");
    static_assert(
        comp::HasAdjacentVertices<AVPComponent&&>,
        "vert::AdjacentVertices&& does not satisfy the HasAdjacentVertices "
        "concept");

    static_assert(
        comp::HasAdjacentVertices<AVIComponent>,
        "Indexed vert::AdjacentVertices does not satisfy the "
        "HasAdjacentVertices concept");
    static_assert(
        comp::HasAdjacentVertices<const AVIComponent>,
        "Indexed const vert::AdjacentVertices does not satisfy the "
        "HasAdjacentVertices concept");
    static_assert(
        comp::HasAdjacentVertices<AVIComponent&>,
        "Indexed vert::AdjacentVertices& does not satisfy the "
        "HasAdjacentVertices concept");
    static_assert(
        comp::HasAdjacentVertices<const AVIComponent&>,
        "Indexed const vert::AdjacentVertices& does not satisfy the "
        "HasAdjacentVertices concept");
    static_assert(
        comp::HasAdjacentVertices<AVIComponent&&>,
        "Indexed vert::AdjacentVertices&& does not satisfy the "
        "HasAdjacentVertices concept");

    static_assert(
        comp::HasAdjacentVertices<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasAdjacentVertices concept");
    static_assert(
        comp::HasAdjacentVertices<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasAdjacentVertices "
        "concept");
    static_assert(
        comp::HasAdjacentVertices<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasAdjacentVertices concept");
    static_assert(
        comp::HasAdjacentVertices<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasAdjacentVertices "
        "concept");
    static_assert(
        comp::HasAdjacentVertices<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasAdjacentVertices concept");

    static_assert(
        comp::HasAdjacentVertices<TriMeshVertexI>,
        "Indexed TriMesh Vertex does not satisfy the HasAdjacentVertices "
        "concept");
    static_assert(
        comp::HasAdjacentVertices<const TriMeshVertexI>,
        "Indexed const TriMesh Vertex does not satisfy the HasAdjacentVertices "
        "concept");
    static_assert(
        comp::HasAdjacentVertices<TriMeshVertexI&>,
        "Indexed TriMesh Vertex& does not satisfy the HasAdjacentVertices "
        "concept");
    static_assert(
        comp::HasAdjacentVertices<const TriMeshVertexI&>,
        "Indexed const TriMesh Vertex& does not satisfy the "
        "HasAdjacentVertices concept");
    static_assert(
        comp::HasAdjacentVertices<TriMeshVertexI&&>,
        "Indexed TriMesh Vertex&& does not satisfy the HasAdjacentVertices "
        "concept");

    static_assert(
        comp::HasOptionalAdjacentVertices<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasOptionalAdjacentVertices "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentVertices<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasOptionalAdjacentVertices "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentVertices<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasOptionalAdjacentVertices "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentVertices<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the "
        "HasOptionalAdjacentVertices concept");
    static_assert(
        comp::HasOptionalAdjacentVertices<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasOptionalAdjacentVertices "
        "concept");

    static_assert(
        comp::HasOptionalAdjacentVertices<TriMeshVertexI>,
        "Indexed TriMesh Vertex does not satisfy the "
        "HasOptionalAdjacentVertices concept");
    static_assert(
        comp::HasOptionalAdjacentVertices<const TriMeshVertexI>,
        "Indexed const TriMesh Vertex does not satisfy the "
        "HasOptionalAdjacentVertices concept");
    static_assert(
        comp::HasOptionalAdjacentVertices<TriMeshVertexI&>,
        "Indexed TriMesh Vertex& does not satisfy the "
        "HasOptionalAdjacentVertices concept");
    static_assert(
        comp::HasOptionalAdjacentVertices<const TriMeshVertexI&>,
        "Indexed const TriMesh Vertex& does not satisfy the "
        "HasOptionalAdjacentVertices concept");
    static_assert(
        comp::HasOptionalAdjacentVertices<TriMeshVertexI&&>,
        "Indexed TriMesh Vertex&& does not satisfy the "
        "HasOptionalAdjacentVertices concept");
}

#endif // COMP_ADJACENT_VERTICES_H
