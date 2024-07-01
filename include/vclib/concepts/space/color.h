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

#ifndef VCL_CONCEPTS_SPACE_COLOR_H
#define VCL_CONCEPTS_SPACE_COLOR_H

#include "point.h"

namespace vcl {

/**
 * @brief ColorConcept is satisfied only if a class provides the member functions
 * specified in this concept. These member functions allows to access to the
 * color components of a given color.
 *
 * @ingroup space_concepts
 */
template<typename T>
concept ColorConcept = requires (T&& o) {
    // clang-format off
    vcl::Point4Concept<T>;

    { o.red() } -> std::same_as<uint8_t&>;
    { o.green() } -> std::same_as<uint8_t&>;
    { o.blue() } -> std::same_as<uint8_t&>;
    { o.alpha() } -> std::same_as<uint8_t&>;

    { o.redF() } -> std::same_as<float>;
    { o.greenF() } -> std::same_as<float>;
    { o.blueF() } -> std::same_as<float>;
    { o.alphaF() } -> std::same_as<float>;
    // clang-format on
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_COLOR_H
