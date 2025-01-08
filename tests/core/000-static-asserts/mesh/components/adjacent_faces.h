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
