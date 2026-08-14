// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_CONCEPTS_SERIALIZATION_H
#define VCL_BASE_CONCEPTS_SERIALIZATION_H

#include "const_correctness.h"

#include <istream>
#include <ostream>
#include <string>
#include <type_traits>

namespace vcl {

/**
 * @brief Checks if a type has a member function `.toString()` returning a
 * string.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept HasMemberToString = requires (const T& obj) {
    { obj.toString() } -> std::convertible_to<std::string>;
};

/**
 * @brief Evaluates true if T can be natively converted to string without ADL
 * `toString`.
 *
 * This includes pointers, types convertible to std::string, types with
 * `.toString()`, and types supported by `std::to_string(obj)`.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept CoreStringifiable =
    std::is_pointer_v<T> || std::is_convertible_v<T, std::string> ||
    HasMemberToString<T> || requires (const T& obj) {
        { std::to_string(obj) } -> std::convertible_to<std::string>;
    };

/**
 * @brief Concept that is evaluated true if T can be converted to a string.
 *
 * A type T is stringifiable if it satisfies `CoreStringifiable<T>` or has a
 * free function `toString(T)` (found via ADL) that returns a `std::string`.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept Stringifiable = CoreStringifiable<T> || requires (const T& obj) {
    { toString(obj) } -> std::convertible_to<std::string>;
};

/**
 * @brief Concept that is evaluated true if T is an output streamable type.
 *
 * A type T is output streamable if it can be written to an output stream, i.e.,
 * it has an overloaded operator<<.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept OutputStreamable = requires (std::ostream& os, T&& value) {
    { os << value } -> std::convertible_to<std::ostream&>;
};

/**
 * @brief Concept that is evaluated true if T is an input streamable type.
 *
 * A type T is input streamable if it can be read from an input stream, i.e.,
 * it has an overloaded operator>>.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept InputStreamable = requires (std::istream& is, T&& value) {
    { is >> value } -> std::convertible_to<std::istream&>;
};

/**
 * @brief Checks if a type has a static `T::fromString(const std::string&)`
 * method.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept HasStaticFromString = requires (const std::string& str) {
    { T::fromString(str) } -> std::convertible_to<T>;
};

/**
 * @brief Defines all types that can be natively parsed from a string.
 *
 * This includes std::string itself, types with a static `T::fromString`,
 * and types supported by std::istream (like int, float).
 *
 * @ingroup util_concepts
 */
template<typename T>
concept CoreParsable = std::is_same_v<std::decay_t<T>, std::string> ||
                       HasStaticFromString<T> || InputStreamable<T>;

/**
 * @brief Concept that is evaluated true if T can be parsed from a string.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept Parsable = CoreParsable<T>;

/**
 * @brief Concept that is evaluated true if T is serializable.
 *
 * A type T is serializable if it can be written to an output stream and read
 * from an input stream, through the methods `serialize` and `deserialize`.
 *
 * @ingroup util_concepts
 */
template<typename T>
concept Serializable = requires (T&& obj, std::ostream& os, std::istream& is) {
    { obj.serialize(os) } -> std::same_as<void>;
    requires IsConst<T> || requires {
        { obj.deserialize(is) } -> std::same_as<void>;
    };
};

} // namespace vcl

#endif // VCL_BASE_CONCEPTS_SERIALIZATION_H
