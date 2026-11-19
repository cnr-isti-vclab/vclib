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

    using TriMeshFace  = FaceType<TriMesh>;

    using EdgeMeshEdge  = EdgeType<EdgeMesh>;

    using VRComponent = face::TriangleVertexReferences<>;

    // test only the vertex indices/pointers component
    static_assert(
        comp::HasVertexReferences<VRComponent>,
        "face::VertexReferences does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const VRComponent>,
        "const face::VertexReferences does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<VRComponent&>,
        "face::VertexReferences& does not satisfy the HasVertexReferences "
        "concept");
    static_assert(
        comp::HasVertexReferences<const VRComponent&>,
        "const face::VertexReferences& does not satisfy the "
        "HasVertexReferences concept");
    static_assert(
        comp::HasVertexReferences<VRComponent&&>,
        "face::VertexReferences&& does not satisfy the HasVertexReferences "
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
}

#endif // COMP_VERTEX_RENFERENCES_H
