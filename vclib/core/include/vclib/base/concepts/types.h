// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_CONCEPTS_TYPES_H
#define VCL_BASE_CONCEPTS_TYPES_H

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
 * @brief Concept for types that can be used as indices, excluding bool and
 * char.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

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

#endif // VCL_BASE_CONCEPTS_TYPES_H
