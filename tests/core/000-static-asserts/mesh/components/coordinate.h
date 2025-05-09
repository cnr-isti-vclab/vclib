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

#ifndef COMP_COORDINATE_H
#define COMP_COORDINATE_H

#include <vclib/meshes.h>

void coordinateComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;

    // test only the coordinate component
    static_assert(
        comp::HasPosition<vert::Position3f>,
        "vert::Coordinate3f does not satisfy the HasCoordinate concept");
    static_assert(
        comp::HasPosition<const vert::Position3f>,
        "const vert::Coordinate3f does not satisfy the HasCoordinate concept");
    static_assert(
        comp::HasPosition<vert::Position3f&>,
        "vert::Coordinate3f& does not satisfy the HasCoordinate concept");
    static_assert(
        comp::HasPosition<const vert::Position3f&>,
        "const vert::Coordinate3f& does not satisfy the HasCoordinate concept");
    static_assert(
        comp::HasPosition<vert::Position3f&&>,
        "vert::Coordinate3f&& does not satisfy the HasCoordinate concept");

    static_assert(
        comp::HasPosition<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasCoordinate concept");
    static_assert(
        comp::HasPosition<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasCoordinate concept");
    static_assert(
        comp::HasPosition<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasCoordinate concept");
    static_assert(
        comp::HasPosition<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasCoordinate concept");
    static_assert(
        comp::HasPosition<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasCoordinate concept");
}

#endif // COMP_COORDINATE_H
