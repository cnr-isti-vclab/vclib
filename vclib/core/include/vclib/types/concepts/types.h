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

#ifndef VCL_TYPES_CONCEPTS_TYPES_H
#define VCL_TYPES_CONCEPTS_TYPES_H

#include <concepts>

namespace vcl {

/**
 * @brief Concept for types that can be used as indices.
 *
 * This concept is evaluated true if T is an integral type or an enum type.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IntegralOrEnum = std::integral<T> || std::is_enum_v<T>;

/**
 * @brief Concept for types that can be used as indices, excluding bool.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept NonBoolIntegralOrEnum = IntegralOrEnum<T> && !std::same_as<T, bool>;

/**
 * @brief Concept that is evaluated true if T is a class.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IsClass = std::is_class_v<T>;

/**
 * @brief Concept that is evaluated true if T is not a class.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IsNotClass = !IsClass<T>;

/**
 * @brief Concept that is evaluated true if the templated type C is instantiable
 * with type T, i.e., C<T> is a valid type.
 *
 * Example of usage:
 * @code{.cpp}
 * if constexpr(IsInstantiable<C, T>) {
 *     C<T> instance;
 *     // do something...
 * }
 * @endcode
 *
 * @ingroup util_concepts
 */
template<template<typename> typename C, typename T>
concept IsInstantiable = requires { typename C<T>; };

} // namespace vcl

#endif // VCL_TYPES_CONCEPTS_TYPES_H
