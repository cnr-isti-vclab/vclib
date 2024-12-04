/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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
