// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VERTEX_COMPONENTS_H
#define VERTEX_COMPONENTS_H

#include <vclib/meshes.h>

inline void vertexComponentsStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasVertexAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasVertexAdjacentEdges");
    static_assert(
        HasVertexAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasVertexAdjacentEdges");
    static_assert(
        HasVertexAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasVertexAdjacentEdges");
    static_assert(
        HasVertexAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasVertexAdjacentEdges");
    static_assert(
        HasVertexAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasVertexAdjacentEdges");

    static_assert(
        HasVertexAdjacentFaces<TriMesh>,
        "TriMesh does not satisfy the HasVertexAdjacentFaces");
    static_assert(
        HasVertexAdjacentFaces<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexAdjacentFaces");
    static_assert(
        HasVertexAdjacentFaces<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexAdjacentFaces");
    static_assert(
        HasVertexAdjacentFaces<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexAdjacentFaces");
    static_assert(
        HasVertexAdjacentFaces<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexAdjacentFaces");

    static_assert(
        HasVertexAdjacentVertices<TriMesh>,
        "TriMesh does not satisfy the HasVertexAdjacentVertices");
    static_assert(
        HasVertexAdjacentVertices<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexAdjacentVertices");
    static_assert(
        HasVertexAdjacentVertices<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexAdjacentVertices");
    static_assert(
        HasVertexAdjacentVertices<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexAdjacentVertices");
    static_assert(
        HasVertexAdjacentVertices<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexAdjacentVertices");

    static_assert(
        HasVertexOptionalAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasVertexOptionalAdjacentEdges");

    static_assert(
        HasVertexOptionalAdjacentFaces<TriMesh>,
        "TriMesh does not satisfy the HasVertexOptionalAdjacentFaces");

    static_assert(
        HasVertexOptionalAdjacentVertices<TriMesh>,
        "TriMesh does not satisfy the HasVertexOptionalAdjacentVertices");

    static_assert(
        HasVertexColor<TriMesh>,
        "TriMesh does not satisfy the HasVertexColor");
    static_assert(
        HasVertexColor<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexColor");
    static_assert(
        HasVertexColor<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexColor");
    static_assert(
        HasVertexColor<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexColor");
    static_assert(
        HasVertexColor<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexColor");

    static_assert(
        HasVertexNormal<TriMesh>,
        "TriMesh does not satisfy the HasVertexNormal");
    static_assert(
        HasVertexNormal<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexNormal");
    static_assert(
        HasVertexNormal<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexNormal");
    static_assert(
        HasVertexNormal<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexNormal");
    static_assert(
        HasVertexNormal<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexNormal");

    static_assert(
        HasVertexPrincipalCurvature<TriMesh>,
        "TriMesh does not satisfy the HasVertexPrincipalCurvature");
    static_assert(
        HasVertexPrincipalCurvature<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexPrincipalCurvature");
    static_assert(
        HasVertexPrincipalCurvature<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexPrincipalCurvature");
    static_assert(
        HasVertexPrincipalCurvature<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexPrincipalCurvature");
    static_assert(
        HasVertexPrincipalCurvature<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexPrincipalCurvature");

    static_assert(
        HasVertexQuality<TriMesh>,
        "TriMesh does not satisfy the HasVertexQuality");
    static_assert(
        HasVertexQuality<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexQuality");
    static_assert(
        HasVertexQuality<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexQuality");
    static_assert(
        HasVertexQuality<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexQuality");
    static_assert(
        HasVertexQuality<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexQuality");

    static_assert(
        HasVertexTexCoord<TriMesh>,
        "TriMesh does not satisfy the HasVertexTexCoord");
    static_assert(
        HasVertexTexCoord<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexTexCoord");
    static_assert(
        HasVertexTexCoord<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexTexCoord");
    static_assert(
        HasVertexTexCoord<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexTexCoord");
    static_assert(
        HasVertexTexCoord<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexTexCoord");

    static_assert(
        HasVertexCustomComponents<TriMesh>,
        "TriMesh does not satisfy the HasVertexCustomComponents");
    static_assert(
        HasVertexCustomComponents<const TriMesh>,
        "const TriMesh does not satisfy the HasVertexCustomComponents");
    static_assert(
        HasVertexCustomComponents<TriMesh&>,
        "TriMesh& does not satisfy the HasVertexCustomComponents");
    static_assert(
        HasVertexCustomComponents<const TriMesh&>,
        "const TriMesh& does not satisfy the HasVertexCustomComponents");
    static_assert(
        HasVertexCustomComponents<TriMesh&&>,
        "TriMesh&& does not satisfy the HasVertexCustomComponents");
}

#endif // VERTEX_COMPONENTS_H
