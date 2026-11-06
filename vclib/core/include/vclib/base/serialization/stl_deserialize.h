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
#include <utility>
#include <vector>

namespace vcl {

namespace detail {
template<typename T>
struct CanDeserializeImpl;
} // namespace detail

/**
 * @brief Concept that evaluates true if an object can be deserialized from an
 * input stream.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept CanDeserialize = detail::CanDeserializeImpl<T>::value;

/// STL Deserialize declarations ///

inline void deserialize(std::istream& is, std::string& s);

template<CanDeserialize T, std::size_t N>
void deserialize(std::istream& is, std::array<T, N>& a);

template<CanDeserialize K, CanDeserialize V>
void deserialize(std::istream& is, std::map<K, V>& m);

template<CanDeserialize K, CanDeserialize V>
void deserialize(std::istream& is, std::unordered_map<K, V>& m);

template<CanDeserialize T>
void deserialize(std::istream& is, std::vector<T>& v);

template<CanDeserialize T>
void deserialize(std::istream& is, std::vector<std::any>& v);

template<CanDeserialize T1, CanDeserialize T2>
void deserialize(std::istream& is, std::pair<T1, T2>& p);

namespace detail {

template<typename T>
struct CanDeserializeImpl {
    static constexpr bool value = requires (std::istream& is, T& e) {
        deserialize(is, e);
    };
};

} // namespace detail

/// STL Deserialize definitions ///

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

template<CanDeserialize T, std::size_t N>
void deserialize(std::istream& is, std::array<T, N>& a)
{
    for (T& e : a) {
        deserialize(is, e);
    }
}

/*
 * map
 */

template<CanDeserialize K, CanDeserialize V>
void deserialize(std::istream& is, std::map<K, V>& m)
{
    m.clear();
    std::size_t size;
    deserialize(is, size);
    for (std::size_t i = 0; i < size; ++i) {
        K key;
        deserialize(is, key);
        V value;
        deserialize(is, value);
        m.insert({std::move(key), std::move(value)});
    }
}

/*
 * unordered_map
 */

template<CanDeserialize K, CanDeserialize V>
void deserialize(std::istream& is, std::unordered_map<K, V>& m)
{
    m.clear();
    std::size_t size;
    deserialize(is, size);
    for (std::size_t i = 0; i < size; ++i) {
        K key;
        deserialize(is, key);
        V value;
        deserialize(is, value);
        m.insert({std::move(key), std::move(value)});
    }
}

/*
 * std::vector
 */

template<CanDeserialize T>
void deserialize(std::istream& is, std::vector<T>& v)
{
    std::size_t size;
    deserialize(is, size);
    v.resize(size);
    for (T& e : v) {
        deserialize(is, e);
    }
}

template<CanDeserialize T>
void deserialize(std::istream& is, std::vector<std::any>& v)
{
    std::size_t size;
    deserialize(is, size);
    v.resize(size);
    for (std::any& e : v) {
        T obj;
        deserialize(is, obj);
        e = std::move(obj);
    }
}

/*
 * std::pair
 */

template<CanDeserialize T1, CanDeserialize T2>
void deserialize(std::istream& is, std::pair<T1, T2>& p)
{
    deserialize(is, p.first);
    deserialize(is, p.second);
}

} // namespace vcl

#endif // VCL_BASE_SERIALIZATION_STL_DESERIALIZE_H
