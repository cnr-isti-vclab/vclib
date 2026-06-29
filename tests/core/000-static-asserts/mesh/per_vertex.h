// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef PER_VERTEX_H
#define PER_VERTEX_H

#include <vclib/meshes.h>

void perVertexStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasPerVertexAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerVertexAdjacentEdges");
    static_assert(
        HasPerVertexAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerVertexAdjacentEdges");
    static_assert(
        HasPerVertexAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerVertexAdjacentEdges");
    static_assert(
        HasPerVertexAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerVertexAdjacentEdges");
    static_assert(
        HasPerVertexAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerVertexAdjacentEdges");

    static_assert(
        HasPerVertexAdjacentFaces<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexAdjacentFaces");
    static_assert(
        HasPerVertexAdjacentFaces<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexAdjacentFaces");
    static_assert(
        HasPerVertexAdjacentFaces<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexAdjacentFaces");
    static_assert(
        HasPerVertexAdjacentFaces<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexAdjacentFaces");
    static_assert(
        HasPerVertexAdjacentFaces<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexAdjacentFaces");

    static_assert(
        HasPerVertexAdjacentVertices<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexAdjacentVertices");
    static_assert(
        HasPerVertexAdjacentVertices<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexAdjacentVertices");
    static_assert(
        HasPerVertexAdjacentVertices<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexAdjacentVertices");
    static_assert(
        HasPerVertexAdjacentVertices<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexAdjacentVertices");
    static_assert(
        HasPerVertexAdjacentVertices<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexAdjacentVertices");

    static_assert(
        HasPerVertexColor<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexColor");
    static_assert(
        HasPerVertexColor<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexColor");
    static_assert(
        HasPerVertexColor<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexColor");
    static_assert(
        HasPerVertexColor<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexColor");
    static_assert(
        HasPerVertexColor<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexColor");

    static_assert(
        HasPerVertexMark<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexMark");
    static_assert(
        HasPerVertexMark<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexMark");
    static_assert(
        HasPerVertexMark<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexMark");
    static_assert(
        HasPerVertexMark<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexMark");
    static_assert(
        HasPerVertexMark<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexMark");

    static_assert(
        HasPerVertexNormal<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexNormal");
    static_assert(
        HasPerVertexNormal<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexNormal");
    static_assert(
        HasPerVertexNormal<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexNormal");
    static_assert(
        HasPerVertexNormal<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexNormal");
    static_assert(
        HasPerVertexNormal<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexNormal");

    static_assert(
        HasPerVertexPrincipalCurvature<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexPrincipalCurvature");
    static_assert(
        HasPerVertexPrincipalCurvature<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexPrincipalCurvature");
    static_assert(
        HasPerVertexPrincipalCurvature<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexPrincipalCurvature");
    static_assert(
        HasPerVertexPrincipalCurvature<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexPrincipalCurvature");
    static_assert(
        HasPerVertexPrincipalCurvature<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexPrincipalCurvature");

    static_assert(
        HasPerVertexQuality<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexQuality");
    static_assert(
        HasPerVertexQuality<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexQuality");
    static_assert(
        HasPerVertexQuality<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexQuality");
    static_assert(
        HasPerVertexQuality<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexQuality");
    static_assert(
        HasPerVertexQuality<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexQuality");

    static_assert(
        HasPerVertexTexCoord<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexTexCoord");
    static_assert(
        HasPerVertexTexCoord<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexTexCoord");
    static_assert(
        HasPerVertexTexCoord<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexTexCoord");
    static_assert(
        HasPerVertexTexCoord<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexTexCoord");
    static_assert(
        HasPerVertexTexCoord<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexTexCoord");

    static_assert(
        HasPerVertexCustomComponents<TriMesh>,
        "TriMesh does not satisfy the HasPerVertexCustomComponents");
    static_assert(
        HasPerVertexCustomComponents<const TriMesh>,
        "const TriMesh does not satisfy the HasPerVertexCustomComponents");
    static_assert(
        HasPerVertexCustomComponents<TriMesh&>,
        "TriMesh& does not satisfy the HasPerVertexCustomComponents");
    static_assert(
        HasPerVertexCustomComponents<const TriMesh&>,
        "const TriMesh& does not satisfy the HasPerVertexCustomComponents");
    static_assert(
        HasPerVertexCustomComponents<TriMesh&&>,
        "TriMesh&& does not satisfy the HasPerVertexCustomComponents");
}

#endif // PER_VERTEX_H
