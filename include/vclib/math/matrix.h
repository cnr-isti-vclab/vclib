/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_MATH_MATRIX_H
#define VCL_MATH_MATRIX_H

#include <Eigen/Core>

namespace vcl {

template<typename Scalar>
using Matrix33 = Eigen::Matrix<Scalar, 3, 3>;

using Matrix33i = Matrix33<int>;
using Matrix33f = Matrix33<float>;
using Matrix33d = Matrix33<double>;

template<typename Scalar>
using Matrix44 = Eigen::Matrix<Scalar, 4, 4>;

using Matrix44i = Matrix44<int>;
using Matrix44f = Matrix44<float>;
using Matrix44d = Matrix44<double>;

template<typename MatrixType, typename PointType, typename ScalarType>
void setTransformMatrixRotation(
	MatrixType&       matrix,
	PointType         axis,
	const ScalarType& angleRad);

template<typename MatrixType, typename PointType, typename ScalarType>
void setTransformMatrixRotationDeg(
	MatrixType&       matrix,
	PointType         axis,
	const ScalarType& angleDeg);

template<typename ScalarType, typename PointType>
void setTrasformMatrixTranslation(Matrix44<ScalarType>& matrix, const PointType& translation);

template<typename ScalarType, typename PointType>
void setTrasformMatrixScale(Matrix44<ScalarType>& matrix, const PointType& scale);

template<typename MatrixType, typename PointType, typename ScalarType>
MatrixType rotationMatrix(const PointType& axis, const ScalarType& angleRad);

template<typename MatrixType, typename PointType, typename ScalarType>
MatrixType rotationMatrixDeg(const PointType& axis, const ScalarType& angleDeg);

} // namespace vcl

#include "matrix.cpp"

#endif // VCL_MATH_MATRIX_H
