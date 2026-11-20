// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H

#include <vclib/base.h>
#include <vclib/space/complex.h>
#include <vclib/space/core.h>

#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace vcl::detail {

// given a buffer, returns a reference to the element at (i,j) considering the
// storage type (row-major or column-major)
inline auto& at(
    auto*             buffer,
    auto              i,
    auto              j,
    auto              numRows,
    auto              numCols,
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

/**
 * @brief Returns the sentinel value for a given buffer or scalar value type.
 * - For signed integer types, this evaluates to -1.
 * - For floating point types, this evaluates to -1.0.
 * - For unsigned integer types, this evaluates to the maximum representable
 *   value (e.g. UINT_NULL / USHORT_NULL).
 */
template<typename BufferValueType>
constexpr BufferValueType bufferSentinel()
{
    return static_cast<BufferValueType>(-1);
}

/**
 * @brief Converts a buffer or matrix scalar value to a valid ushort
 * MaterialIndex, mapping sentinels (-1, negative values, USHORT_NULL, or
 * bufferSentinel<T>()) to USHORT_NULL safely without undefined behavior when
 * casting negative floating point values to unsigned types.
 */
template<typename BufferValueType>
constexpr ushort toMaterialIndex(BufferValueType val)
{
    using T = std::remove_cvref_t<BufferValueType>;
    if constexpr (std::is_floating_point_v<T>) {
        if (std::isnan(val) || val < 0 || val == static_cast<T>(USHORT_NULL)) {
            return USHORT_NULL;
        }
        return static_cast<ushort>(val);
    }
    else if constexpr (std::is_signed_v<T>) {
        if (val < 0 || val == static_cast<T>(USHORT_NULL)) {
            return USHORT_NULL;
        }
        return static_cast<ushort>(val);
    }
    else {
        if (val == USHORT_NULL || val == bufferSentinel<T>()) {
            return USHORT_NULL;
        }
        return static_cast<ushort>(val);
    }
}

/**
 * @brief Verifies that BufferValueType has sufficient capacity to store indices
 * in the range [0, elementCount - 1]. If elementCount cannot be represented
 * safely in BufferValueType without overflowing or colliding with the sentinel
 * value, throws a WrongSizeException.
 */
template<typename BufferValueType, typename SizeType>
void checkBufferIndexCapacity(SizeType elementCount)
{
    using ValueType = std::remove_cvref_t<BufferValueType>;
    if constexpr (std::integral<ValueType>) {
        if (elementCount > 0) {
            auto lastIndex = elementCount - 1;
            if constexpr (std::is_signed_v<ValueType>) {
                if (std::cmp_greater(
                        lastIndex, std::numeric_limits<ValueType>::max())) {
                    throw WrongSizeException(
                        "Buffer element type is too small to represent the "
                        "indices of the mesh.");
                }
            }
            else {
                if (std::cmp_greater_equal(
                        lastIndex, std::numeric_limits<ValueType>::max())) {
                    throw WrongSizeException(
                        "Buffer element type is too small to represent the "
                        "indices of the mesh.");
                }
            }
        }
    }
}

} // namespace vcl::detail

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_DETAIL_H
