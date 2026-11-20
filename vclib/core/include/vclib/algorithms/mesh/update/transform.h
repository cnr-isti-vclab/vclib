// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_TRANSFORM_H
#define VCL_ALGORITHMS_MESH_UPDATE_TRANSFORM_H

#include <vclib/mesh.h>
#include <vclib/space/core.h>

namespace vcl {

namespace detail {

template<MeshConcept MeshType, typename MatrixType>
void updateNormalsAndDirections(MeshType& mesh, const MatrixType& matrix)
{
    if constexpr (HasVertexNormal<MeshType>) {
        if (mesh.isVertexNormalAvailable()) {
            multiplyNormalsByMatrix(
                mesh.vertices() | vcl::views::normals, matrix);
        }
    }

    if constexpr (HasFaceNormal<MeshType>) {
        if (mesh.isFaceNormalAvailable()) {
            multiplyNormalsByMatrix(mesh.faces() | vcl::views::normals, matrix);
        }
    }

    if constexpr (HasEdgeNormal<MeshType>) {
        if (mesh.isEdgeNormalAvailable()) {
            multiplyNormalsByMatrix(mesh.edges() | vcl::views::normals, matrix);
        }
    }

    if constexpr (HasFaceWedgeNormals<MeshType>) {
        if (mesh.isFaceWedgeNormalsAvailable()) {
            for (auto f : mesh.faces()) {
                for (auto w : f.wedges()) {
                    w.normal() = multiplyNormalByMatrix(w.normal(), matrix);
                }
            }
        }
    }

    if constexpr (HasVertexTangent<MeshType>) {
        if (mesh.isVertexTangentAvailable()) {
            multiplyNormalsByMatrix(
                mesh.vertices() | vcl::views::tangents, matrix);

            Matrix33<double> m33;
            if constexpr (Matrix44Concept<MatrixType>) {
                m33 = matrix.template cast<double>().block(0, 0, 3, 3);
            }
            else {
                m33 = matrix.template cast<double>();
            }

            if (m33.determinant() < 0) {
                for (auto v : mesh.vertices()) {
                    v.tangentRightHanded() = !v.tangentRightHanded();
                }
            }
        }
    }

    if constexpr (HasVertexPrincipalCurvature<MeshType>) {
        if (mesh.isVertexPrincipalCurvatureAvailable()) {
            for (auto v : mesh.vertices()) {
                v.principalCurvature().maxDir() = multiplyNormalByMatrix(
                    v.principalCurvature().maxDir(), matrix);
                v.principalCurvature().minDir() = multiplyNormalByMatrix(
                    v.principalCurvature().minDir(), matrix);
            }
        }
    }
}

} // namespace detail

/**
 * @brief Applies a 4x4 transformation matrix to the mesh.
 *
 * This function multiplies the vertex positions of the mesh by the given 4x4
 * transformation matrix.
 *
 * If @p updateNormals is @c true, all directional and differential geometric
 * attributes present and available in the mesh are also updated using the
 * rotation part of the transformation matrix (with scaling removed to preserve
 * unit lengths):
 * - Vertex normals (@ref vcl::HasVertexNormal)
 * - Face normals (@ref vcl::HasFaceNormal)
 * - Edge normals (@ref vcl::HasEdgeNormal)
 * - Face wedge normals (@ref vcl::HasFaceWedgeNormals)
 * - Vertex tangents (@ref vcl::HasVertexTangent), including handedness flip if
 *   the transformation has negative determinant
 * - Vertex principal curvature directions (@ref
 * vcl::HasVertexPrincipalCurvature)
 *
 * @tparam MeshType: The type of the mesh, which must satisfy @ref
 * vcl::MeshConcept.
 * @tparam ScalarM: The scalar type of the transformation matrix.
 *
 * @param[in,out] mesh: The mesh to be transformed.
 * @param[in] matrix: The 4x4 transformation matrix to apply.
 * @param[in] updateNormals: Whether to update normals, tangents, and principal
 * curvature directions. Default is @c true.
 *
 * @ingroup update
 */
template<MeshConcept MeshType, typename ScalarM>
void applyTransformMatrix(
    MeshType&                mesh,
    const Matrix44<ScalarM>& matrix,
    bool                     updateNormals = true)
{
    multiplyPointsByMatrix(mesh.vertices() | vcl::views::positions, matrix);

    if (updateNormals) {
        detail::updateNormalsAndDirections(mesh, matrix);
    }
}

/**
 * @brief Translates the vertex positions of the mesh by a displacement vector.
 *
 * @tparam MeshType: The type of the mesh, which must satisfy @ref
 * vcl::MeshConcept.
 * @tparam PointType: The type of the displacement vector, which must satisfy
 * @ref vcl::PointConcept.
 *
 * @param[in,out] mesh: The mesh to be translated.
 * @param[in] t: The translation vector.
 *
 * @ingroup update
 */
template<MeshConcept MeshType, PointConcept PointType>
void translate(MeshType& mesh, const PointType& t)
{
    using Scalar = ScalarType<MeshType>;
    for (auto& p : mesh.vertices().positions()) {
        p += t.template cast<Scalar>();
    }
}

/**
 * @brief Scales the vertex positions of the mesh non-uniformly along each
 * coordinate axis.
 *
 * @tparam MeshType: The type of the mesh, which must satisfy @ref
 * vcl::MeshConcept.
 * @tparam PointType: The type of the scaling vector, which must satisfy @ref
 * vcl::PointConcept.
 *
 * @param[in,out] mesh: The mesh to be scaled.
 * @param[in] s: The scaling factors for x, y, and z coordinates.
 *
 * @ingroup update
 */
template<MeshConcept MeshType, PointConcept PointType>
void scale(MeshType& mesh, const PointType& s)
{
    for (auto& p : mesh.vertices().positions()) {
        p(0) *= s(0);
        p(1) *= s(1);
        p(2) *= s(2);
    }
}

/**
 * @brief Scales the vertex positions of the mesh uniformly by a scalar factor.
 *
 * @tparam MeshType: The type of the mesh, which must satisfy @ref
 * vcl::MeshConcept.
 * @tparam Scalar: The scalar type of the scale factor.
 *
 * @param[in,out] mesh: The mesh to be scaled.
 * @param[in] s: The uniform scale factor.
 *
 * @ingroup update
 */
template<MeshConcept MeshType, typename Scalar = double>
void scale(MeshType& mesh, const Scalar& s)
{
    for (auto& p : mesh.vertices().positions()) {
        p *= s;
    }
}

/**
 * @brief Rotates the mesh using a 3x3 rotation matrix.
 *
 * This function rotates the vertex positions of the mesh by multiplying them
 * with the given 3x3 rotation matrix. The matrix scalar type is automatically
 * cast to the scalar type of the mesh, allowing e.g. rotating a
 * single-precision mesh with a double-precision rotation matrix.
 *
 * If @p updateNormals is @c true, all directional and differential geometric
 * attributes present and available in the mesh are also rotated:
 * - Vertex normals (@ref vcl::HasVertexNormal)
 * - Face normals (@ref vcl::HasFaceNormal)
 * - Edge normals (@ref vcl::HasEdgeNormal)
 * - Face wedge normals (@ref vcl::HasFaceWedgeNormals)
 * - Vertex tangents (@ref vcl::HasVertexTangent), including handedness flip if
 *   the rotation matrix has negative determinant
 * - Vertex principal curvature directions (@ref
 * vcl::HasVertexPrincipalCurvature)
 *
 * @tparam MeshType: The type of the mesh, which must satisfy @ref
 * vcl::MeshConcept.
 * @tparam Scalar: The scalar type of the rotation matrix.
 *
 * @param[in,out] mesh: The mesh to be rotated.
 * @param[in] m: The 3x3 rotation matrix.
 * @param[in] updateNormals: Whether to update normals, tangents, and principal
 * curvature directions. Default is @c true.
 *
 * @ingroup update
 */
template<MeshConcept MeshType, typename Scalar>
void rotate(
    MeshType&               mesh,
    const Matrix33<Scalar>& m,
    bool                    updateNormals = true)
{
    using ScalarType            = ScalarType<MeshType>;
    Matrix33<ScalarType> rotMat = m.template cast<ScalarType>();

    for (auto&& v : mesh.vertices()) {
        v.position() = rotMat * v.position();
    }

    if (updateNormals) {
        detail::updateNormalsAndDirections(mesh, rotMat);
    }
}

/**
 * @brief Rotates the mesh around an arbitrary axis by an angle in radians.
 *
 * If @p updateNormals is @c true, all directional and differential geometric
 * attributes present and available in the mesh are also rotated (normals,
 * wedge normals, tangents, and principal curvature directions).
 *
 * @tparam MeshType: The type of the mesh, which must satisfy @ref
 * vcl::MeshConcept.
 * @tparam PointType: The type of the axis vector, which must satisfy @ref
 * vcl::PointConcept.
 * @tparam Scalar: The scalar type of the angle.
 *
 * @param[in,out] mesh: The mesh to be rotated.
 * @param[in] axis: The axis of rotation.
 * @param[in] angleRad: The rotation angle in radians.
 * @param[in] updateNormals: Whether to update normals, tangents, and principal
 * curvature directions. Default is @c true.
 *
 * @ingroup update
 */
template<MeshConcept MeshType, PointConcept PointType, typename Scalar = double>
void rotate(
    MeshType&        mesh,
    const PointType& axis,
    const Scalar&    angleRad,
    bool             updateNormals = true)
{
    using ScalarType = ScalarType<MeshType>;

    Matrix33<ScalarType> m;
    setTransformMatrixRotation(m, axis, angleRad);

    rotate(mesh, m, updateNormals);
}

/**
 * @brief Rotates the mesh around an arbitrary axis by an angle in degrees.
 *
 * If @p updateNormals is @c true, all directional and differential geometric
 * attributes present and available in the mesh are also rotated (normals,
 * wedge normals, tangents, and principal curvature directions).
 *
 * @tparam MeshType: The type of the mesh, which must satisfy @ref
 * vcl::MeshConcept.
 * @tparam PointType: The type of the axis vector, which must satisfy @ref
 * vcl::PointConcept.
 * @tparam Scalar: The scalar type of the angle.
 *
 * @param[in,out] mesh: The mesh to be rotated.
 * @param[in] axis: The axis of rotation.
 * @param[in] angleDeg: The rotation angle in degrees.
 * @param[in] updateNormals: Whether to update normals, tangents, and principal
 * curvature directions. Default is @c true.
 *
 * @ingroup update
 */
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
