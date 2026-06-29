// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
        "const mesh::BoundingBox3d does not satisfy the HasBoundingBox "
        "concept");
    static_assert(
        comp::HasBoundingBox<mesh::BoundingBox3d&>,
        "mesh::BoundingBox3d& does not satisfy the HasBoundingBox concept");
    static_assert(
        comp::HasBoundingBox<const mesh::BoundingBox3d&>,
        "const mesh::BoundingBox3d& does not satisfy the HasBoundingBox "
        "concept");
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
