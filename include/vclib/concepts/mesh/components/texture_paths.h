/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H
#define VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H

#include "component.h"

#include <string>

namespace vcl::comp {

/**
 * @brief HasTexturePaths concept is satisfied only if a Element or Mesh class provides the member
 * functions specified in this concept. These member functions allows to access to a TexturePaths
 * component of a given element/mesh.
 */
template<typename T>
concept HasTexturePaths = requires(
	T o,
	const T& co,
	std::string s)
{
	typename T::TexFileNamesIterator;
	typename T::ConstTexFileNamesIterator;
	typename T::TexFileNamesView;
	typename T::ConstTexFileNamesView;

	{ co.textureNumber() } -> std::same_as<uint>;
	{ co.texturePath(uint()) } -> std::same_as<const std::string&>;
	{ o.texturePath(uint()) } -> std::same_as<std::string&>;
	{ co.meshBasePath() } -> std::same_as<const std::string&>;
	{ o.meshBasePath() } -> std::same_as<std::string&>;

	{ o.clearTexturePaths() } -> std::same_as<void>;
	{ o.pushTexturePath(s) } -> std::same_as<void>;

	{ o.texturePathBegin() } -> std::same_as<typename T::TexFileNamesIterator>;
	{ o.texturePathEnd() } -> std::same_as<typename T::TexFileNamesIterator>;
	{ co.texturePathBegin() } -> std::same_as<typename T::ConstTexFileNamesIterator>;
	{ co.texturePathEnd() } -> std::same_as<typename T::ConstTexFileNamesIterator>;
	{ o.texturePaths() } -> std::same_as<typename T::TexFileNamesView>;
	{ co.texturePaths() } -> std::same_as<typename T::ConstTexFileNamesView>;
};

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_TEXTURE_PATHS_H
