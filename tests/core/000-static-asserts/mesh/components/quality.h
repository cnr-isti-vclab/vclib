// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_QUALITY_H
#define COMP_QUALITY_H

#include <vclib/meshes.h>

void qualityComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;

    // test only the quality component
    static_assert(
        comp::HasQuality<vert::Qualityf>,
        "vert::Qualityf does not satisfy the HasQuality concept");
    static_assert(
        comp::HasQuality<const vert::Qualityf>,
        "const vert::Qualityf does not satisfy the HasQuality concept");
    static_assert(
        comp::HasQuality<vert::Qualityf&>,
        "vert::Qualityf& does not satisfy the HasQuality concept");
    static_assert(
        comp::HasQuality<const vert::Qualityf&>,
        "const vert::Qualityf& does not satisfy the HasQuality concept");
    static_assert(
        comp::HasQuality<vert::Qualityf&&>,
        "vert::Qualityf&& does not satisfy the HasQuality concept");

    static_assert(
        comp::HasQuality<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasQuality concept");
    static_assert(
        comp::HasQuality<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasQuality concept");
    static_assert(
        comp::HasQuality<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasQuality concept");
    static_assert(
        comp::HasQuality<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasQuality concept");
    static_assert(
        comp::HasQuality<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasQuality concept");

    static_assert(
        comp::HasOptionalQuality<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasOptionalQuality concept");
    static_assert(
        comp::HasOptionalQuality<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasOptionalQuality concept");
    static_assert(
        comp::HasOptionalQuality<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasOptionalQuality concept");
    static_assert(
        comp::HasOptionalQuality<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasOptionalQuality "
        "concept");
    static_assert(
        comp::HasOptionalQuality<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasOptionalQuality concept");
}

#endif // COMP_QUALITY_H
