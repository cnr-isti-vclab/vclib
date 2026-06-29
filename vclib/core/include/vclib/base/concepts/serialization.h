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

namespace vcl {

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
