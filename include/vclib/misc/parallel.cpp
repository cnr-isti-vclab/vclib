/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "parallel.h"

#include <algorithm>

namespace vcl {

/**
 * @brief This function executes a parallel (vectorized) for over the elements 
 * iterated between `begin` and `end` iterators, if parallel requirements have 
 * been found in the system.
 *
 * Example of usage on a vcl::Mesh, iterating over vertices:
 *
 * @code{.cpp}
 * vcl::parallelFor(m.vertices().begin(), m.vertices().end(), [&](VertexType& v) {
 *     // make some computing on v
 * });
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
 * @brief This function executes a parallel (vectorized) for over the elements 
 * iterated between `begin` and `end` iterators, if parallel requirements have 
 * been found in the system.
 *
 * Example of usage on a vcl::Mesh, iterating over vertices:
 *
 * @code{.cpp}
 * vcl::parallelFor(m.vertices().begin(), m.vertices().end(), [&](VertexType& v) {
 *     // make some computing on v
 * });
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
 * @brief This function executes a parallel (vectorized) for over a range if
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
template<std::ranges::range Rng, typename Lambda>
void parallelFor(Rng&& r, Lambda&& F)
{
	parallelFor(std::ranges::begin(r), std::ranges::end(r), F);
}

/**
 * @brief This function executes a parallel (vectorized) for over a range if
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
template<std::ranges::range Rng, typename Lambda>
void parallelFor(const Rng& r, Lambda&& F)
{
	parallelFor(std::ranges::begin(r), std::ranges::end(r), F);
}

} // namespace vcl
