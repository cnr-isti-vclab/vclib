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

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasTextureImages concept is satisfied only if a Element or Mesh class
 * provides the member functions specified in this concept. These member
 * functions allows to access to a @ref vcl::comp::TextureImages component of a
 * given element/mesh.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTextureImages = requires (
    T obj,
    const T& cObj,
    std::string str) {
    typename T::TextureType;
    typename T::TextureIterator;
    typename T::ConstTextureIterator;

    { cObj.textureNumber() } -> std::same_as<uint>;
    { cObj.texture(uint()) } -> std::same_as<const typename T::TextureType&>;
    { obj.texture(uint()) } -> std::same_as<typename T::TextureType&>;
    { cObj.meshBasePath() } -> std::same_as<const std::string&>;
    { obj.meshBasePath() } -> std::same_as<std::string&>;

    { obj.clearTextures() } -> std::same_as<void>;
    { obj.pushTexture(str) } -> std::same_as<void>;
    { obj.pushTexture(typename T::TextureType()) } -> std::same_as<void>;

    { obj.textureBegin() } -> std::same_as<typename T::TextureIterator>;
    { obj.textureEnd() } -> std::same_as<typename T::TextureIterator>;
    { cObj.textureBegin() } -> std::same_as<typename T::ConstTextureIterator>;
    { cObj.textureEnd() } -> std::same_as<typename T::ConstTextureIterator>;
    obj.textures();
    cObj.textures();
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_IMAGES_H
