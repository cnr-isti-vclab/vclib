// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_MARK_H
#define COMP_MARK_H

#include <vclib/meshes.h>

void markComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;

    // test only the mark component
    static_assert(
        comp::HasMark<vert::Mark>,
        "vert::Mark does not satisfy the HasMark concept");
    static_assert(
        comp::HasMark<const vert::Mark>,
        "const vert::Mark does not satisfy the HasMark concept");
    static_assert(
        comp::HasMark<vert::Mark&>,
        "vert::Mark& does not satisfy the HasMark concept");
    static_assert(
        comp::HasMark<const vert::Mark&>,
        "const vert::Mark& does not satisfy the HasMark concept");
    static_assert(
        comp::HasMark<vert::Mark&&>,
        "vert::Mark&& does not satisfy the HasMark concept");

    static_assert(
        comp::HasMark<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasMark concept");
    static_assert(
        comp::HasMark<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasMark concept");
    static_assert(
        comp::HasMark<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasMark concept");
    static_assert(
        comp::HasMark<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasMark concept");
    static_assert(
        comp::HasMark<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasMark concept");

    static_assert(
        comp::HasOptionalMark<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasOptionalMark concept");
    static_assert(
        comp::HasOptionalMark<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasOptionalMark concept");
    static_assert(
        comp::HasOptionalMark<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasOptionalMark concept");
    static_assert(
        comp::HasOptionalMark<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasOptionalMark concept");
    static_assert(
        comp::HasOptionalMark<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasOptionalMark concept");
}

#endif // COMP_MARK_H
