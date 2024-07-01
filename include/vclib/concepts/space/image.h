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

#ifndef VCL_CONCEPTS_SPACE_IMAGE_H
#define VCL_CONCEPTS_SPACE_IMAGE_H

#include <string>

#include <vclib/types.h>

#include "color.h"

namespace vcl {

template<typename T>
concept ImageConcept = requires (T&& o) {
    // clang-format off
    { o.isNull() } -> std::same_as<bool>;
    { o.height() } -> std::same_as<int>;
    { o.width() } -> std::same_as<int>;

    { o.sizeInBytes() } -> std::same_as<std::size_t>;

    { o.pixel(uint(), uint()) }  -> ColorConcept;

    { o.data() } -> std::same_as<const unsigned char*>;

    { o.load(std::string()) } -> std::same_as<bool>;
    { o.save(std::string()) } -> std::same_as<void>;
    { o.save(std::string(), uint()) } -> std::same_as<void>;

    { o.mirror() } -> std::same_as<void>;
    { o.mirror(bool()) } -> std::same_as<void>;
    { o.mirror(bool(), bool()) } -> std::same_as<void>;

    // clang-format on
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_IMAGE_H
