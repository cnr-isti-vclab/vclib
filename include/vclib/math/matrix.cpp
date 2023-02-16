/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "matrix.h"

#include "base.h"

#include <vclib/space/point.h>

namespace vcl {

template<MatrixConcept MatrixType, PointConcept PointType>
PointType operator*(const MatrixType& m, const PointType& p)
{
	static_assert(MatrixType::RowsAtCompileTime == PointType::DIM, "");
	static_assert(MatrixType::ColsAtCompileTime == PointType::DIM, "");

	PointType res;
	for (uint i = 0; i < PointType::DIM; i++) {
		for (uint j = 0; j < PointType::DIM; j++) {
			res(i) += m(i,j)* p(i);
		}
	}
	return res;
}

/**
 * @brief Given an 3D axis and an angle expressed in randiants, fills the given matrix with a
 * transform matrix that represents the rotation matrix of the given axis/angle.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher than 3x3 (e.g. 4x4),
 * only the 3x3 submatrix will be set, leaving unchanged the other values.
 *
 * @param matrix
 * @param axis
 * @param angleRad
 * @return
 */
template<typename MatrixType, PointConcept PointType, typename ScalarType>
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
 * @brief Given an 3D axis and an angle expressed in degrees, fills the given matrix with a
 * transform matrix that represents the rotation matrix of the given axis/angle.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher than 3x3 (e.g. 4x4),
 * only the 3x3 submatrix will be set, leaving unchanged the other values.
 *
 * @param matrix
 * @param axis
 * @param angleDeg
 * @return
 */
template<typename MatrixType, PointConcept PointType, typename ScalarType>
void setTransformMatrixRotationDeg(
	MatrixType&       matrix,
	PointType         axis,
	const ScalarType& angleDeg)
{
	setTransformMatrixRotation(matrix, axis, vcl::toRad(angleDeg));
}

template<typename ScalarType, PointConcept PointType>
void setTrasformMatrixTranslation(Matrix44<ScalarType>& matrix, const PointType& translation)
{
	matrix(0, 3) = translation[0];
	matrix(1, 3) = translation[1];
	matrix(2, 3) = translation[2];
}

template<typename ScalarType, PointConcept PointType>
void setTrasformMatrixScale(Matrix44<ScalarType>& matrix, const PointType& scale)
{
	matrix(0, 0) = scale[0];
	matrix(1, 1) = scale[0];
	matrix(2, 2) = scale[2];
	matrix(3, 3) = 1;
}

/**
 * @brief Given an 3D axis and an angle expressed in randiants, returns a transform matrix that
 * represents the rotation matrix of the given axis/angle.
 *
 * The MatrixType must be at least a 3x3 matrix having the setIdentity() member function.
 * If the matrix is a higher than 3x3 (e.g. 4x4), only the 3x3 submatrix will be set, leaving the
 * identity values in the other cells of the matrix.
 *
 * @param axis
 * @param angleRad
 * @return
 */
template<typename MatrixType, PointConcept PointType, typename ScalarType>
MatrixType rotationMatrix(const PointType& axis, const ScalarType& angleRad)
{
	MatrixType matrix;
	matrix.setIdentity();
	setTransformMatrixRotation(matrix, axis, angleRad);
	return matrix;
}

/**
 * @brief Given an 3D axis and an angle expressed in degrees, fills the given matrix with a
 * transform matrix that represents the rotation matrix of the given axis/angle.
 *
 * The given matrix must be at least a 3x3 matrix. If the matrix is a higher than 3x3 (e.g. 4x4),
 * only the 3x3 submatrix will be set, leaving unchanged the other values.
 *
 * @param axis
 * @param angleDeg
 * @return
 */
template<typename MatrixType, PointConcept PointType, typename ScalarType>
MatrixType rotationMatrixDeg(const PointType& axis, const ScalarType& angleDeg)
{
	return rotationMatrix<MatrixType>(axis, vcl::toRad(angleDeg));
}

} // namespace vcl
