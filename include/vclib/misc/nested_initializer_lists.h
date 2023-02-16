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

#ifndef VCL_MISC_NESTED_INITIALIZER_LISTS_H
#define VCL_MISC_NESTED_INITIALIZER_LISTS_H

/* @link https://zcrou.com/blog/dev/nested-initializers */

#include <assert.h>
#include <cstddef>
#include <initializer_list>
#include <list>

namespace vcl {

// Traits
namespace internal {

template<typename T, std::size_t L> // recursive case: a nested list is a list of L-1 levels
struct NestedInitializerListsTraits
{
	using type = std::initializer_list<typename NestedInitializerListsTraits<T, L - 1>::type>;
};

template<typename T> // base case: no list, only the type T
struct NestedInitializerListsTraits<T, 0>
{
	using type = T;
};

} // namespace internal

/**
 * @brief The NestedInitializerLists class manages nested std::initializer_list<T> types of L
 * levels L must be known at compile time, and compilation will fail if the NestedInitializerLists
 * used does not correspond to the number of used levels.
 */
template<typename T, std::size_t L>
using NestedInitializerLists = typename internal::NestedInitializerListsTraits<T, L>::type;

// Processors that allow to automatically iterate through nested initializer lists

// Recursive part.
/**
 * @brief The NestedInitializerListsProcessor class
 *
 * Proposes some helper static functions that allows to process vcl::NestedInitializerLists of L
 * levels.
 */
template<typename T, std::size_t L>
class NestedInitializerListsProcessor
{
public:
	static std::list<size_t> maxDimensionsLevels(NestedInitializerLists<T, L> values);

	template<typename T_Function>
	static void processElements(NestedInitializerLists<T, L> values, T_Function function);

	template<typename T_Function>
	static void processElements(
		NestedInitializerLists<T, L> values,
		T_Function                   function,
		std::list<size_t>            sizes);
};

// Last level.
template<typename T>
class NestedInitializerListsProcessor<T, 1>
{
public:
	using InitializerList = vcl::NestedInitializerLists<T, 1>;

	static std::list<size_t> maxDimensionsLevels(InitializerList values);

	template<typename T_Function>
	static void
	processElements(InitializerList values, T_Function function, std::list<size_t> sizes);

	template<typename T_Function>
	static void processElements(InitializerList values, T_Function function);
};

} // namespace vcl

#include "nested_initializer_lists.cpp"

#endif // VCL_MISC_NESTED_INITIALIZER_LISTS_H
