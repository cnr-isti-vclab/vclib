// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_PERMUTE_H
#define VCL_BASE_PERMUTE_H

#include <vclib/base/base.h>

#include <vector>

namespace vcl {

/**
 * @brief It will take care of compacting the vector vec, depending on the
 * content of the vector newIndices.
 *
 * Given the vector newIndices having the following features:
 * - has the same size of vec
 * - for each position i:
 *   - newIndices[i] contains the new position of the element vec[i] after the
 *     compactness
 *   - newIndices[i] contains the value UINT_NULL if the element vec[i] must be
 *     deleted
 * - the non-null elements of newIndices are unique and sorted in ascending
 *   order
 *
 * Non-null elements of newIndices must be unique, and their value must be less
 * than the new size of vec after the compactness. The new size of vec will be
 * the number of non-null elements of newIndices.
 *
 * @param vec
 * @param newIndices
 */
template<typename T, typename... Args>
void compactVector(
    std::vector<T, Args...>& vec,
    const std::vector<uint>& newIndices)
{
    assert(vec.size() == newIndices.size());
    uint newSize = 0;
    for (uint i = 0; i < newIndices.size(); ++i) {
        if (newIndices[i] != UINT_NULL) {
            ++newSize;
            if (newIndices[i] != i) {
                // must move the element from position i to position
                // newIndices[i]
                vec[newIndices[i]] = std::move(vec[i]);
            }
        }
    }
    vec.resize(newSize);
}

/**
 * @brief Permutes the elements of a vector in place according to a mapping of
 * new indices.
 *
 * This function rearranges the elements of the input vector such that the
 * element at position i is moved to position newIndices[i]. The permutation is
 * performed in-place using cycle detection to minimize memory usage and move
 * operations.
 *
 * The algorithm works by identifying cycles in the permutation. For each
 * cycle, it temporarily stores one element and performs a series of swaps to
 * move all elements in the cycle to their correct positions.
 *
 * @tparam T: The type of elements in the vector.
 * @tparam Args: Additional template arguments for the vector allocator.
 *
 * @param[in,out] vec: The vector to be permuted. After the function returns,
 * vec[newIndices[i]] will contain the original value of vec[i].
 * @param[in] newIndices: A vector of indices specifying the new position for
 * each element. Must have the same size as vec. Each value must be a valid
 * index in the range [0, vec.size()).
 *
 * @note The function assumes that newIndices represents a valid permutation
 * (i.e., each index appears exactly once).
 * @note Elements are moved using std::move for efficiency, so the type T should
 * support move semantics.
 *
 * Example:
 * @code
 * std::vector<int> vec = {1, 2, 3, 4};
 * std::vector<uint> newIndices = {2, 0, 3, 1};
 * permuteInPlace(vec, newIndices);
 * // Result: vec = {2, 4, 1, 3}
 * // (vec[0] moved to position 2, vec[1] moved to position 0, etc.)
 * @endcode
 */
template<typename T, typename... Args>
void permuteInPlace(
    std::vector<T, Args...>& vec,
    const std::vector<uint>& newIndices)
{
    using std::swap;

    assert(vec.size() == newIndices.size());

    std::vector<bool> visited(vec.size(), false);

    for (uint i = 0; i < vec.size(); ++i) {
        if (visited[i] || newIndices[i] == i) {
            continue;
        }

        uint current = i;
        T    temp    = std::move(vec[i]);

        while (!visited[current]) {
            visited[current] = true;
            int next         = newIndices[current];
            swap(temp, vec[next]);
            current = next;
        }
    }
}

} // namespace vcl

#endif // VCL_BASE_PERMUTE_H
