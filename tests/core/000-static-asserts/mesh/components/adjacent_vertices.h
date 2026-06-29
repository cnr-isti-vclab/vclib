// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
