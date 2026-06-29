// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_TEX_COORD_H
#define COMP_TEX_COORD_H

#include <vclib/meshes.h>

void texCoordComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;

    // test only the texcoord component
    static_assert(
        comp::HasTexCoord<vert::TexCoordf>,
        "vert::TexCoordf does not satisfy the HasTexCoord concept");
    static_assert(
        comp::HasTexCoord<const vert::TexCoordf>,
        "const vert::TexCoordf does not satisfy the HasTexCoord concept");
    static_assert(
        comp::HasTexCoord<vert::TexCoordf&>,
        "vert::TexCoordf& does not satisfy the HasTexCoord concept");
    static_assert(
        comp::HasTexCoord<const vert::TexCoordf&>,
        "const vert::TexCoordf& does not satisfy the HasTexCoord concept");
    static_assert(
        comp::HasTexCoord<vert::TexCoordf&&>,
        "vert::TexCoordf&& does not satisfy the HasTexCoord concept");

    static_assert(
        comp::HasTexCoord<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasTexCoord concept");
    static_assert(
        comp::HasTexCoord<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasTexCoord concept");
    static_assert(
        comp::HasTexCoord<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasTexCoord concept");
    static_assert(
        comp::HasTexCoord<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasTexCoord concept");
    static_assert(
        comp::HasTexCoord<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasTexCoord concept");

    static_assert(
        comp::HasOptionalTexCoord<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasOptionalTexCoord concept");
    static_assert(
        comp::HasOptionalTexCoord<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasOptionalTexCoord "
        "concept");
    static_assert(
        comp::HasOptionalTexCoord<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasOptionalTexCoord concept");
    static_assert(
        comp::HasOptionalTexCoord<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasOptionalTexCoord "
        "concept");
    static_assert(
        comp::HasOptionalTexCoord<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasOptionalTexCoord concept");
}

#endif // COMP_TEX_COORD_H
