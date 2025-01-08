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

#ifndef COMP_ADJACENT_EDGES_H
#define COMP_ADJACENT_EDGES_H

#include <vclib/meshes.h>

void adjacentEdgesComponentStaticAsserts()
{
    using namespace vcl;

    using EdgeMeshVertex = edgemesh::Vertex<float, false>;
    using EdgeMeshEdge   = edgemesh::Edge<float, false>;
    using EdgeMeshEdgeI  = edgemesh::Edge<float, true>;

    using AEComponent = edge::AdjacentEdges<false, EdgeMeshEdge>;

    // test only the adjacent Edges indices/pointers component
    static_assert(
        comp::HasAdjacentEdges<AEComponent>,
        "edge::AdjacentEdges does not satisfy the HasAdjacentEdges "
        "concept");
    static_assert(
        comp::HasAdjacentEdges<const AEComponent>,
        "const edge::AdjacentEdges does not satisfy the HasAdjacentEdges "
        "concept");
    static_assert(
        comp::HasAdjacentEdges<AEComponent&>,
        "edge::AdjacentEdges& does not satisfy the HasAdjacentEdges "
        "concept");
    static_assert(
        comp::HasAdjacentEdges<const AEComponent&>,
        "const edge::AdjacentEdges& does not satisfy the HasAdjacentEdges "
        "concept");
    static_assert(
        comp::HasAdjacentEdges<AEComponent&&>,
        "edge::AdjacentEdges&& does not satisfy the HasAdjacentEdges "
        "concept");

    static_assert(
        comp::HasAdjacentEdges<EdgeMeshVertex>,
        "EdgeMesh Vertex does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<const EdgeMeshVertex>,
        "const EdgeMesh Vertex does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<EdgeMeshVertex&>,
        "EdgeMesh Vertex& does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<const EdgeMeshVertex&>,
        "const EdgeMesh Vertex& does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<EdgeMeshVertex&&>,
        "EdgeMesh Vertex&& does not satisfy the HasAdjacentEdges concept");

    static_assert(
        comp::HasAdjacentEdges<EdgeMeshEdge>,
        "EdgeMesh Edge does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<const EdgeMeshEdge>,
        "const EdgeMesh Edge does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<EdgeMeshEdge&>,
        "EdgeMesh Edge& does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<const EdgeMeshEdge&>,
        "const EdgeMesh Edge& does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<EdgeMeshEdge&&>,
        "EdgeMesh Edge&& does not satisfy the HasAdjacentEdges concept");

    static_assert(
        comp::HasAdjacentEdges<EdgeMeshEdgeI>,
        "Indexed EdgeMesh Edge does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<const EdgeMeshEdgeI>,
        "Indexed const EdgeMesh Edge does not satisfy the HasAdjacentEdges "
        "concept");
    static_assert(
        comp::HasAdjacentEdges<EdgeMeshEdgeI&>,
        "Indexed EdgeMesh Edge& does not satisfy the HasAdjacentEdges concept");
    static_assert(
        comp::HasAdjacentEdges<const EdgeMeshEdgeI&>,
        "Indexed const EdgeMesh Edge& does not satisfy the HasAdjacentEdges "
        "concept");
    static_assert(
        comp::HasAdjacentEdges<EdgeMeshEdgeI&&>,
        "Indexed EdgeMesh Edge&& does not satisfy the HasAdjacentEdges "
        "concept");

    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshVertex>,
        "EdgeMesh Vertex does not satisfy the HasOptionalAdjacentEdges "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<const EdgeMeshVertex>,
        "const EdgeMesh Vertex does not satisfy the HasOptionalAdjacentEdges "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshVertex&>,
        "EdgeMesh Vertex& does not satisfy the HasOptionalAdjacentEdges "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<const EdgeMeshVertex&>,
        "const EdgeMesh Vertex& does not satisfy the HasOptionalAdjacentEdges "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshVertex&&>,
        "EdgeMesh Vertex&& does not satisfy the HasOptionalAdjacentEdges "
        "concept");

    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshEdge>,
        "EdgeMesh Edge does not satisfy the HasOptionalAdjacentEdges concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<const EdgeMeshEdge>,
        "const EdgeMesh Edge does not satisfy the HasOptionalAdjacentEdges "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshEdge&>,
        "EdgeMesh Edge& does not satisfy the HasOptionalAdjacentEdges concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<const EdgeMeshEdge&>,
        "const EdgeMesh Edge& does not satisfy the HasOptionalAdjacentEdges "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshEdge&&>,
        "EdgeMesh Edge&& does not satisfy the HasOptionalAdjacentEdges "
        "concept");

    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshEdgeI>,
        "EdgeMesh Edge does not satisfy the HasOptionalAdjacentEdges concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<const EdgeMeshEdgeI>,
        "const EdgeMesh Edge does not satisfy the HasOptionalAdjacentEdges "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshEdgeI&>,
        "EdgeMesh Edge& does not satisfy the HasOptionalAdjacentEdges concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<const EdgeMeshEdgeI&>,
        "const EdgeMesh Edge& does not satisfy the HasOptionalAdjacentEdges "
        "concept");
    static_assert(
        comp::HasOptionalAdjacentEdges<EdgeMeshEdgeI&&>,
        "EdgeMesh Edge&& does not satisfy the HasOptionalAdjacentEdges "
        "concept");
}

#endif // COMP_ADJACENT_EDGES_H
