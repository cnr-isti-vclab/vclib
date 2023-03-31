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

#ifndef VCL_MATH_TRANSFORM_H
#define VCL_MATH_TRANSFORM_H

#include <vclib/space/matrix.h>
#include <vclib/space/point.h>

namespace vcl {

template<typename MatrixType, PointConcept PointType, typename ScalarType>
void setTransformMatrixRotation(
	MatrixType&       matrix,
	PointType         axis,
	const ScalarType& angleRad);

template<typename MatrixType, PointConcept PointType, typename ScalarType>
void setTransformMatrixRotationDeg(
	MatrixType&       matrix,
	PointType         axis,
	const ScalarType& angleDeg);

template<typename ScalarType, PointConcept PointType>
void setTrasformMatrixTranslation(Matrix44<ScalarType>& matrix, const PointType& translation);

template<typename ScalarType, PointConcept PointType>
void setTrasformMatrixScale(Matrix44<ScalarType>& matrix, const PointType& scale);

template<typename MatrixType, PointConcept PointType, typename ScalarType>
MatrixType rotationMatrix(const PointType& axis, const ScalarType& angleRad);

template<typename MatrixType, PointConcept PointType, typename ScalarType>
MatrixType rotationMatrixDeg(const PointType& axis, const ScalarType& angleDeg);

} // namespace vcl

#include "transform.cpp"

#endif // VCL_MATH_TRANSFORM_H
