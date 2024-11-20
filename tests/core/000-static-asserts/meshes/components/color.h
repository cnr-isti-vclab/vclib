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
