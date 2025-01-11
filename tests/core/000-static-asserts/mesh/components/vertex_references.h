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

#ifndef COMP_VERTEX_RENFERENCES_H
#define COMP_VERTEX_RENFERENCES_H

#include <vclib/meshes.h>

void vertexReferencesComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex  = trimesh::Vertex<float, false>;
    using TriMeshVertexI = trimesh::Vertex<float, true>;

    using TriMeshFace  = trimesh::Face<float, false>;
    using TriMeshFaceI = trimesh::Face<float, true>;

    using EdgeMeshEdge  = edgemesh::Edge<float, false>;
    using EdgeMeshEdgeI = edgemesh::Edge<float, true>;

    using VIComponent = face::VertexIndices<TriMeshVertexI, 3, TriMeshFaceI>;
    using VPComponent = face::VertexPointers<TriMeshVertex, 3, TriMeshFace>;

    // test only the vertex indices/pointers component
    static_assert(
        comp::HasVertexReferences<VIComponent>,
        "face::VertexIndices does not satisfy the HasVertexReferences concept");
    static_assert(
        comp::HasVertexReferences<const VIComponent>,
        "const face::VertexIndices does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<VIComponent&>,
        "face::VertexIndices& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const VIComponent&>,
        "const face::VertexIndices& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<VIComponent&&>,
        "face::VertexIndices&& does not satisfy the HasVertexReferences "
        "concept");

    static_assert(
        comp::HasVertexReferences<VPComponent>,
        "face::VertexPointers does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const VPComponent>,
        "const face::VertexPointers does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<VPComponent&>,
        "face::VertexPointers& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const VPComponent&>,
        "const face::VertexPointers& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<VPComponent&&>,
        "face::VertexPointers&& does not satisfy the HasVertexReferences "
        "concept");

    static_assert(
        comp::HasVertexReferences<TriMeshFace>,
        "TriMesh Face does not satisfy the HasVertexReferences concept");
    static_assert(
        comp::HasVertexReferences<const TriMeshFace>,
        "const TriMesh Face does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<TriMeshFace&>,
        "TriMesh Face& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const TriMeshFace&>,
        "const TriMesh Face& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<TriMeshFace&&>,
        "TriMesh Face&& does not satisfy the HasVertexReferences "
        "concept");

    static_assert(
        comp::HasVertexReferences<TriMeshFaceI>,
        "Indexed TriMesh Face does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const TriMeshFaceI>,
        "Indexed const TriMesh Face does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<TriMeshFaceI&>,
        "Indexed TriMesh Face& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const TriMeshFaceI&>,
        "Indexed const TriMesh Face& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<TriMeshFaceI&&>,
        "Indexed TriMesh Face&& does not satisfy the HasVertexReferences "
        "concept");

    static_assert(
        comp::HasVertexReferences<EdgeMeshEdge>,
        "EdgeMesh Edge does not satisfy the HasVertexReferences concept");
    static_assert(
        comp::HasVertexReferences<const EdgeMeshEdge>,
        "const EdgeMesh Edge does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<EdgeMeshEdge&>,
        "EdgeMesh Edge& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const EdgeMeshEdge&>,
        "const EdgeMesh Edge& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<EdgeMeshEdge&&>,
        "EdgeMesh Edge&& does not satisfy the HasVertexReferences "
        "concept");

    static_assert(
        comp::HasVertexReferences<EdgeMeshEdgeI>,
        "Indexed EdgeMesh Edge does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const EdgeMeshEdgeI>,
        "Indexed const EdgeMesh Edge does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<EdgeMeshEdgeI&>,
        "Indexed EdgeMesh Edge& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const EdgeMeshEdgeI&>,
        "Indexed const EdgeMesh Edge& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<EdgeMeshEdgeI&&>,
        "Indexed EdgeMesh Edge&& does not satisfy the HasVertexReferences "
        "concept");
}

#endif // COMP_VERTEX_RENFERENCES_H
