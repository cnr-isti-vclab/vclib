// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_COLOR_H
#define COMP_COLOR_H

#include <vclib/meshes.h>

void colorComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;

    // test only the color component
    static_assert(
        comp::HasColor<vert::Color>,
        "vert::Color does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<const vert::Color>,
        "const vert::Color does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<vert::Color&>,
        "vert::Color& does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<const vert::Color&>,
        "const vert::Color& does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<vert::Color&&>,
        "vert::Color&& does not satisfy the HasColor concept");

    static_assert(
        comp::HasColor<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasColor concept");

    static_assert(
        comp::HasColor<TriMesh>,
        "TriMesh Vertex does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<const TriMesh>,
        "const TriMesh Vertex does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<TriMesh&>,
        "TriMesh Vertex& does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<const TriMesh&>,
        "const TriMesh Vertex& does not satisfy the HasColor concept");
    static_assert(
        comp::HasColor<TriMesh&&>,
        "TriMesh Vertex&& does not satisfy the HasColor concept");

    static_assert(
        comp::HasOptionalColor<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasOptionalColor concept");
    static_assert(
        comp::HasOptionalColor<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasOptionalColor concept");
    static_assert(
        comp::HasOptionalColor<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasOptionalColor concept");
    static_assert(
        comp::HasOptionalColor<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasOptionalColor concept");
    static_assert(
        comp::HasOptionalColor<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasOptionalColor concept");
}

#endif // COMP_COLOR_H
