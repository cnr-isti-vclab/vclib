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

#ifndef COMP_BOUNDING_BOX_H
#define COMP_BOUNDING_BOX_H

#include <vclib/meshes.h>

void boundingBoxComponentStaticAsserts()
{
    using namespace vcl;

    // test only the bounding box component
    static_assert(
        comp::HasBoundingBox<mesh::BoundingBox3d>,
        "mesh::BoundingBox3d does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<const mesh::BoundingBox3d>,
        "const mesh::BoundingBox3d does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<mesh::BoundingBox3d&>,
        "mesh::BoundingBox3d& does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<const mesh::BoundingBox3d&>,
        "const mesh::BoundingBox3d& does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<mesh::BoundingBox3d&&>,
        "mesh::BoundingBox3d&& does not satisfy the HasBoundingBox concept");

    static_assert(
        comp::HasBoundingBox<TriMesh>,
        "TriMesh does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<const TriMesh>,
        "const TriMesh does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<TriMesh&>,
        "TriMesh& does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<const TriMesh&>,
        "const TriMesh& does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<TriMesh&&>,
        "TriMesh&& does not satisfy the HasBoundingBox concept");
}

#endif // COMP_BOUNDING_BOX_H
