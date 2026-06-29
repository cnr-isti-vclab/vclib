// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_POSITION_H
#define COMP_POSITION_H

#include <vclib/meshes.h>

void positionComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;

    // test only the position component
    static_assert(
        comp::HasPosition<vert::Position3f>,
        "vert::Position3f does not satisfy the HasPosition concept");
    static_assert(
        comp::HasPosition<const vert::Position3f>,
        "const vert::Position3f does not satisfy the HasPosition concept");
    static_assert(
        comp::HasPosition<vert::Position3f&>,
        "vert::Position3f& does not satisfy the HasPosition concept");
    static_assert(
        comp::HasPosition<const vert::Position3f&>,
        "const vert::Position3f& does not satisfy the HasPosition concept");
    static_assert(
        comp::HasPosition<vert::Position3f&&>,
        "vert::Position3f&& does not satisfy the HasPosition concept");

    static_assert(
        comp::HasPosition<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasPosition concept");
    static_assert(
        comp::HasPosition<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasPosition concept");
    static_assert(
        comp::HasPosition<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasPosition concept");
    static_assert(
        comp::HasPosition<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasPosition concept");
    static_assert(
        comp::HasPosition<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasPosition concept");
}

#endif // COMP_POSITION_H
