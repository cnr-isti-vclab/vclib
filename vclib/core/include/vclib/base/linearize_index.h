// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_LINEARIZE_INDEX_H
#define VCL_BASE_LINEARIZE_INDEX_H

#include <array>
#include <cstddef>

namespace vcl {

/**
 * @brief Linearizes an N-dimensional index into a flat 1D index using row-major
 * order.
 *
 * @tparam N: The number of dimensions.
 * @tparam T: The type of the sizes and indices (typically an unsigned integer
 * type).
 * @param[in] sizes: An array containing the size of each dimension.
 * @param[in] indices: An array containing the N-dimensional index.
 * @return The linearized 1D index.
 *
 * @ingroup base
 */
template<std::size_t N, typename T>
constexpr T linearizeIndex(
    const std::array<T, N>& sizes,
    const std::array<T, N>& indices)
{
    T ind = indices[0];
    for (std::size_t i = 1; i < N; ++i) {
        ind = ind * sizes[i] + indices[i];
    }
    return ind;
}

/**
 * @brief Linearizes an N-dimensional index into a flat 1D index using row-major
 * order.
 *
 * @tparam T: The type of the sizes and indices.
 * @param[in] sizes: A pointer to the sizes array.
 * @param[in] indices: A pointer to the indices array.
 * @param[in] N: The number of dimensions.
 * @return The linearized 1D index.
 *
 * @ingroup base
 */
template<typename T>
constexpr T linearizeIndex(const T* sizes, const T* indices, std::size_t N)
{
    T ind = indices[0];
    for (std::size_t i = 1; i < N; ++i) {
        ind = ind * sizes[i] + indices[i];
    }
    return ind;
}

/**
 * @brief Linearizes an N-dimensional index into a flat 1D index using row-major
 * order.
 *
 * This version uses variadic template arguments to allow passing sizes as
 * compile-time constants.
 *
 * @tparam Sizes: The sizes of each dimension.
 * @tparam Ts: The types of the indices.
 * @param[in] indices: The N-dimensional indices to linearize.
 * @return The linearized 1D index.
 *
 * @ingroup base
 */
template<auto Size, auto... Sizes, typename... Ts>
constexpr auto linearizeIndex(Ts... indices)
{
    static_assert(
        1 + sizeof...(Sizes) == sizeof...(Ts),
        "Number of sizes must match number of indices.");
    return linearizeIndex(std::array {Size, Sizes...}, std::array {indices...});
}

} // namespace vcl

#endif // VCL_BASE_LINEARIZE_INDEX_H
