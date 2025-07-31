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

    static_assert(
        !EdgeConcept<TMFace>, "TMFace does satisfy the EdgeConcept");
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
