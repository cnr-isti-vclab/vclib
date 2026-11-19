// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_ADJACENT_FACES_H
#define COMP_ADJACENT_FACES_H

#include <vclib/meshes.h>

void adjacentFacesComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = VertexType<TriMesh>;
    using TriMeshFace   = FaceType<TriMesh>;
    using PolyMeshFace  = FaceType<PolyMesh>;

    using ATPComponent = face::TriangleAdjacentFaces<>;
    using APIComponent = face::PolygonAdjacentFaces<>;

    // test only the adjacent faces indices/pointers component
    static_assert(
        comp::HasAdjacentFaces<ATPComponent>,
        "face::TriangleAdjacentFaces does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<const ATPComponent>,
        "const face::TriangleAdjacentFaces does not satisfy the "
        "HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<ATPComponent&>,
        "face::TriangleAdjacentFaces& does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<const ATPComponent&>,
        "const face::TriangleAdjacentFaces& does not satisfy the "
        "HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<ATPComponent&&>,
        "face::TriangleAdjacentFaces&& does not satisfy the HasAdjacentFaces "
        "concept");

    static_assert(
        comp::HasAdjacentFaces<APIComponent>,
        "face::PolygonAdjacentFaces does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<const APIComponent>,
        "const face::PolygonAdjacentFaces does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<APIComponent&>,
        "face::PolygonAdjacentFaces& does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<const APIComponent&>,
        "const face::PolygonAdjacentFaces& does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<APIComponent&&>,
        "face::PolygonAdjacentFaces&& does not satisfy the HasAdjacentFaces "
        "concept");

    static_assert(
        comp::HasAdjacentFaces<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasAdjacentFaces concept");

    static_assert(
        comp::HasAdjacentFaces<TriMeshFace>,
        "TriMesh Face does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const TriMeshFace>,
        "const TriMesh Face does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<TriMeshFace&>,
        "TriMesh Face& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const TriMeshFace&>,
        "const TriMesh Face& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<TriMeshFace&&>,
        "TriMesh Face&& does not satisfy the HasAdjacentFaces concept");

    static_assert(
        comp::HasAdjacentFaces<PolyMeshFace>,
        "PolyMesh Face does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const PolyMeshFace>,
        "const PolyMesh Face does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<PolyMeshFace&>,
        "PolyMesh Face& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const PolyMeshFace&>,
        "const PolyMesh Face& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<PolyMeshFace&&>,
        "PolyMesh Face&& does not satisfy the HasAdjacentFaces concept");

    static_assert(
        comp::HasOptionalAdjacentFaces<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasOptionalAdjacentFaces concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasOptionalAdjacentFaces "
        "concept");

    static_assert(
        comp::HasOptionalAdjacentFaces<TriMeshFace>,
        "TriMesh Face does not satisfy the HasOptionalAdjacentFaces concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<const TriMeshFace>,
        "const TriMesh Face does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<TriMeshFace&>,
        "TriMesh Face& does not satisfy the HasOptionalAdjacentFaces concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<const TriMeshFace&>,
        "const TriMesh Face& does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<TriMeshFace&&>,
        "TriMesh Face&& does not satisfy the HasOptionalAdjacentFaces concept");

    static_assert(
        comp::HasOptionalAdjacentFaces<PolyMeshFace>,
        "PolyMesh Face does not satisfy the HasOptionalAdjacentFaces concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<const PolyMeshFace>,
        "const PolyMesh Face does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<PolyMeshFace&>,
        "PolyMesh Face& does not satisfy the HasOptionalAdjacentFaces concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<const PolyMeshFace&>,
        "const PolyMesh Face& does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<PolyMeshFace&&>,
        "PolyMesh Face&& does not satisfy the HasOptionalAdjacentFaces "
        "concept");
}

#endif // COMP_ADJACENT_FACES_H
