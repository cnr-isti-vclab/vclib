// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
