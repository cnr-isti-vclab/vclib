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
