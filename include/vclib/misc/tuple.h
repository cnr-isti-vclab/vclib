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

#ifndef VCL_MISC_TUPLE_H
#define VCL_MISC_TUPLE_H

#include <tuple>

#include "types.h"

namespace vcl {

/**
 * @brief Returns the index of a type T in a std::tuple<U, Us...> passed as parameter;
 * Returns UINT_NULL if the type T is not found in the tuple
 */
template <typename T, typename U, typename... Us>
constexpr auto tupleIndexByType(const std::tuple<U, Us...> &) {
	return indexInTypePack<T, U, Us...>();
}

/* Remove all types that do not satisfy a condition, and get them as a tuple. */

namespace internal {

template <typename, typename> struct Cons;

template <typename  T, typename ...Args>
struct Cons<T, std::tuple<Args...>>
{
	using type = std::tuple<T, Args...>;
};

} // namespace vcl::internal

template <template <class> class, typename ...>
struct FilterTypesByCondition { using type = std::tuple<>; };

/**
 * @brief Removes all types that do not satisfy a condition, and get them as a tuple.
 *
 * Usage:
 *
 * @code{.cpp}
 * using ResTuple = FilterTypesByCondition<std::is_integral, int, float, double, char>::type;
 * static_assert(std::is_same<ResTuple, std::tuple<int, char>>::value, "");
 * @endcode
 *
 * ResTuple will be a std::tuple<int, char> (int and char are the only integral types)
 */
template <template <class> class Pred, typename Head, typename ...Tail>
struct FilterTypesByCondition<Pred, Head, Tail...>
{
	using type = typename std::conditional<
		Pred<Head>::value,
		typename internal::Cons<Head, typename FilterTypesByCondition<Pred, Tail...>::type>::type,
		typename FilterTypesByCondition<Pred, Tail...>::type>::type;
};

} // namespace vcl

#endif // VCL_MISC_TUPLE_H
