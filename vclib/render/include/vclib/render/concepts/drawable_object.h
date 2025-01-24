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

#ifndef VCL_RENDER_CONCEPTS_DRAWABLE_OBJECT_H
#define VCL_RENDER_CONCEPTS_DRAWABLE_OBJECT_H

#include <vclib/concepts.h>

namespace vcl {

template<typename T>
concept DrawableObjectConcept = requires (T&& obj, uint u) {
    { obj.draw(u) } -> std::same_as<void>;
    { obj.boundingBox() } -> Box3Concept;
    obj.clone();
    { obj.isVisible() } -> std::same_as<bool>;
    { obj.name() } -> std::convertible_to<std::string>;
    { obj.info() } -> std::convertible_to<std::string>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.init() } -> std::same_as<void>;
        { obj.setVisibility(bool()) } -> std::same_as<void>;
        { obj.name() } -> std::same_as<std::string&>;
        { obj.info() } -> std::same_as<std::string&>;
    };
};

} // namespace vcl

#endif // VCL_RENDER_CONCEPTS_DRAWABLE_OBJECT_H
