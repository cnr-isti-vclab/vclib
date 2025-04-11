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

#ifndef VCL_SERIALIZATION_STL_SERIALIZE_H
#define VCL_SERIALIZATION_STL_SERIALIZE_H

#include "serialize.h"

#include <array>
#include <string>
#include <vector>

namespace vcl {

/// STL Serialize specializations ///

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

#endif // VCL_SERIALIZATION_STL_SERIALIZE_H
