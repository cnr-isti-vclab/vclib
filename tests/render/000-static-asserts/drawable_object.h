// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include <vclib/meshes.h>
#include <vclib/render/concepts/drawable_object.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <type_traits>

void drawableObjectStaticAsserts()
{
    using namespace vcl;

    static_assert(
        DrawableObjectConcept<DrawableMesh<TriMesh>>,
        "DrawableMesh does not satisfy the DrawableObjectConcept");
    static_assert(
        DrawableObjectConcept<const DrawableMesh<TriMesh>>,
        "const DrawableMesh does not satisfy the DrawableObjectConcept");
    static_assert(
        DrawableObjectConcept<DrawableMesh<TriMesh>&>,
        "DrawableMesh& does not satisfy the DrawableObjectConcept");
    static_assert(
        DrawableObjectConcept<const DrawableMesh<TriMesh>&>,
        "const DrawableMesh& does not satisfy the DrawableObjectConcept");
    static_assert(
        DrawableObjectConcept<DrawableMesh<TriMesh>&&>,
        "DrawableMesh&& does not satisfy the DrawableObjectConcept");

    static_assert(std::is_copy_constructible_v<DrawableMesh<TriMesh>>);
    static_assert(std::is_move_constructible_v<DrawableMesh<TriMesh>>);
    static_assert(std::is_copy_assignable_v<DrawableMesh<TriMesh>>);
    static_assert(std::is_move_assignable_v<DrawableMesh<TriMesh>>);
}

#endif // DRAWABLE_OBJECT_H
