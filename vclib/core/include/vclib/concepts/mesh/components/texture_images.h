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

#include "component.h"
#include "texture_paths.h"

#include <vclib/concepts/const_correctness.h>
#include <vclib/concepts/ranges/range.h>
#include <vclib/concepts/space/texture.h>

#include <string>

namespace vcl::comp {

/**
 * @brief HasTextureImages concept is satisfied only if a Element or Mesh class
 * provides the member functions specified in this concept. These member
 * functions allows to access to a @ref vcl::comp::TextureImages component of a
 * given element/mesh.
 *
 * @ingroup components_concepts
 */
// TODO: reuse HasTexturePaths concept
template<typename T>
concept HasTextureImages =
    requires (T&& obj, typename RemoveRef<T>::TextureType t) {
        typename RemoveRef<T>::TextureType;
        typename RemoveRef<T>::TextureIterator;
        typename RemoveRef<T>::ConstTextureIterator;

        { obj.textureNumber() } -> std::same_as<uint>;
        { obj.texture(uint()) } -> TextureConcept;

        { obj.meshBasePath() } -> std::convertible_to<std::string>;

        { obj.textureBegin() } -> std::input_iterator;
        { obj.textureEnd() } -> std::input_iterator;
        { obj.textures() } -> vcl::RangeOf<decltype(t)>;

        // non const requirements
        requires vcl::IsConst<T> || requires {
            { obj.meshBasePath() } -> std::same_as<std::string&>;

            { obj.clearTextures() } -> std::same_as<void>;
            { obj.pushTexture(std::string()) } -> std::same_as<void>;
            { obj.pushTexture(t) } -> std::same_as<void>;

            { obj.textureBegin() } -> std::output_iterator<decltype(t)>;
            { obj.textureEnd() } -> std::output_iterator<decltype(t)>;
        };
    };

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_IMAGES_H
