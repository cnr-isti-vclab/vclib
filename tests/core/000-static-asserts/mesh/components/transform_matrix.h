// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
