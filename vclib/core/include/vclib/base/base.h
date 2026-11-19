// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_BASE_H
#define VCL_BASE_BASE_H

#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>

namespace vcl {

// Define some basic types, for convenience
using uint   = uint32_t;
using ushort = uint16_t;
using uchar  = uint8_t;

/**
 * @brief The UINT_NULL value represent a null value of uint that is the maximum
 * value that can be represented with unsigned int.
 *
 * Allows to fully use all the possible values (except one) that can be
 * represented in an unsigned int, but with the possibility to flag whether the
 * value is not initialized or is set to null.
 *
 * @ingroup base
 */
constexpr uint UINT_NULL = std::numeric_limits<uint>::max();

/**
 * @brief The USHORT_NULL value represent a null value of ushort that is the
 * maximum value that can be represented with unsigned short.
 *
 * Allows to fully use all the possible values (except one) that can be
 * represented in an unsigned short, but with the possibility to flag whether
 * the value is not initialized or is set to null.
 *
 * @ingroup base
 */
constexpr ushort USHORT_NULL = std::numeric_limits<ushort>::max();

/**
 * @brief The NULL_INDEX variable template represents a null / invalid sentinel
 * value for a generic unsigned integral index type, which is the maximum value
 * that can be represented with that type.
 *
 * Allows to fully use all the possible values (except one) that can be
 * represented in an unsigned integer, with the possibility to flag whether the
 * index is not initialized or is set to null.
 *
 * @tparam Index An unsigned integral type. Defaults to uint (uint32_t).
 * @ingroup base
 */
template<std::unsigned_integral Index = uint>
inline constexpr Index NULL_INDEX = std::numeric_limits<Index>::max();

/**
 * @brief A simple type that enumerates the main primitive types.
 *
 * It is used mostly for I/O operations, but can be useful in any context where
 * a type must be described in a variable at runtime.
 *
 * @ingroup base
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
 * @ingroup base
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
 * @ingroup base
 */
template<class T>
class FakePointerWithValue
{
    T mValue;

public:
    FakePointerWithValue(const T& value) : mValue(value) {}

    T* operator->() { return std::addressof(mValue); }
};

/**
 * @brief Utility function to create in place a std::array with all the elements
 * initialized to the same value.
 * @param[in] value: the value to initialize all the elements of the array with.
 * @return a std::array with all the elements initialized to the same value.
 */
template<typename T, size_t N>
constexpr std::array<T, N> makeArray(T value)
{
    std::array<T, N> a;
    a.fill(value);
    return a;
}

} // namespace vcl

#endif // VCL_BASE_BASE_H
