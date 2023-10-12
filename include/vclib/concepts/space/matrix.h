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

#include "array.h"

namespace vcl {

/**
 * @brief Concept for Eigen matrices. It is satisfied when `T` is an Eigen
 * matrix.
 *
 * The concept just checks that `T` has the following members:
 * - `T::RowsAtCompileTime`
 * - `T::ColsAtCompileTime`
 * - `T::rows()`
 * - `T::cols()`
 * - `T::operator()(std::size_t, std::size_t)`
 */
template<typename T>
concept EigenMatrixConcept = requires (T o, const T& co) {
    // clang-format off
    typename T::Scalar;

    co.RowsAtCompileTime;
    co.ColsAtCompileTime;

    co.rows();
    co.cols();

    o.operator()(std::size_t(), std::size_t());
    co.operator()(std::size_t(), std::size_t());
    // clang-format on
};

/**
 * @brief Concept for Eigen matrices that can be resized. It is satisfied when
 * `T` is an Eigen matrix.
 *
 * The concept just checks that `T` is an Eigen matrix and has the following
 * members:
 * - `T::resize()`
 * - `T::conservativeResize()`
 *
 * @note The fact that an Eigen matrix has the two methods above does not mean
 * that it can be resized. For example, a matrix with fixed size cannot be
 * resized, but it has the two methods above, and calling them with the same
 * sizes of the matrix does not cause any error.
 */
template<typename T>
concept ResizableEigenMatrixConceipt = EigenMatrixConcept<T> && requires (T o) {
    // clang-format off
    o.resize(std::size_t(), std::size_t());
    o.conservativeResize(std::size_t(), std::size_t());
    // clang-format on
};

/**
 * @brief Concept for 2D arrays (matrices). It is satisfied when `T` is a
 * matrix, no matter its sizes.
 *
 * The concept just checks that `T` is an resizable Eigen matrix or an Array2,
 * trough their respective concepts.
 */
template<typename T>
concept MatrixConcept = ResizableEigenMatrixConceipt<T> || Array2Concept<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_MATRIX_H
