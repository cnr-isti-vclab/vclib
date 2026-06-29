// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
