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

#ifndef VCL_IO_SERIALIZATION_DESERIALIZE_H
#define VCL_IO_SERIALIZATION_DESERIALIZE_H

#include <bit>
#include <istream>
#include <vector>

#include <vclib/concepts/serialization.h>
#include <vclib/concepts/types.h>

#include "endian.h"

namespace vcl {

/**
 * @brief Deserialize data from an input stream, using the specified endian
 * format.
 *
 * The endian format specifies the format of the data read from the input
 * stream. If the endian format is different from the native one, the data is
 * swapped.
 *
 * @param[in] is: input stream.
 * @param[out] data: deserialized data.
 * @param[in] endian: endian format of the deserialization.
 */
template<IsNotClass T>
void deserialize(std::istream& is, T& data, std::endian endian)
{
    is.read(reinterpret_cast<char*>(&data), sizeof(T));
    if (endian != std::endian::native) {
        data = detail::swapEndian(data);
    }
}

/**
 * @brief Deserialize an array of contiguous data from an input stream, using
 * the specified format.
 *
 * The endian format specifies the format of the data read from the input
 * stream. If the endian format is different from the native one, the data is
 * swapped.
 *
 * By default, the deserialization is done in binary little endian format.
 *
 * @param[in] is: input stream.
 * @param[out] data: pointer to the deserialized data.
 * @param[in] size: number of elements to deserialize.
 * @param[in] endian: endian format of the deserialization.
 */
template<typename T>
void deserializeN(
    std::istream& is,
    T*            data,
    std::size_t   size,
    std::endian   endian = std::endian::little)
{
    for (std::size_t i = 0; i < size; ++i) {
        deserialize(is, data[i], endian);
    }
}

template<typename T, typename... Others>
void deserialize(std::istream& is, T& data, Others&... others)
{
    if constexpr (Serializable<T>) {
        data.deserialize(is);
    }
    else {
        deserialize(is, data, std::endian::little);
    }
    if constexpr (sizeof...(Others) > 0) {
        deserialize(is, others...);
    }
}

/// Deserialize specializations ///

/*
 * std::array
 */

template<typename T, std::size_t N>
void deserialize(std::istream& is, std::array<T, N>& a)
{
    if constexpr (Serializable<T>) {
        for (T& v : a) {
            v.deserialize(is);
        }
    }
    else {
        for (T& e : a) {
            deserialize(is, e);
        }
    }
}

/*
 * std::string
 */

inline void deserialize(std::istream& is, std::string& s)
{
    std::size_t size;
    deserialize(is, size);
    s.resize(size);
    deserializeN(is, s.data(), size);
}

/*
 * std::vector
 */

template<typename T>
void deserialize(std::istream& is, std::vector<T>& v)
{
    std::size_t size;
    deserialize(is, size);
    v.resize(size);
    if constexpr (Serializable<T>) {
        for (T& e : v) {
            e.deserialize(is);
        }
    }
    else {
        for (T& e : v) {
            deserialize(is, e);
        }
    }
}

} // namespace vcl

#endif // VCL_IO_SERIALIZATION_DESERIALIZE_H
