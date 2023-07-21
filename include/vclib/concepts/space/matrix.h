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

#ifndef VCL_CONCEPTS_SPACE_MATRIX_H
#define VCL_CONCEPTS_SPACE_MATRIX_H

#include <vclib/types.h>

namespace vcl {

/**
 * @brief Concept for Eigen matrices. It is satisfied when `T` is an Eigen
 * matrix.
 *
 * The concept just checks that `T` has the following members:
 * - `T::RowsAtCompileTime`
 * - `T::ColsAtCompileTime`
 */
template<typename T>
concept EigenMatrixConcept = requires (T o, const T& co)
{
	o.RowsAtCompileTime;
	o.ColsAtCompileTime;
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_MATRIX_H
