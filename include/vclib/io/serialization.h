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

#ifndef VCL_IO_SERIALIZATION_H
#define VCL_IO_SERIALIZATION_H

#include <bit>

#include <vclib/concepts/serialization.h>

#include "file_format.h"

namespace vcl {

// https://stackoverflow.com/a/38141476/5851101
template <typename T>
T swapEndian(T u)
{
    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (std::size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

/**
 * @brief Serialize data to an output stream, using the specified format.
 *
 * The format specifies if the serialization should be binary or text, and if
 * the data should be converted to a different endianness w.r.t. the native one.
 *
 * By default, the serialization is done in binary little endian format.
 *
 * @param[in] os: output stream.
 * @param[in] data: data to serialize.
 * @param[in] format: format of the serialization.
 */
template<typename T>
void serialize(
    std::ostream& os,
    const T&      data,
    FileFormat    format = FileFormat())
{
    if (format.isBinary) {
        if (format.endian != std::endian::native) {
            T swapped = swapEndian(data);
            os.write(reinterpret_cast<const char*>(&swapped), sizeof(T));
        }
        else {
            os.write(reinterpret_cast<const char*>(&data), sizeof(T));
        }
    }
    else {
        if constexpr (OutputStreamable<T>) {
            os << data << " ";
        }
        else {
            throw std::runtime_error(
                "Data type is not serializable to text stream");
        }
    }
}

template<typename T>
void serialize(
    std::ostream& os,
    const T*      data,
    std::size_t   size,
    FileFormat    format = FileFormat())
{
    for (std::size_t i = 0; i < size; ++i) {
        serialize(os, data[i], format);
    }
}

/**
 * @brief Deserialize data from an input stream, using the specified format.
 *
 * The format specifies if the deserialization should be binary or text, and if
 * the data is read with a different endianness w.r.t. the native one.
 *
 * By default, the deserialization is done in binary little endian format.
 *
 * @param[in] is: input stream.
 * @param[out] data: deserialized data.
 * @param[in] format: format of the deserialization.
 */
template<typename T>
void deserialize(
    std::istream& is,
    T&            data,
    FileFormat    format = FileFormat(std::endian::little))
{
    if (format.isBinary) {
        is.read(reinterpret_cast<char*>(&data), sizeof(T));
        if (format.endian != std::endian::native) {
            data = swapEndian(data);
        }
    }
    else {
        if constexpr (InputStreamable<T>) {
            is >> data;
        }
        else {
            throw std::runtime_error(
                "Data type is not deserializable from text stream");
        }
    }
}

template<typename T>
void deserialize(
    std::istream& is,
    T*           data,
    std::size_t  size,
    FileFormat   format = FileFormat(std::endian::little))
{
    for (std::size_t i = 0; i < size; ++i) {
        deserialize(is, data[i], format);
    }
}

} // namespace vcl

#endif // VCL_IO_SERIALIZATION_H
