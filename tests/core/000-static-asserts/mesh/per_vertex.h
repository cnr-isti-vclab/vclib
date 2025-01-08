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
