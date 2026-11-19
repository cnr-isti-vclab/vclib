// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_PARALLEL_H
#define VCL_BASE_PARALLEL_H

#include <vclib/base/concepts/parallel.h>
#include <vclib/base/concepts/range.h>

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <thread>
#include <type_traits>
#include <vector>

namespace vcl {

namespace detail {

// Iterators over a container that is addressable by index (e.g. the mesh
// element iterators) and that may skip deleted elements while iterating.
template<typename It>
concept IndexedSkippingIterator = requires (const It& it, const It& end) {
    { it.parentContainer() } -> std::convertible_to<const void*>;
    { it.index() } -> std::convertible_to<std::size_t>;
    { it.jumpDeleted() } -> std::same_as<bool>;
    { end.index() } -> std::convertible_to<std::size_t>;
    it.parentContainer()->isElementDeleted(it.index());
};

template<typename Policy, typename Iterator, typename Lambda>
void parallelForImpl(Policy&& p, Iterator begin, Iterator end, Lambda&& F)
{
    if constexpr (IndexedSkippingIterator<Iterator>) {
        // Iterators that jump deleted elements do not have a constant-time
        // `begin + i` nor `end - begin`: using them with parallel algorithms
        // would cost O(n^2). In this case, we split the index range in chunks,
        // and each chunk is visited sequentially, skipping deleted elements.
        if (begin.jumpDeleted() && begin.parentContainer()) {
            auto*             container = begin.parentContainer();
            const std::size_t first     = begin.index();
            const std::size_t last      = end.index();
            if (first >= last) {
                return;
            }

            const std::size_t n = last - first;
            const std::size_t nChunks = std::min<std::size_t>(
                n, 8 * std::max(1u, std::thread::hardware_concurrency()));
            const std::size_t chunkSize = (n + nChunks - 1) / nChunks;

            std::vector<std::size_t> chunkStarts;
            chunkStarts.reserve(nChunks);
            for (std::size_t s = first; s < last; s += chunkSize) {
                chunkStarts.push_back(s);
            }

            std::for_each(
                p,
                chunkStarts.begin(),
                chunkStarts.end(),
                [&](std::size_t s) {
                    const std::size_t e = std::min(s + chunkSize, last);
                    for (std::size_t i = s; i < e; ++i) {
                        if (!container->isElementDeleted(i)) {
                            F(*Iterator(*container, i, false));
                        }
                    }
                });
            return;
        }
    }
    std::for_each(p, begin, end, F);
}

} // namespace detail

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
 * @note When iterating over the elements (or the components) of a mesh
 * container that contains deleted elements, the iterators jump the deleted
 * elements, and their random access operations (`begin + i`, `end - begin`)
 * are not constant-time. To avoid a quadratic cost, in this case the index
 * range is split into chunks that are processed in parallel, each one visiting
 * its elements sequentially and skipping the deleted ones. There is no need to
 * compact the mesh before calling this function. The same applies to the
 * overloads taking a range.
 *
 * @param[in] p: execution policy to use for the parallel for
 * @param[in] begin: iterator of the first element to iterate
 * @param[in] end: iterator of the end of the iterated container
 * @param[in] F: lambda function that takes the iterated type as input
 */
template<ExecutionPolicy Policy, typename Iterator, typename Lambda>
void parallelFor(Policy&& p, Iterator&& begin, Iterator&& end, Lambda&& F)
{
    detail::parallelForImpl(p, begin, end, F);
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
    detail::parallelForImpl(std::execution::par, begin, end, F);
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
