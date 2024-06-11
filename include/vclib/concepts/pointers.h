/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_CONCEPTS_POINTERS_H
#define VCL_CONCEPTS_POINTERS_H

#include <memory>

#include "const_correctness.h"

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
 * @brief Utility concept that is evaluated true if T is a Pointer, even if the
 * type is a T is a reference to a pointer.
 *
 * @ingroup uconcepts
 */
template<typename T>
concept IsPointer = std::is_pointer_v<std::remove_reference_t<T>>;

template<typename T>
concept IsSharedPointer = detail::IsSharedPtr<T>::value;

template<typename T>
concept IsAnyPointer = IsPointer<T> || IsSharedPointer<T>;

/**
 * @brief Utility concept that is evaluated true if T is a Pointer to a constant
 * object.
 *
 * @ingroup uconcepts
 */
// https://stackoverflow.com/a/37370281/5851101
template<typename T>
concept IsPointerToConst = IsPointer<T> && IsConst<T>;

template<typename T>
concept IsSharedPointerToConst =
    IsSharedPointer<T> && IsConst<typename T::element_type>;

template<typename T>
concept IsAnyPointerToConst = IsPointerToConst<T> || IsSharedPointerToConst<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_POINTERS_H
