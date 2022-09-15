/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

inline const std::string &TexturePaths::meshBasePath() const
{
	return meshPath;
}

inline std::string &TexturePaths::meshBasePath()
{
	return meshPath;
}

inline void TexturePaths::clearTexturePaths()
{
	texPaths.clear();
}

inline void TexturePaths::pushTexturePath(const std::string& textName)
{
	texPaths.push_back(textName);
}

inline TexturePaths::TexFileNamesIterator TexturePaths::texturePathBegin()
{
	return texPaths.begin();
}

inline TexturePaths::TexFileNamesIterator TexturePaths::texturePathEnd()
{
	return texPaths.end();
}

inline TexturePaths::ConstTexFileNamesIterator TexturePaths::texturePathBegin() const
{
	return texPaths.begin();
}

inline TexturePaths::ConstTexFileNamesIterator TexturePaths::texturePathEnd() const
{
	return texPaths.end();
}

inline TexturePaths::TexFileNamesRangeIterator TexturePaths::texturePaths()
{
	return TexFileNamesRangeIterator(
		*this, &TexturePaths::texturePathBegin, &TexturePaths::texturePathEnd);
}

inline TexturePaths::ConstTexFileNamesRangeIterator TexturePaths::texturePaths() const
{
	return ConstTexFileNamesRangeIterator(
		*this, &TexturePaths::texturePathBegin, &TexturePaths::texturePathEnd);
}

template<typename Element>
void TexturePaths::importFrom(const Element& e)
{
	if constexpr (HasTexturePaths<Element>) {
		texPaths = e.TexturePaths::texPaths;
		meshPath = e.TexturePaths::meshPath;
	}
}

} // namespace vcl::comp
