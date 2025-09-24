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

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H

#include <vclib/space/core.h>

namespace vcl::detail {

// given a buffer, returns a reference to the element at (i,j) considering the
// storage type (row-major or column-major)
inline auto& at(
    auto*             buffer,
    uint              i,
    uint              j,
    uint              rowNum,
    uint              colNum,
    MatrixStorageType storage)
{
    if (storage == MatrixStorageType::ROW_MAJOR) {
        return buffer[i * colNum + j];
    }
    else {
        return buffer[j * rowNum + i];
    }
}

} // namespace vcl::detail

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H
