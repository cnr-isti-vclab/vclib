/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BASE_PARALLEL_H
#define VCL_BASE_PARALLEL_H

#include <vclib/base/concepts/parallel.h>
#include <vclib/base/concepts/range.h>

#include <algorithm>

namespace vcl {

/**
 * @brief This function corresponds to std::for_each with an execution policy,
 * and it is provided for completeness.
 *
 * It executes a parallel for over the elements iterated between `begin` and
 * `end` iterators, with the given execution policy, if parallel requirements
 * have been found in the system.
 *
 * Example of usage on a vcl::Mesh, iterating over vertices:
 *
 * @code{.cpp}
 * vcl::parallelFor(
 *     std::execution::par_unseq,
 *     m.vertices().begin(),
 *     m.vertices().end(),
 *     [&](VertexType& v) {
 *         // make some computing on v
 *     });
 * @endcode
 *
 * @param[in] p: execution policy to use for the parallel for
 * @param[in] begin: iterator of the first element to iterate
 * @param[in] end: iterator of the end of the iterated container
 * @param[in] F: lambda function that takes the iterated type as input
 */
template<ExecutionPolicy Policy, typename Iterator, typename Lambda>
void parallelFor(Policy&& p, Iterator&& begin, Iterator&& end, Lambda&& F)
{
    std::for_each(p, begin, end, F);
}

/**
 * @brief This function executes a parallel for (with std::execution::par
 * policy) over the elements iterated between `begin` and `end` iterators, if
 * parallel requirements have been found in the system.
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
 * @brief This function executes a parallel for (with std::execution::par
 * policy) over a range, with the given execution policy, if parallel
 * requirements have been found in the system.
 *
 * Example of usage on a vcl::Mesh, iterating over vertices:
 *
 * @code{.cpp}
 * vcl::parallelFor(
 *     std::execution::par_unseq,
 *     m.vertices(),
 *     [&](VertexType& v) {
 *         // make some computing on v
 *     });
 * @endcode
 *
 * @param[in] p: execution policy to use for the parallel for
 * @param[in] r: a range having begin() and end() functions
 * @param[in] F: lambda function that takes the iterated type as input
 */
template<ExecutionPolicy Policy, Range Rng, typename Lambda>
void parallelFor(Policy&& p, Rng&& r, Lambda&& F)
{
    parallelFor(p, std::ranges::begin(r), std::ranges::end(r), F);
}

/**
 * @brief This function executes a parallel for (with std::execution::par
 * policy) over a range if parallel requirements have been found in the system.
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
template<Range Rng, typename Lambda>
void parallelFor(Rng&& r, Lambda&& F)
{
    parallelFor(std::ranges::begin(r), std::ranges::end(r), F);
}

} // namespace vcl

#endif // VCL_BASE_PARALLEL_H
