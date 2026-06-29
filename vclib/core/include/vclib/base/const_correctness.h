// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_CONST_CORRECTNESS_H
#define VCL_BASE_CONST_CORRECTNESS_H

#include <memory>
#include <type_traits>

namespace vcl {

/**
 * @brief Utility type that makes possible to treat const pointers in a
 * templated class that can treat a both const and non-const pointer type.
 *
 * The type `T` is converted to a const pointer if it is a raw pointer or a
 * shared pointer type.
 *
 * @tparam T The type to be converted to a const pointer.
 *
 * @ingroup base
 */
template<typename T>
struct MakeConstPointer
{
    using type = T;
};

/**
 * @copydoc MakeConstPointer
 *
 * @ingroup base
 */
template<typename T>
struct MakeConstPointer<T*>
{
    using type = const T*;
};

/**
 * @copydoc MakeConstPointer
 *
 * @ingroup base
 */
template<typename T>
struct MakeConstPointer<std::shared_ptr<T>>
{
    using type = std::shared_ptr<const T>;
};

/**
 * @brief Utility alias for the MakeConstPointer type.
 *
 * @tparam T The type to be converted to a const pointer.
 *
 * @ingroup base
 */
template<typename T>
using MakeConstPointerT = typename MakeConstPointer<T>::type;

/*
 * Full deduction for the possibility to re-use same code for const and
 * non-const member functions https://stackoverflow.com/a/47369227/5851101
 */

/**
 * @brief Utility function that converts a const pointer/reference to a
 * non-const pointer/reference.
 *
 * It is useful when you need to re-use the same code for const and non-const
 * member functions.
 *
 * @warning The function should be used only in contexts where the constness of
 * the pointer/reference is guaranteed to be removed (see
 * [const_cast](https://en.cppreference.com/w/cpp/language/const_cast)).
 *
 * @tparam T The type of the pointer/reference.
 * @param[in] value: The const pointer/reference to be const-casted.
 * @return The non-const pointer/reference.
 *
 * @ingroup base
 */
template<typename T>
constexpr T& asConst(const T& value) noexcept
{
    return const_cast<T&>(value);
}

/**
 * @copydoc asConst
 *
 * @ingroup base
 */
template<typename T>
constexpr T* asConst(const T* value) noexcept
{
    return const_cast<T*>(value);
}

/**
 * @copydoc asConst
 *
 * @ingroup base
 */
template<typename T>
constexpr T* asConst(T* value) noexcept
{
    return value;
}

template<typename T>
void asConst(const T&&) = delete;

} // namespace vcl

#endif // VCL_BASE_CONST_CORRECTNESS_H
