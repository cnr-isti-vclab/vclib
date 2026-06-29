// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
