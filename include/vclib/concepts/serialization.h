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

#ifndef VCL_CONCEPTS_SERIALIZATION_H
#define VCL_CONCEPTS_SERIALIZATION_H

#include <ostream>
#include <istream>

namespace vcl {

template<typename T>
concept OutputStreamable = requires (std::ostream& os, T value) {
    // clang-format off
    { os << value } -> std::convertible_to<std::ostream&>;
    // clang-format on
};

template<typename T>
concept InputStreamable = requires (std::istream& is, T& value) {
    // clang-format off
    { is >> value } -> std::convertible_to<std::istream&>;
    // clang-format on
};

template<typename T>
concept Serializable =
    requires (T& o, const T& co, std::ofstream& ofs, std::ifstream& ifs) {
    // clang-format off
    { co.serialize(ofs) } -> std::same_as<void>;
    { o.deserialize(ifs) } -> std::same_as<void>;
    // clang-format on
};

} // namespace vcl

#endif // VCL_CONCEPTS_SERIALIZATION_H
