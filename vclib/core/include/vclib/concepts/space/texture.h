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

#ifndef VCL_CONCEPTS_SPACE_TEXTURE_H
#define VCL_CONCEPTS_SPACE_TEXTURE_H

#include "image.h"

#include <string>

namespace vcl {

template<typename T>
concept TextureConcept = requires (T&& obj) {
    // constructors
    RemoveRef<T>();
    RemoveRef<T>(std::string());

    { obj.path() } -> std::convertible_to<std::string>;
    { obj.image() } -> ImageConcept;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.path() } -> std::same_as<std::string&>;
    };
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_TEXTURE_H
