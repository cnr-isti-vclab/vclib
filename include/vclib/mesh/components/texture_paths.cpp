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

#include "texture_paths.h"

namespace vcl::comp {

/**
 * @brief Empty constructor. It initializes the component with an empty vector
 * of texture paths, and an empty string as mesh base path.
 */
TexturePaths::TexturePaths()
{
}

/**
 * @brief Returns the number of texture paths of the mesh.
 */
uint TexturePaths::textureNumber() const
{
	return texPaths().size();
}

/**
 * @brief Returns the path of the i-th texture of the mesh. The path is relative
 * to the mesh base path.
 * @param[in] i: The index of the texture.
 */
const std::string& TexturePaths::texturePath(uint i) const
{
	return texPaths()[i];
}

/**
 * @brief Returns a reference to the path of the i-th texture of the mesh. The
 * path is relative to the mesh base path.
 * @param[in] i: The index of the texture.
 */
std::string& TexturePaths::texturePath(uint i)
{
	return texPaths()[i];
}

/**
 * @brief Returns the mesh base path.
 */
const std::string& TexturePaths::meshBasePath() const
{
	return Base::data(this).meshPath;
}

/**
 * @brief Returns a reference to the mesh base path.
 */
std::string& TexturePaths::meshBasePath()
{
	return Base::data(this).meshPath;
}

/**
 * @brief Clears the vector of texture paths.
 */
void TexturePaths::clearTexturePaths()
{
	texPaths().clear();
}

/**
 * @brief Adds a texture path to the vector of texture paths.
 * @param[in] textName: The texture path to add.
 */
void TexturePaths::pushTexturePath(const std::string& textName)
{
	texPaths().push_back(textName);
}

/**
 * @brief Returns an iterator to the beginning of the vector of texture paths.
 * @return an iterator to the beginning of the vector of texture paths.
 */
typename TexturePaths::TexFileNamesIterator TexturePaths::texturePathBegin()
{
	return texPaths().begin();
}

/**
 * @brief Returns an iterator to the end of the vector of texture paths.
 * @return an iterator to the end of the vector of texture paths.
 */
typename TexturePaths::TexFileNamesIterator TexturePaths::texturePathEnd()
{
	return texPaths().end();
}

/**
 * @brief Returns a const iterator to the beginning of the vector of texture
 * paths.
 * @return a const iterator to the beginning of the vector of texture paths.
 */
typename TexturePaths::ConstTexFileNamesIterator
TexturePaths::texturePathBegin() const
{
	return texPaths().begin();
}

/**
 * @brief Returns a const iterator to the end of the vector of texture paths.
 * @return a const iterator to the end of the vector of texture paths.
 */
typename TexturePaths::ConstTexFileNamesIterator
TexturePaths::texturePathEnd() const
{
	return texPaths().end();
}

/**
 * @brief Returns a lightweigth view object that stores the begin and end
 * iterators of the vector of texture paths. The view
 * object exposes the iterators trough the `begin()` and `end()` member
 * functions, and therefore the returned object can be used in range-based for
 * loops:
 *
 * @code{.cpp}
 * for (auto& tpath : m.texturePaths()) {
 *     // Do something with tpath
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over the vector of texture paths.
 */
auto TexturePaths::texturePaths()
{
	return View(texturePathBegin(), texturePathEnd());
}

/**
 * @brief Returns a lightweigth const view object that stores the begin and end
 * iterators of the vector of texture paths. The view
 * object exposes the iterators trough the `begin()` and `end()` member
 * functions, and therefore the returned object can be used in range-based for
 * loops:
 *
 * @code{.cpp}
 * for (const auto& tpath : m.texturePaths()) {
 *     // Do something with tpath
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over the vector of texture paths.
 */
auto TexturePaths::texturePaths() const
{
	return View(texturePathBegin(), texturePathEnd());
}

template<typename Element>
void TexturePaths::importFrom(const Element& e)
{
	if constexpr (HasTexturePaths<Element>) {
		texPaths() = e.texPaths();
		meshBasePath() = e.meshBasePath();
	}
}

std::vector<std::string>& TexturePaths::texPaths()
{
	return Base::data(this).texPaths;
}

const std::vector<std::string>& TexturePaths::texPaths() const
{
	return Base::data(this).texPaths;
}

} // namespace vcl::comp
