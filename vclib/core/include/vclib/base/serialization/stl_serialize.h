// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_SERIALIZATION_STL_SERIALIZE_H
#define VCL_BASE_SERIALIZATION_STL_SERIALIZE_H

#include "serialize.h"

#include <any>
#include <array>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace vcl {

/// STL Serialize specializations ///

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
 * std::map
 */

template<typename K, typename V>
void serialize(std::ostream& os, const std::map<K, V>& m)
{
    std::size_t size = m.size();
    serialize(os, size);
    for (const auto& [key, value] : m) {
        if constexpr (Serializable<K>) {
            key.serialize(os);
        }
        else {
            serialize(os, key);
        }
        if constexpr (Serializable<V>) {
            value.serialize(os);
        }
        else {
            serialize(os, value);
        }
    }
}

/*
 * std::unordered_map
 */

template<typename K, typename V>
void serialize(std::ostream& os, const std::unordered_map<K, V>& m)
{
    std::size_t size = m.size();
    serialize(os, size);
    for (const auto& [key, value] : m) {
        if constexpr (Serializable<K>) {
            key.serialize(os);
        }
        else {
            serialize(os, key);
        }
        if constexpr (Serializable<V>) {
            value.serialize(os);
        }
        else {
            serialize(os, value);
        }
    }
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

template<typename T>
void serialize(std::ostream& os, const std::vector<std::any>& v)
{
    std::size_t size = v.size();
    serialize(os, size);
    if constexpr (Serializable<T>) {
        for (const std::any& e : v) {
            std::any_cast<T>(e).serialize(os);
        }
    }
    else {
        for (const std::any& e : v) {
            serialize(os, std::any_cast<T>(e));
        }
    }
}

} // namespace vcl

#endif // VCL_BASE_SERIALIZATION_STL_SERIALIZE_H
