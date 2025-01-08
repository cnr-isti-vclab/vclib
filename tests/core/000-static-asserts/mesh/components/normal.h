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

#ifndef COMP_NORMAL_H
#define COMP_NORMAL_H

#include <vclib/meshes.h>

void normalComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;
    using EdgeMeshEdge  = edgemesh::Edge<float, true>;

    // test only the normal component
    static_assert(
        comp::HasNormal<vert::Normal3f>,
        "vert::Normal3f does not satisfy the HasNormal concept");
    static_assert(
        comp::HasNormal<const vert::Normal3f>,
        "const vert::Normal3f does not satisfy the HasNormal concept");
    static_assert(
        comp::HasNormal<vert::Normal3f&>,
        "vert::Normal3f& does not satisfy the HasNormal concept");
    static_assert(
        comp::HasNormal<const vert::Normal3f&>,
        "const vert::Normal3f& does not satisfy the HasNormal concept");
    static_assert(
        comp::HasNormal<vert::Normal3f&&>,
        "vert::Normal3f&& does not satisfy the HasNormal concept");

    static_assert(
        comp::HasNormal<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasNormal concept");
    static_assert(
        comp::HasNormal<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasNormal concept");
    static_assert(
        comp::HasNormal<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasNormal concept");
    static_assert(
        comp::HasNormal<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasNormal concept");
    static_assert(
        comp::HasNormal<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasNormal concept");

    static_assert(
        comp::HasOptionalNormal<EdgeMeshEdge>,
        "EdgeMesh Edge does not satisfy the HasOptionalNormal concept");
    static_assert(
        comp::HasOptionalNormal<const EdgeMeshEdge>,
        "const EdgeMesh Edge does not satisfy the HasOptionalNormal concept");
    static_assert(
        comp::HasOptionalNormal<EdgeMeshEdge&>,
        "EdgeMesh Edge& does not satisfy the HasOptionalNormal concept");
    static_assert(
        comp::HasOptionalNormal<const EdgeMeshEdge&>,
        "const EdgeMesh Edge& does not satisfy the HasOptionalNormal concept");
    static_assert(
        comp::HasOptionalNormal<EdgeMeshEdge&&>,
        "EdgeMesh Edge&& does not satisfy the HasOptionalNormal concept");
}

#endif // COMP_NORMAL_H
