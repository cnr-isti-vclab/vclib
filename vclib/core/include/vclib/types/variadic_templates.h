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

#ifndef VCL_TYPES_VARIADIC_TEMPLATES_H
#define VCL_TYPES_VARIADIC_TEMPLATES_H

#include "base.h"

#include <tuple>
#include <typeindex>

namespace vcl {

// TODO: write documentation for all the functions and classes in this file

/**
 * @brief Get the first type of a pack of types (variadic templates) or a
 * TypeWrapper.
 *
 * Usage:
 * @code{.cpp}
 * using ResType = FirstType<int, float, double>::type;
 * static_assert(std::is_same<ResType, int>::value, "");
 * @endcode
 *
 * @ingroup types
 */
template<typename... Args>
struct FirstType
{
    using type = std::tuple_element<0, std::tuple<Args...>>::type;
};

/**
 * @brief Alias for the type of the first type in a pack of types.
 *
 * Usage:
 * @code{.cpp}
 * using ResType = FirstTypeT<int, float, double>;
 * static_assert(std::is_same<ResType, int>::value, "");
 * @endcode
 *
 * @ingroup types
 */
template<typename... Args>
using FirstTypeT = typename FirstType<Args...>::type;

/**
 * @brief Function that returns the index of a Type T in a pack of types
 * (variadic templates). The pack is composed of U and Us...
 *
 * @ingroup types
 */
// https://stackoverflow.com/a/71477756/5851101
template<typename T, typename U, typename... Us>
constexpr uint indexInTypePack()
{
    if constexpr (std::is_same_v<T, U>) {
        return 0;
    }
    else {
        if constexpr (sizeof...(Us)) {
            // there is at least another type to check
            constexpr uint res =
                indexInTypePack<T, Us...>(); // look in the rest of the types
            if constexpr (res == UINT_NULL)  // not found in the rest
                return UINT_NULL;
            else
                return 1 + res; // found
        }
        else { // not found
            return UINT_NULL;
        }
    }
}

/**
 * @brief Function that returns the index of the type having the given
 * type_index in a pack of types (variadic templates).
 * The pack is composed of U and Us...
 *
 * @param ti: the type_index of the type to search.
 *
 * @ingroup types
 */
template<typename U, typename... Us>
uint indexInTypePack(std::type_index ti)
{
    if (ti == typeid(U)) {
        return 0;
    }
    else {
        if constexpr (sizeof...(Us)) {
            // there is at least another type to check
            uint res = indexInTypePack<Us...>(ti);
            if (res == UINT_NULL)
                return UINT_NULL;
            else
                return 1 + res;
        }
        else { // not found
            return UINT_NULL;
        }
    }
}

template<typename T, typename... Us>
struct IndexInTypes
{
    static constexpr uint value = indexInTypePack<T, Us...>();
};

template<uint I, typename ...T>
struct TypeAt
{
    using type = std::tuple_element_t<I, std::tuple<T...>>;
};

template<uint I, typename ...T>
using TypeAtT = typename TypeAt<I, T...>::type;

template<typename... Args>
struct NumberOfTypes
{
    static constexpr uint value = sizeof...(Args);
};

/**
 * @brief Allows to apply a function to each type in a variadic template pack.
 *
 * Usage:
 * @code{.cpp}
 * // declare the function as a lambda - will do something with T
 * auto f = []<typename T>()
 * {
 *     std::cout << typeid(T).name() << std::endl;
 * };
 *
 * // call a function for each type in a parameter pack
 * vcl::ForEachType<int, float, double>::apply(f);
 * @endcode
 *
 * @ingroup types
 */
template<typename... T>
struct ForEachType
{
    template<typename F>
    static void apply(F&& f)
    {
        (f.template operator()<T>(), ...);
    }
};

} // namespace vcl

#endif // VCL_TYPES_VARIADIC_TEMPLATES_H
