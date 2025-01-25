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

#ifndef VCL_TYPES_BASE_H
#define VCL_TYPES_BASE_H

#include <cassert>
#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>

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
enum class PrimitiveType {
    CHAR,
    UCHAR,
    SHORT,
    USHORT,
    INT,
    UINT,
    FLOAT,
    DOUBLE,
    NONE
};

/**
 * @brief A simple type that enumerates the main storage types for matrices (row
 * or column major).
 *
 * @ingroup types
 */
enum class MatrixStorageType { ROW_MAJOR, COLUMN_MAJOR };

/**
 * @brief Returns the size in bytes of a primitive type.
 * @param[in] type: The primitive type.
 * @return The size in bytes of the primitive type.
 */
// TODO: from c++23 use fixed width floating-point types
constexpr int sizeOf(PrimitiveType type) noexcept
{
    switch (type) {
    case PrimitiveType::CHAR: return sizeof(char);
    case PrimitiveType::UCHAR: return sizeof(unsigned char);
    case PrimitiveType::SHORT: return sizeof(int16_t);
    case PrimitiveType::USHORT: return sizeof(uint16_t);
    case PrimitiveType::INT: return sizeof(int32_t);
    case PrimitiveType::UINT: return sizeof(uint32_t);
    case PrimitiveType::FLOAT: return sizeof(float);
    case PrimitiveType::DOUBLE: return sizeof(double);
    default: return 0;
    }
}

/**
 * @brief Returns the underlying type of an enum.
 * @param[in] e: The enum value.
 * @return The value casted to the underlying type of the enum.
 */
// TODO: remove from c++23 (std::to_underlying)
template<typename E>
constexpr typename std::underlying_type<E>::type toUnderlying(E e) noexcept
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}

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
