// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H

#include <vclib/space/complex.h>
#include <vclib/space/core.h>

namespace vcl::detail {

// given a buffer, returns a reference to the element at (i,j) considering the
// storage type (row-major or column-major)
inline auto& at(
    auto*             buffer,
    uint              i,
    uint              j,
    uint              numRows,
    uint              numCols,
    MatrixStorageType storage)
{
    if (storage == MatrixStorageType::ROW_MAJOR) {
        return buffer[i * numCols + j];
    }
    else {
        return buffer[j * numRows + i];
    }
}

inline TriPolyIndexBiMap indexMap;

} // namespace vcl::detail

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H
