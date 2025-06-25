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

#include <vclib/concepts/range.h>
#include <vclib/concepts/space/matrix.h>
#include <vclib/concepts/space/point.h>
#include <vclib/misc/parallel.h>
#include <vclib/space/core/matrix.h>

#include <cmath>

namespace vcl {

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
