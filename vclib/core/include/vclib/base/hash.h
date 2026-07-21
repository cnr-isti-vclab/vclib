// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_HASH_H
#define VCL_BASE_HASH_H

#include <functional>

namespace vcl {

/**
 * @brief Starting from a seed, computes the hash of a series of objects.
 *
 * https://stackoverflow.com/a/57595105/5851101
 *
 * @param[in,out] seed: input seed and output hash
 * @param[in] v: first argument object
 * @param[in] rest: rest of the argument objects
 *
 * @ingroup base
 */
template<typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest)
{
    seed ^= std::hash<T> {}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

} // namespace vcl

#endif // VCL_BASE_HASH_H
