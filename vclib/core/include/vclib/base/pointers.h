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

#ifndef VCL_BASE_POINTERS_H
#define VCL_BASE_POINTERS_H

#include <type_traits>
#include <utility>

namespace vcl {

/**
 * @brief Utility alias to get a type without the pointer.
 * e.g. If T is int*, the resulting type is int.
 *
 * It corresponds to std::remove_pointer_t.
 *
 * @tparam T The input type.
 *
 * @ingroup base
 */
template<typename T>
using RemovePtr = std::remove_pointer_t<T>;

/**
 * @brief Utility alias to get a type type without the reference.
 * e.g. If T is int&, the resulting type is int.
 *
 * It corresponds to std::remove_reference_t.
 *
 * @tparam T The input type.
 *
 * @ingroup base
 */
template<typename T>
using RemoveRef = std::remove_reference_t<T>;

/**
 * @brief Utility alias to get clean type from an input type that could have a
 * reference or a pointer.
 *
 * @tparam T The input type.
 *
 * @ingroup base
 */
template<typename T>
using RemoveCVRefAndPointer = std::remove_cvref_t<RemovePtr<T>>;

/**
 * @brief Utility alias to get a pointer type without the constness.
 * e.g. If T is const int*, the resulting type is int*
 * If the type is not a pointer, it is left as it was
 *
 * @tparam T The input type.
 *
 * @ingroup base
 */
template<typename T>
using RemoveConstFromPointer = std::conditional_t<
    std::is_pointer_v<T>,
    std::add_pointer_t<std::remove_cv_t<RemovePtr<T>>>,
    T>;

/**
 * @brief Utility function that applies the unary operator '*' to the argument
 * only if the object is a pointer, and returns a reference to the object
 * itself.
 *
 * @param[in] obj: The object to dereference.
 * @return a reference to obj if it is not a pointer, or a reference to *obj if
 * it is a pointer.
 *
 * @ingroup base
 */
template<typename T>
auto& dereferencePtr(T&& obj)
{
    if constexpr (std::is_pointer_v<RemoveRef<T>>) {
        return *obj;
    }
    else {
        return std::forward<T>(obj);
    }
}

/**
 * @brief Utility function that applies the unary operator '&' to the argument
 * only if it is not a pointer.
 *
 * @param obj
 * @return the address of 'obj' if obj is not a pointer, or obj if it is a
 * pointer.
 *
 * @ingroup base
 */
template<typename T>
auto addressOfObj(T& obj)
{
    if constexpr (std::is_pointer_v<RemoveRef<T>>) {
        return obj;
    }
    else {
        return &obj;
    }
}

} // namespace vcl

#endif // VCL_BASE_POINTERS_H
