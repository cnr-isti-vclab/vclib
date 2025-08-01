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
