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

namespace vcl {

/**
 * @brief Multiplies a matrix and a point
 *
 * This function multiplies a matrix and a point and returns the resulting point.
 *
 * @tparam MatrixType: A type that satisfies the EigenMatrixConcept
 * @tparam PointType: A type that satisfies the PointConcept
 *
 * @param[in] m: The matrix to be multiplied with the point
 * @param[in] p: The point to be multiplied with the matrix
 * @return The resulting point after multiplying the matrix and point
 *
 * @requires MatrixType::RowsAtCompileTime == PointType::DIM &&
 *           MatrixType::ColsAtCompileTime == PointType::DIM
 */
template<EigenMatrixConcept MatrixType, PointConcept PointType>
PointType operator*(const MatrixType& m, const PointType& p)
	requires(
		MatrixType::RowsAtCompileTime == PointType::DIM &&
		MatrixType::ColsAtCompileTime == PointType::DIM)
{
	PointType res;
	for (uint i = 0; i < PointType::DIM; i++) {
		for (uint j = 0; j < PointType::DIM; j++) {
			res(i) += m(i,j)* p(i);
		}
	}
	return res;
}

} // namespace vcl
