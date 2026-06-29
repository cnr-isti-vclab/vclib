// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef ELEM_EDGE_H
#define ELEM_EDGE_H

#include <vclib/meshes.h>

void edgeStaticAsserts()
{
    using namespace vcl;

    using EMEdge  = edgemesh::Edge<float, false>;
    using EMEdgeI = edgemesh::Edge<float, true>;
    using TMFace  = trimesh::Face<float, false>;

    static_assert(
        EdgeConcept<EMEdge>, "EMEdge does not satisfy the EdgeConcept");
    static_assert(
        EdgeConcept<const EMEdge>,
        "const EMEdge does not satisfy the EdgeConcept");
    static_assert(
        EdgeConcept<EMEdge&>, "EMEdge& does not satisfy the EdgeConcept");
    static_assert(
        EdgeConcept<const EMEdge&>,
        "const EMEdge& does not satisfy the EdgeConcept");
    static_assert(
        EdgeConcept<EMEdge&&>, "EMEdge&& does not satisfy the EdgeConcept");

    static_assert(
        EdgeConcept<EMEdgeI>, "EMEdgeI does not satisfy the EdgeConcept");
    static_assert(
        EdgeConcept<const EMEdgeI>,
        "const EMEdgeI does not satisfy the EdgeConcept");
    static_assert(
        EdgeConcept<EMEdgeI&>, "EMEdgeI& does not satisfy the EdgeConcept");
    static_assert(
        EdgeConcept<const EMEdgeI&>,
        "const EMEdgeI& does not satisfy the EdgeConcept");
    static_assert(
        EdgeConcept<EMEdgeI&&>, "EMEdgeI&& does not satisfy the EdgeConcept");

    static_assert(!EdgeConcept<TMFace>, "TMFace does satisfy the EdgeConcept");
    static_assert(
        !EdgeConcept<const TMFace>,
        "const TMFace does satisfy the EdgeConcept");
    static_assert(
        !EdgeConcept<TMFace&>, "TMFace& does satisfy the EdgeConcept");
    static_assert(
        !EdgeConcept<const TMFace&>,
        "const TMFace& does satisfy the EdgeConcept");
    static_assert(
        !EdgeConcept<TMFace&&>, "TMFace&& does satisfy the EdgeConcept");
}

#endif // ELEM_EDGE_H
