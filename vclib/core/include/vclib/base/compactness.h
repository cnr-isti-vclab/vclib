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

#ifndef VCL_BASE_COMPACTNESS_H
#define VCL_BASE_COMPACTNESS_H

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

} // namespace vcl

#endif // VCL_BASE_COMPACTNESS_H
