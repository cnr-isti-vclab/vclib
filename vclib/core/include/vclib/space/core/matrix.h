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

#ifndef VCL_SPACE_CORE_MATRIX_H
#define VCL_SPACE_CORE_MATRIX_H

#include "matrix/affine.h"
#include "matrix/matrix.h"

namespace vcl {

/**
 * @brief Get the storage type of a matrix.
 *
 * @tparam MatrixType: The type of the matrix, it must satisfy the
 * MatrixConcept.
 * @return The storage type of the matrix.
 *
 * @ingroup space_core
 */
template<MatrixConcept MatrixType>
vcl::MatrixStorageType matrixStorageType()
{
    vcl::MatrixStorageType stg = vcl::MatrixStorageType::ROW_MAJOR;

    // Eigen matrices can be column major
    if constexpr (vcl::EigenMatrixConcept<MatrixType>) {
        if constexpr (!MatrixType::IsRowMajor) {
            stg = vcl::MatrixStorageType::COLUMN_MAJOR;
        }
    }

    return stg;
}

} // namespace vcl

#endif // VCL_SPACE_CORE_MATRIX_H
