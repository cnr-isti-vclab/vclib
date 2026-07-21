// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_NAME_H
#define COMP_NAME_H

#include <vclib/meshes.h>

void nameComponentStaticAsserts()
{
    using namespace vcl;

    // test only the name component
    static_assert(
        comp::HasName<mesh::Name>,
        "mesh::Name does not satisfy the HasName concept");
    static_assert(
        comp::HasName<const mesh::Name>,
        "const mesh::Name does not satisfy the HasName "
        "concept");
    static_assert(
        comp::HasName<mesh::Name&>,
        "mesh::Name& does not satisfy the HasName concept");
    static_assert(
        comp::HasName<const mesh::Name&>,
        "const mesh::Name& does not satisfy the HasName "
        "concept");
    static_assert(
        comp::HasName<mesh::Name&&>,
        "mesh::Name&& does not satisfy the HasName concept");

    static_assert(
        comp::HasName<TriMesh>, "TriMesh does not satisfy the HasName concept");
    static_assert(
        comp::HasName<const TriMesh>,
        "const TriMesh does not satisfy the HasName concept");
    static_assert(
        comp::HasName<TriMesh&>,
        "TriMesh& does not satisfy the HasName concept");
    static_assert(
        comp::HasName<const TriMesh&>,
        "const TriMesh& does not satisfy the HasName concept");
    static_assert(
        comp::HasName<TriMesh&&>,
        "TriMesh&& does not satisfy the HasName concept");
}

#endif // COMP_NAME_H
