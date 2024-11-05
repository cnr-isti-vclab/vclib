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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H

#include <string>

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasTexturePaths concept is satisfied only if a Element or Mesh class
 * provides the member functions specified in this concept. These member
 * functions allows to access to a @ref vcl::comp::TexturePaths component of a
 * given element/mesh.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTexturePaths = requires (
    T                                     obj,
    const T&                              cObj,
    typename T::TexFileNamesIterator      it,
    typename T::ConstTexFileNamesIterator cIt,
    std::string                           str) {
    typename T::TexFileNamesIterator;
    typename T::ConstTexFileNamesIterator;

    { cObj.textureNumber() } -> std::same_as<uint>;
    { cObj.texturePath(uint()) } -> std::same_as<const std::string&>;
    { obj.texturePath(uint()) } -> std::same_as<std::string&>;
    { cObj.meshBasePath() } -> std::same_as<const std::string&>;
    { obj.meshBasePath() } -> std::same_as<std::string&>;

    { obj.clearTexturePaths() } -> std::same_as<void>;
    { obj.pushTexturePath(str) } -> std::same_as<void>;

    { obj.texturePathBegin() } -> std::same_as<decltype(it)>;
    { obj.texturePathEnd() } -> std::same_as<decltype(it)>;
    { cObj.texturePathBegin() } -> std::same_as<decltype(cIt)>;
    { cObj.texturePathEnd() } -> std::same_as<decltype(cIt)>;
    obj.texturePaths();
    cObj.texturePaths();
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H
