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

#ifndef VCL_TYPES_FILTER_TYPES_H
#define VCL_TYPES_FILTER_TYPES_H

#include "type_wrapper.h"

namespace vcl {

namespace detail {

template<typename, typename>
struct TypeWrapperConstructor;

template<typename T, typename... Args>
struct TypeWrapperConstructor<T, TypeWrapper<Args...>>
{
    using type = TypeWrapper<T, Args...>;
};

} // namespace detail

/**
 * @brief Removes all types that do not satisfy a condition, and get them as a
 * tuple.
 *
 * The condition is a templated predicate struct that must have a static member
 * bool `value` that is true if the type satisfies the condition.
 *
 * The result is the `type` type alias, that is a TypeWrapper containing the
 * types that satisfy the condition.
 *
 * The input types can be a pack of types or a TypeWrapper.
 *
 * Usage:
 *
 * @code{.cpp}
 * using ResTypes =
 *     FilterTypesByCondition<std::is_integral, int, float, double, char>::type;
 * static_assert(std::is_same<ResTypes, TypeWrapper<int, char>>::value, "");
 * @endcode
 *
 * ResTuple will be a TypeWrapper<int, char> (int and char are the only integral
 * types).
 *
 * @ingroup types
 */
template<template<class> class, typename...>
struct FilterTypesByCondition
{
    using type = TypeWrapper<>;
};

/**
 * @copydoc FilterTypesByCondition
 *
 * @ingroup types
 */
template<template<class> class Pred, typename Head, typename... Tail>
struct FilterTypesByCondition<Pred, Head, Tail...>
{
    using type = std::conditional<
        Pred<Head>::value,
        typename detail::TypeWrapperConstructor<
            Head,
            typename FilterTypesByCondition<Pred, Tail...>::type>::type,
        typename FilterTypesByCondition<Pred, Tail...>::type>::type;
};

/**
 * @copydoc FilterTypesByCondition
 *
 * @ingroup types
 */
// TypeWrapper specialization
template<template<class> class Pred, typename... Tail>
struct FilterTypesByCondition<Pred, TypeWrapper<Tail...>>
{
    using type = FilterTypesByCondition<Pred, Tail...>::type;
};

/**
 * @brief Its value is set to true if there is at least one type in the given
 * pack Args... that satisfies the given condition
 *
 * Usage:
 *
 * @code{.cpp}
 * // there is a type (int) that is integral
 * static const bool res =
 *     TypesSatisfyCondition<std::is_integral, int, float, double>::value;
 * static_assert(res == true, "");
 *
 * static const bool res2 =
 *     TypesSatisfyCondition<std::is_integral, float, double>::value;
 * static_assert(res2 != true, "");
 * @endcode
 *
 * @ingroup types
 */
template<template<class> class Pred, typename... Args>
struct TypesSatisfyCondition
{
private:
    using ResTypes = FilterTypesByCondition<Pred, Args...>::type;

public:
    static constexpr bool value = NumberOfTypes<ResTypes>::value != 0;
};

/**
 * @copydoc TypesSatisfyCondition
 *
 * @ingroup types
 */
// TypeWrapper specialization
template<template<class> class Pred, typename... Args>
struct TypesSatisfyCondition<Pred, TypeWrapper<Args...>>
{
    using type = TypesSatisfyCondition<Pred, Args...>::type;
};

/**
 * @brief The the first type of a pack that satisfies the given condition.
 *
 * Usage:
 * @code{.cpp}
 * // the first integral type is char
 * using ResType =
 *     GetTypeByCondition<std::is_integral, float, double, char, int>::type;
 * static_assert(std::is_same<ResType, char>::value, "");
 * @endcode
 *
 * @ingroup types
 */
template<template<class> class Pred, typename... Args>
struct GetTypeByCondition
{
private:
    using ResTypes = FilterTypesByCondition<Pred, Args...>::type;

public:
    using type = FirstTypeT<ResTypes>;
};

/**
 * @copydoc GetTypeByCondition
 *
 * @ingroup types
 */
// TypeWrapper specialization
template<template<class> class Pred, typename... Args>
struct GetTypeByCondition<Pred, TypeWrapper<Args...>>
{
    using type = GetTypeByCondition<Pred, Args...>::type;
};

} // namespace vcl

#endif // VCL_TYPES_FILTER_TYPES_H
