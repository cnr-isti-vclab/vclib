// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_FILTER_TYPES_H
#define VCL_BASE_FILTER_TYPES_H

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
 * TypeWrapper.
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
 * ResTypes will be a TypeWrapper<int, char> (int and char are the only integral
 * types).
 *
 * @ingroup base
 */
template<template<typename> typename, typename...>
struct FilterTypesByCondition
{
    using type = TypeWrapper<>;
};

/**
 * @copydoc FilterTypesByCondition
 *
 * @ingroup base
 */
template<template<typename> typename Pred, typename Head, typename... Tail>
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
 * @ingroup base
 */
// TypeWrapper specialization
template<template<typename> typename Pred, typename... Tail>
struct FilterTypesByCondition<Pred, TypeWrapper<Tail...>>
{
    using type = FilterTypesByCondition<Pred, Tail...>::type;
};

/**
 * @brief Sets its `value` to `true` if there is at least one type in the given
 * pack Args... that satisfies the given condition
 *
 * Usage:
 *
 * @code{.cpp}
 * // there is a type (int) that is integral
 * static const bool res = OneTypeAtLeastSatisfiesCondition<
 *     std::is_integral,
 *     int, float, double>::value;
 * static_assert(res == true, "");
 *
 * static const bool res2 =
 *     OneTypeAtLeastSatisfiesCondition<
 *     std::is_integral,
 *     float, double>::value;
 * static_assert(res2 != true, "");
 * @endcode
 *
 * @ingroup base
 */
template<template<typename> typename Pred, typename... Args>
struct OneTypeAtLeastSatisfiesCondition
{
private:
    using ResTypes = FilterTypesByCondition<Pred, Args...>::type;

public:
    static constexpr bool value = NumberOfTypes<ResTypes>::value != 0;
};

/**
 * @copydoc OneTypeAtLeastSatisfiesCondition
 *
 * @ingroup base
 */
// TypeWrapper specialization
template<template<typename> typename Pred, typename... Args>
struct OneTypeAtLeastSatisfiesCondition<Pred, TypeWrapper<Args...>>
{
    using type = OneTypeAtLeastSatisfiesCondition<Pred, Args...>::type;
};

/**
 * @brief Sets `type` to the first type of a pack that satisfies the given
 * condition.
 *
 * Usage:
 * @code{.cpp}
 * // the first integral type is char
 * using ResType =
 *     GetTypeByCondition<std::is_integral, float, double, char, int>::type;
 * static_assert(std::is_same<ResType, char>::value, "");
 * @endcode
 *
 * @ingroup base
 */
template<template<typename> typename Pred, typename... Args>
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
 * @ingroup base
 */
// TypeWrapper specialization
template<template<typename> typename Pred, typename... Args>
struct GetTypeByCondition<Pred, TypeWrapper<Args...>>
{
    using type = GetTypeByCondition<Pred, Args...>::type;
};

} // namespace vcl

#endif // VCL_BASE_FILTER_TYPES_H
