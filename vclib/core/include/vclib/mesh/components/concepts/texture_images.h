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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_TEXTURE_IMAGES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_TEXTURE_IMAGES_H

#include "texture_paths.h"

#include <vclib/space/core.h>

#include <string>

namespace vcl::comp {

/**
 * @brief HasTextureImages concept is satisfied only if a Mesh class
 * provides the member functions specified in this concept. These member
 * functions allows to access to a @ref vcl::comp::TextureImages component of a
 * given mesh.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTextureImages =
    HasTexturePaths<T> &&
    requires (T&& obj, typename RemoveRef<T>::TextureType t) {
        typename RemoveRef<T>::TextureType;
        typename RemoveRef<T>::TextureIterator;
        typename RemoveRef<T>::ConstTextureIterator;

        { obj.texture(uint()) } -> TextureConcept;

        { obj.textureBegin() } -> InputIterator<decltype(t)>;
        { obj.textureEnd() } -> InputIterator<decltype(t)>;
        { obj.textures() } -> InputRange<decltype(t)>;

        // non const requirements
        requires IsConst<T> || requires {
            { obj.clearTextures() } -> std::same_as<void>;
            { obj.pushTexture(std::string()) } -> std::same_as<void>;
            { obj.pushTexture(t) } -> std::same_as<void>;

            { obj.textureBegin() } -> OutputIterator<decltype(t)>;
            { obj.textureEnd() } -> OutputIterator<decltype(t)>;
            { obj.textures() } -> OutputRange<decltype(t)>;
        };
    };

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_TEXTURE_IMAGES_H
