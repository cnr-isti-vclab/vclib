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
