// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_SERIALIZATION_STL_DESERIALIZE_H
#define VCL_BASE_SERIALIZATION_STL_DESERIALIZE_H

#include "deserialize.h"

#include <any>
#include <array>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace vcl {

/// STL Deserialize specializations ///

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
 * map
 */

template<typename K, typename V>
void deserialize(std::istream& is, std::map<K, V>& m)
{
    m.clear();
    std::size_t size;
    deserialize(is, size);
    for (std::size_t i = 0; i < size; ++i) {
        K key;
        if constexpr (Serializable<K>) {
            key.deserialize(is);
        }
        else {
            deserialize(is, key);
        }

        V value;
        if constexpr (Serializable<V>) {
            value.deserialize(is);
        }
        else {
            deserialize(is, value);
        }
        m.insert({std::move(key), std::move(value)});
    }
}

/*
 * unordered_map
 */

template<typename K, typename V>
void deserialize(std::istream& is, std::unordered_map<K, V>& m)
{
    m.clear();
    std::size_t size;
    deserialize(is, size);
    for (std::size_t i = 0; i < size; ++i) {
        K key;
        if constexpr (Serializable<K>) {
            key.deserialize(is);
        }
        else {
            deserialize(is, key);
        }

        V value;
        if constexpr (Serializable<V>) {
            value.deserialize(is);
        }
        else {
            deserialize(is, value);
        }
        m.insert({std::move(key), std::move(value)});
    }
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

#endif // VCL_BASE_SERIALIZATION_STL_DESERIALIZE_H
