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

#ifndef VCL_MISC_PARALLEL_H
#define VCL_MISC_PARALLEL_H

#include <algorithm>

#include <vclib/concepts/range.h>
#include <vclib/types.h>

// Apple clang does not support c++17 parallel algorithms.
// To compensate this lack, waiting for Apple to support them, we use pstld
// (https://github.com/mikekazakov/pstld) that implements them in the stl
// namespace
#if defined(__clang__) && defined(__APPLE__)
#if __has_include(<pstld/pstld.h>)
#include <pstld/pstld.h>
#else // if pstld is not found, include the header only
// inclusion for usage of vclib without cmake
#define PSTLD_HEADER_ONLY   // no prebuilt library, only the header
#define PSTLD_HACK_INTO_STD // hack into std namespace
#include "../../../external/pstld-master/pstld/pstld.h"
#endif // __has_include(<pstld/pstld.h>)
#else // if not apple clang
// tbb and qt conflicts: if both are linked, we need to first undef Qt's
// emit - see: https://github.com/oneapi-src/oneTBB/issues/547
#ifndef Q_MOC_RUN
#if defined(emit)
#undef emit
#include <execution>
#define emit // restore the macro definition of "emit", as it was
             // defined in gtmetamacros.h
#else // if emit is not defined
#include <execution>
#endif // emit
#endif // Q_MOC_RUN
#endif // defined(__clang__) && defined(__APPLE__)

namespace vcl {

/**
 * @brief This function executes a parallel for over the elements
 * iterated between `begin` and `end` iterators, if parallel requirements have
 * been found in the system.
 *
 * Example of usage on a vcl::Mesh, iterating over vertices:
 *
 * @code{.cpp}
 * vcl::parallelFor(m.vertices().begin(), m.vertices().end(),
 *     [&](VertexType& v) {
 *         // make some computing on v
 *     });
 * @endcode
 *
 * @param[in] begin: iterator of the first element to iterate
 * @param[in] end: iterator of the end of the iterated container
 * @param[in] F: lambda function that takes the iterated type as input
 */
template<typename Iterator, typename Lambda>
void parallelFor(Iterator&& begin, Iterator&& end, Lambda&& F)
{
    std::for_each(std::execution::par, begin, end, F);
}

/**
 * @brief This function executes a parallel for over the elements
 * iterated between `begin` and `end` iterators, if parallel requirements have
 * been found in the system.
 *
 * Example of usage on a vcl::Mesh, iterating over vertices:
 *
 * @code{.cpp}
 * vcl::parallelFor(m.vertices().begin(), m.vertices().end(),
 *     [&](VertexType& v) {
 *         // make some computing on v
 *     });
 * @endcode
 *
 * @param[in] begin: iterator of the first element to iterate
 * @param[in] end: iterator of the end of the iterated container
 * @param[in] F: lambda function that takes the iterated type as input
 */
template<typename Iterator, typename Lambda>
void parallelFor(const Iterator& begin, const Iterator& end, Lambda&& F)
{
    std::for_each(std::execution::par, begin, end, F);
}

/**
 * @brief This function executes a parallel for over a range if
 * parallel requirements have been found in the system.
 *
 * Example of usage on a vcl::Mesh, iterating over vertices:
 *
 * @code{.cpp}
 * vcl::parallelFor(m.vertices(), [&](VertexType& v) {
 *     // make some computing on v
 * });
 * @endcode
 *
 * @param[in] r: a range having begin() and end() functions
 * @param[in] F: lambda function that takes the iterated type as input
 */
template<vcl::Range Rng, typename Lambda>
void parallelFor(Rng&& r, Lambda&& F)
{
    parallelFor(std::ranges::begin(r), std::ranges::end(r), F);
}

/**
 * @brief This function executes a parallel for over a range if
 * parallel requirements have been found in the system.
 *
 * Example of usage on a vcl::Mesh, iterating over vertices:
 *
 * @code{.cpp}
 * vcl::parallelFor(m.vertices(), [&](VertexType& v) {
 *     // make some computing on v
 * });
 * @endcode
 *
 * @param[in] r: a range having begin() and end() functions
 * @param[in] F: lambda function that takes the iterated type as input
 */
template<vcl::Range Rng, typename Lambda>
void parallelFor(const Rng& r, Lambda&& F)
{
    parallelFor(std::ranges::begin(r), std::ranges::end(r), F);
}

} // namespace vcl

#endif // VCL_MISC_PARALLEL_H
