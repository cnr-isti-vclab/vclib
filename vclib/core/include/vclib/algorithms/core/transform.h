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

#ifndef VCL_ALGORITHMS_CORE_TRANSFORM_H
#define VCL_ALGORITHMS_CORE_TRANSFORM_H

#include <vclib/space/core.h>

#include <cmath>

namespace vcl {

/**
 * @brief Given an 3D axis and an angle expressed in radiants, fills the given
 * matrix with a transform matrix that represents the rotation matrix of the
 * given axis/angle.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher
 * than 3x3 (e.g. 4x4), only the 3x3 submatrix will be set, leaving unchanged
 * the other values.
 *
 * @param matrix
 * @param axis
 * @param angleRad
 *
 * @ingroup algorithms_core
 */
template<MatrixConcept MatrixType, Point3Concept PointType, typename ScalarType>
void setTransformMatrixRotation(
    MatrixType&       matrix,
    PointType         axis,
    const ScalarType& angleRad)
{
    ScalarType c = std::cos(angleRad);
    ScalarType s = std::sin(angleRad);
    ScalarType q = 1 - c;
    axis.normalize();
    matrix(0, 0) = axis[0] * axis[0] * q + c;
    matrix(0, 1) = axis[0] * axis[1] * q - axis[2] * s;
    matrix(0, 2) = axis[0] * axis[2] * q + axis[1] * s;
    matrix(1, 0) = axis[1] * axis[0] * q + axis[2] * s;
    matrix(1, 1) = axis[1] * axis[1] * q + c;
    matrix(1, 2) = axis[1] * axis[2] * q - axis[0] * s;
    matrix(2, 0) = axis[2] * axis[0] * q - axis[1] * s;
    matrix(2, 1) = axis[2] * axis[1] * q + axis[0] * s;
    matrix(2, 2) = axis[2] * axis[2] * q + c;
}

/**
 * @brief Given an 3D axis and an angle expressed in degrees, fills the given
 * matrix with a transform matrix that represents the rotation matrix of the
 * given axis/angle.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher
 * than 3x3 (e.g. 4x4), only the 3x3 submatrix will be set, leaving unchanged
 * the other values.
 *
 * @param matrix
 * @param axis
 * @param angleDeg
 *
 * @ingroup algorithms_core
 */
template<MatrixConcept MatrixType, Point3Concept PointType, typename ScalarType>
void setTransformMatrixRotationDeg(
    MatrixType&       matrix,
    PointType         axis,
    const ScalarType& angleDeg)
{
    setTransformMatrixRotation(matrix, axis, toRad(angleDeg));
}

/**
 * @brief Given two 3D vectors, fills the given matrix with a transform matrix
 * that represents the rotation matrix from the first vector to the second
 * vector.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher
 * than 3x3 (e.g. 4x4), only the 3x3 submatrix will be set, leaving unchanged
 * the other values.
 *
 * @param matrix
 * @param fromVector
 * @param toVector
 *
 * @ingroup algorithms_core
 */
template<MatrixConcept MatrixType, Point3Concept PointType>
void setTransformMatrixRotation(
    MatrixType&      matrix,
    const PointType& fromVector,
    const PointType& toVector)
{
    if (fromVector == toVector) {
        matrix.block(0, 0, 3, 3).setIdentity();
        return;
    }
    PointType axis  = fromVector.cross(toVector);
    auto      angle = std::acos(
        fromVector.dot(toVector) / (fromVector.norm() * toVector.norm()));
    setTransformMatrixRotation(matrix, axis, angle);
}

template<MatrixConcept MatrixType, Point3Concept PointType>
void setTransformMatrixTranslation(
    MatrixType&      matrix,
    const PointType& translation)
{
    matrix(0, 3) = translation[0];
    matrix(1, 3) = translation[1];
    matrix(2, 3) = translation[2];
}

template<MatrixConcept MatrixType, typename ScalarType>
void setTransformMatrixScale(MatrixType& matrix, const ScalarType& scale)
{
    matrix(0, 0) = scale;
    matrix(1, 1) = scale;
    matrix(2, 2) = scale;
    matrix(3, 3) = 1;
}

template<MatrixConcept MatrixType, Point3Concept PointType>
void setTransformMatrixScale(MatrixType& matrix, const PointType& scale)
{
    matrix(0, 0) = scale[0];
    matrix(1, 1) = scale[1];
    matrix(2, 2) = scale[2];
    matrix(3, 3) = 1;
}

/**
 * @brief Given an 3D axis and an angle expressed in radiants, returns a
 * transform matrix that represents the rotation matrix of the given axis/angle.
 *
 * The MatrixType must be at least a 3x3 matrix having the setIdentity() member
 * function. If the matrix is a higher than 3x3 (e.g. 4x4), only the 3x3
 * submatrix will be set, leaving the identity values in the other cells of the
 * matrix.
 *
 * @param axis
 * @param angleRad
 * @return
 *
 * @ingroup algorithms_core
 */
template<MatrixConcept MatrixType, Point3Concept PointType, typename ScalarType>
MatrixType rotationMatrix(const PointType& axis, const ScalarType& angleRad)
{
    MatrixType matrix;
    matrix.setIdentity();
    setTransformMatrixRotation(matrix, axis, angleRad);
    return matrix;
}

/**
 * @brief Given an 3D axis and an angle expressed in degrees, fills the given
 * matrix with a transform matrix that represents the rotation matrix of the
 * given axis/angle.
 *
 * The MatrixType must be at least a 3x3 matrix having the setIdentity() member
 * function. If the matrix is a higher than 3x3 (e.g. 4x4), only the 3x3
 * submatrix will be set, leaving the identity values in the other cells of the
 * matrix.
 *
 * @param axis
 * @param angleDeg
 * @return
 *
 * @ingroup algorithms_core
 */
template<MatrixConcept MatrixType, Point3Concept PointType, typename ScalarType>
MatrixType rotationMatrixDeg(const PointType& axis, const ScalarType& angleDeg)
{
    return rotationMatrix<MatrixType>(axis, toRad(angleDeg));
}

/**
 * @brief Given two 3D vectors, returns a transform matrix that represents the
 * rotation matrix from the first vector to the second vector.
 *
 * The MatrixType must be at least a 3x3 matrix having the setIdentity() member
 * function. If the matrix is a higher than 3x3 (e.g. 4x4), only the 3x3
 * submatrix will be set, leaving the identity values in the other cells of the
 * matrix.
 *
 * @param fromVector
 * @param toVector
 * @return
 *
 * @ingroup algorithms_core
 */
template<MatrixConcept MatrixType, Point3Concept PointType>
MatrixType rotationMatrix(
    const PointType& fromVector,
    const PointType& toVector)
{
    MatrixType matrix;
    matrix.setIdentity();
    setTransformMatrixRotation(matrix, fromVector, toVector);
    return matrix;
}

/**
 * @brief Removes the scaling factors from the matrix in place.
 *
 * The input matrix is expected to be a 3x3 or 4x4 matrix, and the function
 * removes the scaling factors from the first three rows (or columns) of the
 * matrix. The scaling factors are computed as the Euclidean norm of the
 * first three rows of the matrix. The resulting matrix will have the same
 * orientation as the input matrix, but without scaling.
 *
 * @param[in/out] matrix: The input matrix from which the scaling factors will
 * be removed.
 *
 * @ingroup algorithms_core
 */
template<Matrix33Or44Concept MatrixType>
void removeScalingFromMatrixInPlace(MatrixType& matrix)
{
    using ScalarType  = typename MatrixType::Scalar;
    ScalarType scaleX = std::sqrt(
        matrix(0, 0) * matrix(0, 0) + matrix(0, 1) * matrix(0, 1) +
        matrix(0, 2) * matrix(0, 2));
    ScalarType scaleY = std::sqrt(
        matrix(1, 0) * matrix(1, 0) + matrix(1, 1) * matrix(1, 1) +
        matrix(1, 2) * matrix(1, 2));
    ScalarType scaleZ = std::sqrt(
        matrix(2, 0) * matrix(2, 0) + matrix(2, 1) * matrix(2, 1) +
        matrix(2, 2) * matrix(2, 2));
    for (int i = 0; i < 3; ++i) {
        matrix(0, i) /= scaleX;
        matrix(1, i) /= scaleY;
        matrix(2, i) /= scaleZ;
    }
}

/**
 * @brief Returns a matrix with the scaling factors removed.
 *
 * The input matrix is expected to be a 3x3 or 4x4 matrix, and the function
 * removes the scaling factors from the first three rows (or columns) of the
 * matrix. The scaling factors are computed as the Euclidean norm of the
 * first three rows of the matrix. The resulting matrix will have the same
 * orientation as the input matrix, but without scaling.
 *
 * @param[in] matrix: The input matrix from which the scaling factors will be
 * removed.
 * @return A matrix of the same type as the input matrix, but with the scaling
 * factors removed.
 *
 * @ingroup algorithms_core
 */
template<Matrix33Or44Concept MatrixType>
MatrixType removeScalingFromMatrix(const MatrixType& matrix)
{
    MatrixType result = matrix;
    removeScalingFromMatrixInPlace(result);
    return result;
}

template<Point3Concept PointType, Matrix33Concept MatrixType>
PointType multiplyNormalByMatrix(
    const PointType& normal,
    MatrixType       mat,
    bool             removeScalingFromMatrix = true)
{
    using ScalarType = typename PointType::Scalar;
    if (removeScalingFromMatrix) {
        removeScalingFromMatrixInPlace(mat);
    }
    return mat.template cast<ScalarType>() * normal;
}

template<Point3Concept PointType, Matrix44Concept MatrixType>
PointType multiplyNormalByMatrix(
    const PointType& normal,
    MatrixType       mat,
    bool             removeScalingFromMatrix = true)
{
    using ScalarType = typename PointType::Scalar;

    Matrix33<ScalarType> m33 =
        mat.template cast<ScalarType>().block(0, 0, 3, 3);
    if (removeScalingFromMatrix) {
        removeScalingFromMatrixInPlace(mat);
    }
    return mat * normal;
}

template<Range R, Matrix44Concept MatrixType>
void multiplyPointsByMatrix(R&& points, const MatrixType& mat)
    requires Point3Concept<std::ranges::range_value_t<R>>
{
    using PointType  = std::ranges::range_value_t<R>;
    using ScalarType = PointType::ScalarType;

    Matrix44<ScalarType> m44 = mat.template cast<ScalarType>();

    parallelFor(points, [&](auto& point) {
        point *= m44;
    });
}

template<Range R, Matrix33Concept MatrixType>
void multiplyNormalsByMatrix(
    R&&               normals,
    const MatrixType& mat,
    bool              removeScalingFromMatrix = true)
    requires Point3Concept<std::ranges::range_value_t<R>>
{
    using PointType  = std::ranges::range_value_t<R>;
    using ScalarType = PointType::ScalarType;

    Matrix33<ScalarType> m33 = mat.template cast<ScalarType>();

    if (removeScalingFromMatrix) {
        removeScalingFromMatrixInPlace(m33);
    }

    parallelFor(normals, [&](auto& normal) {
        normal = m33 * normal;
    });
}

template<Range R, Matrix44Concept MatrixType>
void multiplyNormalsByMatrix(
    R&&               normals,
    const MatrixType& mat,
    bool              removeScalingFromMatrix = true)
    requires Point3Concept<std::ranges::range_value_t<R>>
{
    using PointType  = std::ranges::range_value_t<R>;
    using ScalarType = PointType::ScalarType;

    Matrix33<ScalarType> m33 =
        mat.template cast<ScalarType>().block(0, 0, 3, 3);

    multiplyNormalsByMatrix(normals, m33, removeScalingFromMatrix);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_TRANSFORM_H
