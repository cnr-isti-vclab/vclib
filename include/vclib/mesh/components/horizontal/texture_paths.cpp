/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

inline TexturePaths::TexturePaths()
{
}

inline uint TexturePaths::textureNumber() const
{
	return texPaths.size();
}

inline const std::string& TexturePaths::texturePath(uint i) const
{
	return texPaths[i];
}

inline std::string& TexturePaths::texturePath(uint i)
{
	return texPaths[i];
}

inline const std::string& TexturePaths::meshBasePath() const
{
	return meshPath;
}

inline std::string& TexturePaths::meshBasePath()
{
	return meshPath;
}

inline void TexturePaths::clearTextures()
{
	texPaths.clear();
}

inline void TexturePaths::pushTexturePath(const std::string& textName)
{
	texPaths.push_back(textName);
}

inline TexturePaths::TextureFileNamesIterator TexturePaths::texturePathBegin()
{
	return texPaths.begin();
}

inline TexturePaths::TextureFileNamesIterator TexturePaths::texturePathEnd()
{
	return texPaths.end();
}

inline TexturePaths::ConstTextureFileNamesIterator TexturePaths::texturePathBegin() const
{
	return texPaths.begin();
}

inline TexturePaths::ConstTextureFileNamesIterator TexturePaths::texturePathEnd() const
{
	return texPaths.end();
}

inline TexturePaths::TextureFileNamesRangeIterator TexturePaths::texturePaths()
{
	return TextureFileNamesRangeIterator(
		*this, &TexturePaths::texturePathBegin, &TexturePaths::texturePathEnd);
}

inline TexturePaths::ConstTextureFileNamesRangeIterator TexturePaths::texturePaths() const
{
	return ConstTextureFileNamesRangeIterator(
		*this, &TexturePaths::texturePathBegin, &TexturePaths::texturePathEnd);
}

template<typename Element>
void TexturePaths::importFrom(const Element& e)
{
	if constexpr (hasTexturePaths<Element>()) {
		texPaths = e.TexturePaths::texPaths;
	}
}

} // namespace vcl::comp
