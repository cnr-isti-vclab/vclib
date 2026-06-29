// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_CONCEPTS_POINTERS_H
#define VCL_BASE_CONCEPTS_POINTERS_H

#include "const_correctness.h"

#include <memory>

namespace vcl {

namespace detail {

template<typename T>
struct IsSharedPtr : std::false_type
{
};

template<typename T>
struct IsSharedPtr<std::shared_ptr<T>> : std::true_type
{
};

} // namespace detail

/**
 * @brief The IsPointer concept is satisfied if T is a Pointer, even if the type
 * T is a reference to a pointer.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IsPointer = std::is_pointer_v<RemoveRef<T>>;

/**
 * @brief The IsSharedPointer concept is satisfied if T is a shared pointer.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IsSharedPointer = detail::IsSharedPtr<T>::value;

/**
 * @brief The IsAnyPointer concept is satisfied if T is a Pointer or a shared
 * pointer.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IsAnyPointer = IsPointer<T> || IsSharedPointer<T>;

/**
 * @brief The IsPointerToConst concept is satisfied if T is a Pointer to a
 * constant object.
 *
 * @ingroup util_concepts
 *
 * @private https://stackoverflow.com/a/37370281/5851101
 */
template<typename T>
concept IsPointerToConst = IsPointer<T> && IsConst<T>;

/**
 * @brief The IsSharedPointerToConst concept is satisfied if T is a shared
 * pointer to a constant object.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IsSharedPointerToConst =
    IsSharedPointer<T> && IsConst<typename T::element_type>;

/**
 * @brief The IsAnyPointerToConst concept is satisfied if T is a Pointer or a
 * shared pointer to a constant object.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IsAnyPointerToConst = IsPointerToConst<T> || IsSharedPointerToConst<T>;

} // namespace vcl

#endif // VCL_BASE_CONCEPTS_POINTERS_H
