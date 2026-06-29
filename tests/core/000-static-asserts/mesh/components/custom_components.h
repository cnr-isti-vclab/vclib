// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_CUSTOM_COMPONENTS_H
#define COMP_CUSTOM_COMPONENTS_H

#include <vclib/meshes.h>

void customComponentsComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;

    using TmpTriMeshVertex = detail::tmpMesh::TriVertex;

    using CustomComponents = vert::CustomComponents<TriMeshVertex>;

    // test only the customComponent component
    static_assert(
        comp::HasCustomComponents<CustomComponents>,
        "CustomComponents does not satisfy the HasCustomComponents concept");
    static_assert(
        comp::HasCustomComponents<const CustomComponents>,
        "const CustomComponents does not satisfy the HasCustomComponents "
        "concept");
    static_assert(
        comp::HasCustomComponents<CustomComponents&>,
        "CustomComponents& does not satisfy the HasCustomComponents concept");
    static_assert(
        comp::HasCustomComponents<const CustomComponents&>,
        "const CustomComponents& does not satisfy the HasCustomComponents "
        "concept");
    static_assert(
        comp::HasCustomComponents<CustomComponents&&>,
        "CustomComponents&& does not satisfy the HasCustomComponents concept");

    static_assert(
        comp::HasCustomComponents<TriMeshVertex>,
        "TriMeshVertex does not satisfy the HasCustomComponents concept");
    static_assert(
        comp::HasCustomComponents<const TriMeshVertex>,
        "const TriMeshVertex does not satisfy the HasCustomComponents concept");
    static_assert(
        comp::HasCustomComponents<TriMeshVertex&>,
        "TriMeshVertex& does not satisfy the HasCustomComponents concept");
    static_assert(
        comp::HasCustomComponents<const TriMeshVertex&>,
        "const TriMeshVertex& does not satisfy the HasCustomComponents "
        "concept");
    static_assert(
        comp::HasCustomComponents<TriMeshVertex&&>,
        "TriMeshVertex&& does not satisfy the HasCustomComponents concept");

    static_assert(
        !comp::HasCustomComponents<TmpTriMeshVertex>,
        "TmpTriMeshVertex does satisfy the HasCustomComponents concept");
    static_assert(
        !comp::HasCustomComponents<const TmpTriMeshVertex>,
        "const TmpTriMeshVertex does satisfy the HasCustomComponents concept");
    static_assert(
        !comp::HasCustomComponents<TmpTriMeshVertex&>,
        "TmpTriMeshVertex& does satisfy the HasCustomComponents concept");
    static_assert(
        !comp::HasCustomComponents<const TmpTriMeshVertex&>,
        "const TmpTriMeshVertex& does satisfy the HasCustomComponents "
        "concept");
    static_assert(
        !comp::HasCustomComponents<TmpTriMeshVertex&&>,
        "TmpTriMeshVertex&& does satisfy the HasCustomComponents concept");

    static_assert(
        comp::HasCustomComponents<TriMesh>,
        "TriMesh does not satisfy the HasCustomComponents concept");
    static_assert(
        comp::HasCustomComponents<const TriMesh>,
        "const TriMesh does not satisfy the HasCustomComponents concept");
    static_assert(
        comp::HasCustomComponents<TriMesh&>,
        "TriMesh& does not satisfy the HasCustomComponents concept");
    static_assert(
        comp::HasCustomComponents<const TriMesh&>,
        "const TriMesh& does not satisfy the HasCustomComponents "
        "concept");
    static_assert(
        comp::HasCustomComponents<TriMesh&&>,
        "TriMesh&& does not satisfy the HasCustomComponents concept");
}

#endif // COMP_CUSTOM_COMPONENTS_H
