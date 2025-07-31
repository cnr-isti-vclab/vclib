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
 * @param[in] ti: the type_index of the type to search.
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

/**
 * @brief Get the index of a type T in a pack of types (variadic templates) or a
 * TypeWrapper.
 *
 * Usage:
 * @code{.cpp}
 * const uint i = IndexInTypes<int, int, float, double>::value;
 * static_assert(i == 0, "");
 * @endcode
 *
 * @ingroup types
 */
template<typename T, typename... Us>
struct IndexInTypes
{
    static constexpr uint value = indexInTypePack<T, Us...>();
};

/**
 * @brief Get the type at a given index in a pack of types (variadic templates)
 * or a TypeWrapper.
 *
 * Usage:
 * @code{.cpp}
 * using ResType = TypeAt<1, int, float, double>::type;
 * static_assert(std::is_same<ResType, float>::value, "");
 * @endcode
 *
 * @ingroup types
 */
template<uint I, typename... T>
struct TypeAt
{
    using type = std::tuple_element_t<I, std::tuple<T...>>;
};

/**
 * @brief Alias for the type at a given index in a pack of types (variadic
 * templates) or a TypeWrapper.
 *
 * Usage:
 * @code{.cpp}
 * using ResType = TypeAtT<1, int, float, double>;
 * static_assert(std::is_same<ResType, float>::value, "");
 * @endcode
 *
 * @ingroup types
 */
template<uint I, typename... T>
using TypeAtT = typename TypeAt<I, T...>::type;

/**
 * @brief Get the number of types in a pack of types (variadic templates) or a
 * TypeWrapper.
 *
 * Usage:
 * @code{.cpp}
 * const uint i = NumberOfTypes<int, float, double>::value;
 * static_assert(i == 3, "");
 * @endcode
 *
 * @ingroup types
 */
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

/**
 * @brief Trait to check if a type is a specialization of a given template.
 *
 * Usage:
 * @code{.cpp}
 * static_assert(IsSpecializationOf<std::vector<int>, std::vector>::value, "");
 * static_assert(!IsSpecializationOf<int, std::vector>::value, "");
 * @endcode
 *
 * @ingroup types
 */
template<typename T, template<typename...> class Template>
struct IsSpecializationOf : std::false_type {};

template<template<typename...> class Template, typename... Args>
struct IsSpecializationOf<Template<Args...>, Template> : std::true_type {};

/**
 * @brief Alias for IsSpecializationOf trait.
 *
 * @ingroup types
 */
template<typename T, template<typename...> class Template>
constexpr bool IsSpecializationOfV = IsSpecializationOf<T, Template>::value;

/**
 * @brief Trait to check if a type is derived from a specialization of a given template.
 *
 * This trait combines inheritance checking with template specialization detection.
 *
 * Usage:
 * @code{.cpp}
 * class MyVector : public std::vector<int> {};
 * static_assert(IsDerivedFromSpecializationOf<MyVector, std::vector>::value, "");
 * @endcode
 *
 * @ingroup types
 */
template<typename T, template<typename...> class Template>
struct IsDerivedFromSpecializationOf
{
private:
    // Helper to check if T is derived from Template<Args...> for any Args
    template<typename... Args>
    static std::true_type test(const Template<Args...>*);
    static std::false_type test(...);
    
public:
    static constexpr bool value = decltype(test(std::declval<std::remove_cvref_t<T>*>()))::value;
};

/**
 * @brief Alias for IsDerivedFromSpecializationOf trait.
 *
 * @ingroup types
 */
template<typename T, template<typename...> class Template>
constexpr bool IsDerivedFromSpecializationOfV = IsDerivedFromSpecializationOf<T, Template>::value;

} // namespace vcl

#endif // VCL_TYPES_VARIADIC_TEMPLATES_H
