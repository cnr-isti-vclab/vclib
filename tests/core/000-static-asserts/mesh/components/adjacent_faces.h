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

    using TriMeshVertex = trimesh::Vertex<float, false>;
    using TriMeshFace   = trimesh::Face<float, false>;
    using PolyMeshFaceI = polymesh::Face<float, true>;

    using ATPComponent = face::AdjacentTriangles<false, TriMeshFace>;
    using APIComponent = face::AdjacentPolygons<true, PolyMeshFaceI>;

    // test only the adjacent faces indices/pointers component
    static_assert(
        comp::HasAdjacentFaces<ATPComponent>,
        "face::AdjacentTriangles does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<const ATPComponent>,
        "const face::AdjacentTriangles does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<ATPComponent&>,
        "face::AdjacentTriangles& does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<const ATPComponent&>,
        "const face::AdjacentTriangles& does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<ATPComponent&&>,
        "face::AdjacentTriangles&& does not satisfy the HasAdjacentFaces "
        "concept");

    static_assert(
        comp::HasAdjacentFaces<APIComponent>,
        "face::AdjacentPolgons does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const APIComponent>,
        "const face::AdjacentPolgons does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<APIComponent&>,
        "face::AdjacentPolgons& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const APIComponent&>,
        "const face::AdjacentPolgons& does not satisfy the HasAdjacentFaces "
        "concept");
    static_assert(
        comp::HasAdjacentFaces<APIComponent&&>,
        "face::AdjacentPolgons&& does not satisfy the HasAdjacentFaces "
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
        comp::HasAdjacentFaces<PolyMeshFaceI>,
        "PolyMesh Face does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const PolyMeshFaceI>,
        "const PolyMesh Face does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<PolyMeshFaceI&>,
        "PolyMesh Face& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<const PolyMeshFaceI&>,
        "const PolyMesh Face& does not satisfy the HasAdjacentFaces concept");
    static_assert(
        comp::HasAdjacentFaces<PolyMeshFaceI&&>,
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
        comp::HasOptionalAdjacentFaces<PolyMeshFaceI>,
        "PolyMesh Face does not satisfy the HasOptionalAdjacentFaces concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<const PolyMeshFaceI>,
        "const PolyMesh Face does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<PolyMeshFaceI&>,
        "PolyMesh Face& does not satisfy the HasOptionalAdjacentFaces concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<const PolyMeshFaceI&>,
        "const PolyMesh Face& does not satisfy the HasOptionalAdjacentFaces "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentFaces<PolyMeshFaceI&&>,
        "PolyMesh Face&& does not satisfy the HasOptionalAdjacentFaces "
        "concept");
}

#endif // COMP_ADJACENT_FACES_H
