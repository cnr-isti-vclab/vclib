// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_SERIALIZATION_SERIALIZE_H
#define VCL_BASE_SERIALIZATION_SERIALIZE_H

#include "endian.h"

#include <vclib/base/concepts/serialization.h>
#include <vclib/base/concepts/types.h>

#include <bit>
#include <ostream>

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
    std::endian   endian = std::endian::little)
{
    for (std::size_t i = 0; i < size; ++i) {
        serialize(os, data[i], endian);
    }
}

template<typename T, typename... Others>
void serialize(std::ostream& os, const T& data, const Others&... others)
{
    if constexpr (Serializable<T>) {
        data.serialize(os);
    }
    else {
        serialize(os, data, std::endian::little);
    }
    if constexpr (sizeof...(Others) > 0) {
        serialize(os, others...);
    }
}

} // namespace vcl

#endif // VCL_BASE_SERIALIZATION_SERIALIZE_H
