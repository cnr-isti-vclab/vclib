// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef EDGE_COMPONENTS_H
#define EDGE_COMPONENTS_H

#include <vclib/meshes.h>

inline void edgeComponentsStaticAsserts()
{
    using namespace vcl;

    static_assert(
        HasEdgeAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeAdjacentEdges");
    static_assert(
        HasEdgeAdjacentEdges<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeAdjacentEdges");
    static_assert(
        HasEdgeAdjacentEdges<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeAdjacentEdges");
    static_assert(
        HasEdgeAdjacentEdges<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeAdjacentEdges");
    static_assert(
        HasEdgeAdjacentEdges<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeAdjacentEdges");

    static_assert(
        HasEdgeAdjacentFaces<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeAdjacentFaces");
    static_assert(
        HasEdgeAdjacentFaces<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeAdjacentFaces");
    static_assert(
        HasEdgeAdjacentFaces<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeAdjacentFaces");
    static_assert(
        HasEdgeAdjacentFaces<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeAdjacentFaces");
    static_assert(
        HasEdgeAdjacentFaces<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeAdjacentFaces");

    static_assert(
        HasEdgeOptionalAdjacentEdges<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeOptionalAdjacentEdges");

    static_assert(
        HasEdgeOptionalAdjacentFaces<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeOptionalAdjacentFaces");

    static_assert(
        HasEdgeColor<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeColor");
    static_assert(
        HasEdgeColor<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeColor");
    static_assert(
        HasEdgeColor<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeColor");
    static_assert(
        HasEdgeColor<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeColor");
    static_assert(
        HasEdgeColor<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeColor");

    static_assert(
        HasEdgeCustomComponents<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeCustomComponents");
    static_assert(
        HasEdgeCustomComponents<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeCustomComponents");
    static_assert(
        HasEdgeCustomComponents<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeCustomComponents");
    static_assert(
        HasEdgeCustomComponents<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeCustomComponents");
    static_assert(
        HasEdgeCustomComponents<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeCustomComponents");

    static_assert(
        HasEdgeNormal<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeNormal");
    static_assert(
        HasEdgeNormal<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeNormal");
    static_assert(
        HasEdgeNormal<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeNormal");
    static_assert(
        HasEdgeNormal<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeNormal");
    static_assert(
        HasEdgeNormal<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeNormal");

    static_assert(
        HasEdgeQuality<TriEdgeMesh>,
        "TriEdgeMesh does not satisfy the HasEdgeQuality");
    static_assert(
        HasEdgeQuality<const TriEdgeMesh>,
        "const TriEdgeMesh does not satisfy the HasEdgeQuality");
    static_assert(
        HasEdgeQuality<TriEdgeMesh&>,
        "TriEdgeMesh& does not satisfy the HasEdgeQuality");
    static_assert(
        HasEdgeQuality<const TriEdgeMesh&>,
        "const TriEdgeMesh& does not satisfy the HasEdgeQuality");
    static_assert(
        HasEdgeQuality<TriEdgeMesh&&>,
        "TriEdgeMesh&& does not satisfy the HasEdgeQuality");
}

#endif // EDGE_COMPONENTS_H
