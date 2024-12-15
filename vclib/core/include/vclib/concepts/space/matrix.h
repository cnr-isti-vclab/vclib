/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_CONCEPTS_SPACE_MATRIX_H
#define VCL_CONCEPTS_SPACE_MATRIX_H

#include "array.h"

#include <vclib/concepts/const_correctness.h>

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
 *
 * If the type `T` is non-const, it must also have the following members:
 * - `T::resize(std::size_t, std::size_t)`
 * - `T::conservativeResize(std::size_t, std::size_t)`
 *
 * @note The fact that an Eigen matrix has the two resize member functions does
 * not mean that it can be resized. For example, a matrix with fixed size cannot
 * be resized, but it has the two resize member functions, and calling them with
 * the same sizes of the matrix does not cause any error.
 */
template<typename T>
concept EigenMatrixConcept = requires (T&& obj) {
    typename RemoveRef<T>::Scalar;

    RemoveRef<T>();

    obj.RowsAtCompileTime;
    obj.ColsAtCompileTime;

    obj.rows();
    obj.cols();

    obj.operator()(std::size_t(), std::size_t());
    obj.operator()(std::size_t(), std::size_t());

    // non const requirements
    requires vcl::IsConst<T> || requires {
        obj.resize(std::size_t(), std::size_t());
        obj.conservativeResize(std::size_t(), std::size_t());
    };
};

/**
 * @brief Concept for 2D arrays (matrices). It is satisfied when `T` is a
 * matrix, no matter its sizes.
 *
 * The concept just checks that `T` is a resizable Eigen matrix or an Array2,
 * trough their respective concepts.
 */
template<typename T>
concept MatrixConcept = EigenMatrixConcept<T> || Array2Concept<T>;

/**
 * @brief Concept for 3x3 matrices.
 */
template<typename T>
concept Matrix33Concept =
    EigenMatrixConcept<T> && (RemoveRef<T>::RowsAtCompileTime == 3) &&
    (RemoveRef<T>::ColsAtCompileTime == 3);

/**
 * @brief Concept for 4x4 matrices.
 */
template<typename T>
concept Matrix44Concept =
    EigenMatrixConcept<T> && (RemoveRef<T>::RowsAtCompileTime == 4) &&
    (RemoveRef<T>::ColsAtCompileTime == 4);

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_MATRIX_H
