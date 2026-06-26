// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef COMP_PRINCIPAL_CURVATURE_H
#define COMP_PRINCIPAL_CURVATURE_H

#include <vclib/meshes.h>

void principalCurvatureComponentStaticAsserts()
{
    using namespace vcl;

    using TriMeshVertex = trimesh::Vertex<float, true>;

    // test only the principalcurvature component
    static_assert(
        comp::HasPrincipalCurvature<vert::PrincipalCurvaturef>,
        "vert::PrincipalCurvaturef does not satisfy the HasPrincipalCurvature "
        "concept");
    static_assert(
        comp::HasPrincipalCurvature<const vert::PrincipalCurvaturef>,
        "const vert::PrincipalCurvaturef does not satisfy the "
        "HasPrincipalCurvature concept");
    static_assert(
        comp::HasPrincipalCurvature<vert::PrincipalCurvaturef&>,
        "vert::PrincipalCurvaturef& does not satisfy the HasPrincipalCurvature "
        "concept");
    static_assert(
        comp::HasPrincipalCurvature<const vert::PrincipalCurvaturef&>,
        "const vert::PrincipalCurvaturef& does not satisfy the "
        "HasPrincipalCurvature concept");
    static_assert(
        comp::HasPrincipalCurvature<vert::PrincipalCurvaturef&&>,
        "vert::PrincipalCurvaturef&& does not satisfy the "
        "HasPrincipalCurvature concept");

    static_assert(
        comp::HasPrincipalCurvature<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasPrincipalCurvature concept");
    static_assert(
        comp::HasPrincipalCurvature<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the HasPrincipalCurvature "
        "concept");
    static_assert(
        comp::HasPrincipalCurvature<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasPrincipalCurvature concept");
    static_assert(
        comp::HasPrincipalCurvature<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the HasPrincipalCurvature "
        "concept");
    static_assert(
        comp::HasPrincipalCurvature<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasPrincipalCurvature concept");

    static_assert(
        comp::HasOptionalPrincipalCurvature<TriMeshVertex>,
        "TriMesh Vertex does not satisfy the HasOptionalPrincipalCurvature "
        "concept");
    static_assert(
        comp::HasOptionalPrincipalCurvature<const TriMeshVertex>,
        "const TriMesh Vertex does not satisfy the "
        "HasOptionalPrincipalCurvature concept");
    static_assert(
        comp::HasOptionalPrincipalCurvature<TriMeshVertex&>,
        "TriMesh Vertex& does not satisfy the HasOptionalPrincipalCurvature "
        "concept");
    static_assert(
        comp::HasOptionalPrincipalCurvature<const TriMeshVertex&>,
        "const TriMesh Vertex& does not satisfy the "
        "HasOptionalPrincipalCurvature concept");
    static_assert(
        comp::HasOptionalPrincipalCurvature<TriMeshVertex&&>,
        "TriMesh Vertex&& does not satisfy the HasOptionalPrincipalCurvature "
        "concept");
}

#endif // COMP_PRINCIPAL_CURVATURE_H
