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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_TEXTURES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_TEXTURES_H

#include <vclib/types.h>

namespace vcl::comp {

class TextureImages;
class TexturePaths;

/**
 * @brief A concept that checks whether a type T (that should be a Mesh)
 * has the TextureImages component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::TextureImages, with any template arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasTextureImages.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTextureImages =
    std::derived_from<std::remove_cvref_t<T>, TextureImages>;

/**
 * @brief A concept that checks whether a type T (that should be a Mesh)
 * has the TexturePaths or TextureImages component (inherits from it).
 *
 * The concept is satisfied if T is a class that inherits from
 * vcl::comp::TexturePaths or vcl::comp::TextureImages, with any template
 * arguments.
 *
 * @tparam T: The type to be tested for conformity to the HasTexturePaths.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasTexturePaths =
    HasTextureImages<T> ||
    std::derived_from<std::remove_cvref_t<T>, TexturePaths>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_TEXTURES_H
