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

#ifndef PER_EDGE_H
#define PER_EDGE_H

#include <vclib/meshes.h>

void perEdgeStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasPerEdgeAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeAdjacentEdges");
    static_assert(
        HasPerEdgeAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeAdjacentEdges");
    static_assert(
        HasPerEdgeAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeAdjacentEdges");
    static_assert(
        HasPerEdgeAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeAdjacentEdges");
    static_assert(
        HasPerEdgeAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeAdjacentEdges");

    static_assert(
        HasPerEdgeAdjacentFaces<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeAdjacentFaces");
    static_assert(
        HasPerEdgeAdjacentFaces<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeAdjacentFaces");
    static_assert(
        HasPerEdgeAdjacentFaces<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeAdjacentFaces");
    static_assert(
        HasPerEdgeAdjacentFaces<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeAdjacentFaces");
    static_assert(
        HasPerEdgeAdjacentFaces<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeAdjacentFaces");

    static_assert(
        HasPerEdgeBitFlags<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeBitFlags");
    static_assert(
        HasPerEdgeBitFlags<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeBitFlags");
    static_assert(
        HasPerEdgeBitFlags<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeBitFlags");
    static_assert(
        HasPerEdgeBitFlags<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeBitFlags");
    static_assert(
        HasPerEdgeBitFlags<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeBitFlags");

    static_assert(
        HasPerEdgeColor<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeColor");
    static_assert(
        HasPerEdgeColor<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeColor");
    static_assert(
        HasPerEdgeColor<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeColor");
    static_assert(
        HasPerEdgeColor<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeColor");
    static_assert(
        HasPerEdgeColor<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeColor");

    static_assert(
        HasPerEdgeCustomComponents<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeCustomComponents");
    static_assert(
        HasPerEdgeCustomComponents<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeCustomComponents");
    static_assert(
        HasPerEdgeCustomComponents<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeCustomComponents");
    static_assert(
        HasPerEdgeCustomComponents<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeCustomComponents");
    static_assert(
        HasPerEdgeCustomComponents<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeCustomComponents");

    static_assert(
        HasPerEdgeMark<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeMark");
    static_assert(
        HasPerEdgeMark<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeMark");
    static_assert(
        HasPerEdgeMark<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeMark");
    static_assert(
        HasPerEdgeMark<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeMark");
    static_assert(
        HasPerEdgeMark<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeMark");

    static_assert(
        HasPerEdgeNormal<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeNormal");
    static_assert(
        HasPerEdgeNormal<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeNormal");
    static_assert(
        HasPerEdgeNormal<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeNormal");
    static_assert(
        HasPerEdgeNormal<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeNormal");
    static_assert(
        HasPerEdgeNormal<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeNormal");

    static_assert(
        HasPerEdgeQuality<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeQuality");
    static_assert(
        HasPerEdgeQuality<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeQuality");
    static_assert(
        HasPerEdgeQuality<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeQuality");
    static_assert(
        HasPerEdgeQuality<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeQuality");
    static_assert(
        HasPerEdgeQuality<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeQuality");

    static_assert(
        HasPerEdgeVertexReferences<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasPerEdgeVertexReferences");
    static_assert(
        HasPerEdgeVertexReferences<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasPerEdgeVertexReferences");
    static_assert(
        HasPerEdgeVertexReferences<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasPerEdgeVertexReferences");
    static_assert(
        HasPerEdgeVertexReferences<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasPerEdgeVertexReferences");
    static_assert(
        HasPerEdgeVertexReferences<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasPerEdgeVertexReferences");
}

#endif // PER_EDGE_H
