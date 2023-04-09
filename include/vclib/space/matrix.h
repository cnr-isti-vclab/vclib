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

#ifndef VCL_SPACE_MATRIX_H
#define VCL_SPACE_MATRIX_H

#include <Eigen/Core>

#include <vclib/concepts/space/matrix.h>
#include <vclib/concepts/space/point.h>

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

template<EigenMatrixConcept MatrixType, PointConcept PointType>
PointType operator*(const MatrixType& m, const PointType& p)
	requires(
		MatrixType::RowsAtCompileTime == PointType::DIM &&
		MatrixType::ColsAtCompileTime == PointType::DIM);

} // namespace vcl

#include "matrix.cpp"

#endif // VCL_SPACE_MATRIX_H
