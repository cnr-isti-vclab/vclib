// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_SERIALIZATION_DESERIALIZE_H
#define VCL_BASE_SERIALIZATION_DESERIALIZE_H

#include "endian.h"

#include <vclib/base/concepts/serialization.h>
#include <vclib/base/concepts/types.h>

#include <bit>
#include <istream>

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

} // namespace vcl

#endif // VCL_BASE_SERIALIZATION_DESERIALIZE_H
