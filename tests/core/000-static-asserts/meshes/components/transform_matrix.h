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

#ifndef COMP_TRANSFORM_MATRIX_H
#define COMP_TRANSFORM_MATRIX_H

#include <vclib/meshes.h>

void transformMatrixComponentStaticAsserts()
{
    using namespace vcl;

    // test only the transform matrix component
    static_assert(
        comp::HasTransformMatrix<mesh::TransformMatrixd>,
        "mesh::TransformMatrixd does not satisfy the HasTransformMatrix "
        "concept");
    static_assert(
        comp::HasTransformMatrix<const mesh::TransformMatrixd>,
        "const mesh::TransformMatrixd does not satisfy the HasTransformMatrix "
        "concept");
    static_assert(
        comp::HasTransformMatrix<mesh::TransformMatrixd&>,
        "mesh::TransformMatrixd& does not satisfy the HasTransformMatrix "
        "concept");
    static_assert(
        comp::HasTransformMatrix<const mesh::TransformMatrixd&>,
        "const mesh::TransformMatrixd& does not satisfy the HasTransformMatrix "
        "concept");
    static_assert(
        comp::HasTransformMatrix<mesh::TransformMatrixd&&>,
        "mesh::TransformMatrixd&& does not satisfy the HasTransformMatrix "
        "concept");

    static_assert(
        comp::HasTransformMatrix<TriMesh>,
        "TriMesh does not satisfy the HasTransformMatrix concept");
    static_assert(
        comp::HasTransformMatrix<const TriMesh>,
        "const TriMesh does not satisfy the HasTransformMatrix concept");
    static_assert(
        comp::HasTransformMatrix<TriMesh&>,
        "TriMesh& does not satisfy the HasTransformMatrix concept");
    static_assert(
        comp::HasTransformMatrix<const TriMesh&>,
        "const TriMesh& does not satisfy the HasTransformMatrix concept");
    static_assert(
        comp::HasTransformMatrix<TriMesh&&>,
        "TriMesh&& does not satisfy the HasTransformMatrix concept");
}

#endif // COMP_TRANSFORM_MATRIX_H
