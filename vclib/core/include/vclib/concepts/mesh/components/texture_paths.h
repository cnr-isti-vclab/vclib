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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H

#include <vclib/concepts/const_correctness.h>
#include <vclib/concepts/iterators.h>
#include <vclib/concepts/ranges/range.h>

#include <string>

namespace vcl::comp {

/**
 * @brief HasTexturePaths concept is satisfied only if a Mesh class
 * provides the member functions specified in this concept. These member
 * functions allows to access to a @ref vcl::comp::TexturePaths component of a
 * given mesh.
 *
 * @note This concept is satisfied also if a Mesh provides the @ref
 * vcl::comp::TextureImages component, since the texture paths are stored along
 * with the textures.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTexturePaths = requires (T&& obj) {
    typename RemoveRef<T>::TexFileNamesIterator;
    typename RemoveRef<T>::ConstTexFileNamesIterator;

    { obj.textureNumber() } -> std::same_as<uint>;
    { obj.texturePath(uint()) } -> std::convertible_to<std::string>;

    { obj.meshBasePath() } -> std::convertible_to<std::string>;

    { obj.texturePathBegin() } -> InputIterator<std::string>;
    { obj.texturePathEnd() } -> InputIterator<std::string>;
    { obj.texturePaths() } -> InputRange<std::string>;

    // non const requirements
    requires IsConst<T> || requires {
        { obj.texturePath(uint()) } -> std::same_as<std::string&>;
        { obj.meshBasePath() } -> std::same_as<std::string&>;

        { obj.clearTexturePaths() } -> std::same_as<void>;
        { obj.pushTexturePath(std::string()) } -> std::same_as<void>;

        { obj.texturePathBegin() } -> OutputIterator<std::string>;
        { obj.texturePathEnd() } -> OutputIterator<std::string>;
        { obj.texturePaths() } -> OutputRange<std::string>;
    };
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H
