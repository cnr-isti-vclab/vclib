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

#ifndef VCL_TYPES_BASE_H
#define VCL_TYPES_BASE_H

#include <cassert>
#include <concepts>
#include <limits>
#include <numeric>

namespace vcl {

// Define some basic types, for convenience
using uint   = unsigned int;
using ushort = unsigned short;

/**
 * @brief The UINT_NULL value represent a null value of uint that is the maximum
 * value that can be represented with unsigned int.
 *
 * Allows to fully use all the possible values (except one) that can be
 * represented in an unsigned int, but with the possibility to flag whether the
 * value is not initialized or is set to null.
 *
 * @ingroup types
 */
constexpr uint UINT_NULL = std::numeric_limits<uint>::max();

/**
 * @brief A simple type that enumerates the main primitive types.
 *
 * It is used mostly for I/O operations, but can be useful in any context where
 * a type must be described in a variable at runtime.
 *
 * @ingroup types
 */
struct PrimitiveType
{
    enum Enum { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE, NONE };
};

/**
 * @brief A simple utility class to represent a pointer with a value.
 *
 * Useful for iterators that return a pointer to a value, but the value is
 * stored in the iterator itself.
 *
 * @ingroup types
 */
template<class T>
class FakePointerWithValue
{
    T mValue;

public:
    FakePointerWithValue(const T& value) : mValue(value) {}

    T* operator->() { return std::addressof(mValue); }
};

} // namespace vcl

#endif // VCL_TYPES_BASE_H
