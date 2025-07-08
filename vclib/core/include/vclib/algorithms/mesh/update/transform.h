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

#ifndef VCL_ALGORITHMS_MESH_UPDATE_TRANSFORM_H
#define VCL_ALGORITHMS_MESH_UPDATE_TRANSFORM_H

#include <vclib/math/transform.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/core/matrix.h>

namespace vcl {

template<MeshConcept MeshType, typename ScalarM>
void applyTransformMatrix(
    MeshType&                mesh,
    const Matrix44<ScalarM>& matrix,
    bool                     updateNormals = true)
{
    using VertexType = MeshType::VertexType;

    multiplyPointsByMatrix(mesh.vertices() | vcl::views::positions, matrix);

    // TODO: automatize: for each element, check if it has normal and apply
    // the matrix to it
    if (updateNormals) {
        if constexpr (HasPerVertexNormal<MeshType>) {
            if (isPerVertexNormalAvailable(mesh)) {
                multiplyNormalsByMatrix(
                    mesh.vertices() | vcl::views::normals, matrix);
            }
        }
        if constexpr (HasPerFaceNormal<MeshType>) {
            if (isPerFaceNormalAvailable(mesh)) {
                multiplyNormalsByMatrix(
                    mesh.faces() | vcl::views::normals, matrix);
            }
        }
    }
}

template<MeshConcept MeshType, PointConcept PointType>
void translate(MeshType& mesh, const PointType& t)
{
    using VertexType = MeshType::VertexType;
    for (VertexType& v : mesh.vertices()) {
        v.position() += t;
    }
}

template<MeshConcept MeshType, PointConcept PointType>
void scale(MeshType& mesh, const PointType& s)
{
    using VertexType = MeshType::VertexType;
    for (VertexType& v : mesh.vertices()) {
        v.position()(0) *= s(0);
        v.position()(1) *= s(1);
        v.position()(2) *= s(2);
    }
}

template<MeshConcept MeshType, typename Scalar = double>
void scale(MeshType& mesh, const Scalar& s)
{
    using VertexType = MeshType::VertexType;
    for (VertexType& v : mesh.vertices()) {
        v.position() *= s;
    }
}

template<MeshConcept MeshType, typename Scalar>
void rotate(
    MeshType&               mesh,
    const Matrix33<Scalar>& m,
    bool                    updateNormals = true)
{
    for (auto& v : mesh.vertices()) {
        v.position() = m * v.position();
    }

    if (updateNormals) {
        if constexpr (HasPerVertexNormal<MeshType>) {
            if (isPerVertexNormalAvailable(mesh)) {
                for (auto& v : mesh.vertices()) {
                    v.normal() = m * v.normal();
                }
            }
        }

        if constexpr (HasPerFaceNormal<MeshType>) {
            if (isPerFaceNormalAvailable(mesh)) {
                for (auto& f : mesh.faces()) {
                    f.normal() = m * f.normal();
                }
            }
        }
    }
}

template<MeshConcept MeshType, PointConcept PointType, typename Scalar = double>
void rotate(
    MeshType&        mesh,
    const PointType& axis,
    const Scalar&    angleRad,
    bool             updateNormals = true)
{
    using ScalarType = MeshType::VertexType::PositionType::ScalarType;

    Matrix33<ScalarType> m;
    setTransformMatrixRotation(m, axis, angleRad);

    rotate(mesh, m, updateNormals);
}

template<MeshConcept MeshType, PointConcept PointType, typename Scalar = double>
void rotateDeg(
    MeshType&        mesh,
    const PointType& axis,
    const Scalar&    angleDeg,
    bool             updateNormals = true)
{
    rotate(mesh, axis, toRad(angleDeg), updateNormals);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_TRANSFORM_H
