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

#ifndef VCL_SERIALIZATION_STL_DESERIALIZE_H
#define VCL_SERIALIZATION_STL_DESERIALIZE_H

#include "deserialize.h"

#include <any>
#include <array>
#include <string>
#include <vector>

namespace vcl {

/// STL Deserialize specializations ///

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

template<typename T>
void deserialize(std::istream& is, std::vector<std::any>& v)
{
    std::size_t size;
    deserialize(is, size);
    v.resize(size);
    if constexpr (Serializable<T>) {
        for (std::any& e : v) {
            T obj;
            obj.deserialize(is);
            e = std::move(obj);
        }
    }
    else {
        for (std::any& e : v) {
            T obj;
            deserialize(is, obj);
            e = std::move(obj);
        }
    }
}

} // namespace vcl

#endif // VCL_SERIALIZATION_STL_DESERIALIZE_H
