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

#ifndef VCL_IO_SERIALIZATION_SERIALIZE_H
#define VCL_IO_SERIALIZATION_SERIALIZE_H

#include <ostream>
#include <vector>

#include <vclib/concepts/serialization.h>
#include <vclib/concepts/types.h>

#include "endian.h"

namespace vcl {

/**
 * @brief Serialize data to an output stream, using the specified endian format.
 *
 * The endian format specifies if the data should be converted to a different
 * endianness w.r.t. the native one.
 *
 * @param[in] os: output stream.
 * @param[in] data: data to serialize.
 * @param[in] endian: endian format of the serialization.
 */
template<IsNotClass T>
void serialize(std::ostream& os, const T& data, std::endian endian)
{
    if (endian != std::endian::native) {
        T swapped = detail::swapEndian(data);
        os.write(reinterpret_cast<const char*>(&swapped), sizeof(T));
    }
    else {
        os.write(reinterpret_cast<const char*>(&data), sizeof(T));
    }

}

/**
 * @brief Serialize an array of contiguous data to an output stream, using the
 * specified format.
 *
 * The endian format specifies if the data should be converted to a different
 * endianness w.r.t. the native one.
 *
 * By default, the serialization is done in binary little endian format.
 *
 * @param[in] os: output stream.
 * @param[in] data: pointer to the data to serialize.
 * @param[in] size: number of elements to serialize.
 * @param[in] endian: endian format of the serialization.
 */
template<typename T>
void serializeN(
    std::ostream& os,
    const T*      data,
    std::size_t   size,
    std::endian endian = std::endian::little)
{
    for (std::size_t i = 0; i < size; ++i) {
        serialize(os, data[i], endian);
    }
}

template<typename T, typename ...Others>
void serialize(std::ostream& os, const T& data, const Others&... others)
{
    if constexpr(Serializable<T>) {
        data.serialize(os);
    }
    else {
        serialize(os, data, std::endian::little);
    }
    if constexpr(sizeof...(Others) > 0) {
        serialize(os, others...);
    }
}

/// Serialize specializations ///

/*
 * std::array
 */

template<typename T, std::size_t N>
void serialize(std::ostream& os, const std::array<T, N>& a)
{
    if constexpr (Serializable<T>) {
        for (const T& v : a) {
            v.serialize(os);
        }
    }
    else {
        for (const T& e : a) {
            serialize(os, e);
        }
    }
}

/*
 * std::string
 */

inline void serialize(std::ostream& os, const std::string& s)
{
    std::size_t size = s.size();
    serialize(os, size);
    serializeN(os, s.data(), size);
}

/*
 * std::vector
 */

template<typename T>
void serialize(std::ostream& os, const std::vector<T>& v)
{
    std::size_t size = v.size();
    serialize(os, size);
    if constexpr (Serializable<T>) {
        for (const T& e : v) {
            e.serialize(os);
        }
    }
    else {
        for (const T& e : v) {
            serialize(os, e);
        }
    }
}

} // namespace vcl

#endif // VCL_IO_SERIALIZATION_SERIALIZE_H
