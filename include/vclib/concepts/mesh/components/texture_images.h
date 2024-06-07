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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_IMAGES_H
#define VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_IMAGES_H

#include <string>

#include <vclib/space/texture.h>

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasTextureImages concept is satisfied only if a Element or Mesh class
 * provides the member functions specified in this concept. These member
 * functions allows to access to a TextureImages component of a given
 * element/mesh.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTextureImages =
    requires (T o, const T& co, std::string s, vcl::Texture t) {
        // clang-format off
    typename T::TextureIterator;
    typename T::ConstTextureIterator;

    { co.textureNumber() } -> std::same_as<uint>;
    { co.texture(uint()) } -> std::same_as<const vcl::Texture&>;
    { o.texture(uint()) } -> std::same_as<vcl::Texture&>;
    { co.meshBasePath() } -> std::same_as<const std::string&>;
    { o.meshBasePath() } -> std::same_as<std::string&>;

    { o.clearTextures() } -> std::same_as<void>;
    { o.pushTexture(s) } -> std::same_as<void>;
    { o.pushTexture(t)} -> std::same_as<void>;

    { o.textureBegin() } -> std::same_as<typename T::TextureIterator>;
    { o.textureEnd() } -> std::same_as<typename T::TextureIterator>;
    { co.textureBegin() } -> std::same_as<typename T::ConstTextureIterator>;
    { co.textureEnd() } -> std::same_as<typename T::ConstTextureIterator>;
    o.textures();
    co.textures();
        // clang-format on
    };

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_IMAGES_H
