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

#ifndef VCL_TYPES_POINTERS_H
#define VCL_TYPES_POINTERS_H

#include <type_traits>
#include <utility>

#include <vclib/concepts/pointers.h>

namespace vcl {

/*
 * Utility to get clean type from an input type that could have a reference or a
 * pointer.
 */
template<typename T>
using RemoveCVRefAndPointer =
    typename std::remove_cvref_t<std::remove_pointer_t<T>>;

/*
 * Utility to get a pointer type without the constness.
 * e.g. If T is const int*, the resulting type is int*
 * If the type is not a pointer, it is left as it was
 */
template<typename T>
using RemoveConstFromPointer = std::conditional_t<
    std::is_pointer_v<T>,
    std::add_pointer_t<
        typename std::remove_cv_t<typename std::remove_pointer_t<T>>>,
    T>;

/**
 * @brief Utility function that applies the unary operator '*' to the argument
 * only if the object is a pointer.
 *
 * @param obj
 * @return obj if it is not a pointer, or *obj if it is a pointer.
 */
template<typename T>
auto dereferencePtr(T&& obj)
{
    if constexpr (IsPointer<T>) {
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
 */
template<typename T>
auto addressOfObj(T& obj)
{
    if constexpr (IsPointer<T>) {
        return obj;
    }
    else {
        return &obj;
    }
}

} // namespace vcl

#endif // VCL_TYPES_POINTERS_H
