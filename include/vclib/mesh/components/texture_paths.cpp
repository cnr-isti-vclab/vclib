/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

template<typename El, bool h, bool o>
uint TexturePaths<El, h, o>::textureNumber() const
{
	return texPaths().size();
}

template<typename El, bool h, bool o>
const std::string& TexturePaths<El, h, o>::texturePath(uint i) const
{
	return texPaths()[i];
}

template<typename El, bool h, bool o>
std::string& TexturePaths<El, h, o>::texturePath(uint i)
{
	return texPaths()[i];
}

template<typename El, bool h, bool o>
const std::string &TexturePaths<El, h, o>::meshBasePath() const
{
	return meshPath();
}

template<typename El, bool h, bool o>
std::string &TexturePaths<El, h, o>::meshBasePath()
{
	return meshPath();
}

template<typename El, bool h, bool o>
void TexturePaths<El, h, o>::clearTexturePaths()
{
	texPaths().clear();
}

template<typename El, bool h, bool o>
void TexturePaths<El, h, o>::pushTexturePath(const std::string& textName)
{
	texPaths().push_back(textName);
}

template<typename El, bool h, bool o>
typename TexturePaths<El, h, o>::TexFileNamesIterator TexturePaths<El, h, o>::texturePathBegin()
{
	return texPaths().begin();
}

template<typename El, bool h, bool o>
typename TexturePaths<El, h, o>::TexFileNamesIterator TexturePaths<El, h, o>::texturePathEnd()
{
	return texPaths().end();
}

template<typename El, bool h, bool o>
typename TexturePaths<El, h, o>::ConstTexFileNamesIterator TexturePaths<El, h, o>::texturePathBegin() const
{
	return texPaths().begin();
}

template<typename El, bool h, bool o>
typename TexturePaths<El, h, o>::ConstTexFileNamesIterator TexturePaths<El, h, o>::texturePathEnd() const
{
	return texPaths().end();
}

template<typename El, bool h, bool o>
typename TexturePaths<El, h, o>::TexFileNamesRangeIterator TexturePaths<El, h, o>::texturePaths()
{
	return TexFileNamesRangeIterator(
		*this, &TexturePaths::texturePathBegin, &TexturePaths::texturePathEnd);
}

template<typename El, bool h, bool o>
typename TexturePaths<El, h, o>::ConstTexFileNamesRangeIterator TexturePaths<El, h, o>::texturePaths() const
{
	return ConstTexFileNamesRangeIterator(
		*this, &TexturePaths::texturePathBegin, &TexturePaths::texturePathEnd);
}

template<typename El, bool h, bool o>
template<typename Element>
void TexturePaths<El, h, o>::importFrom(const Element& e)
{
	if constexpr (HasTexturePaths<Element>) {
		texPaths() = e.texPaths();
		meshPath() = e.meshPath();
	}
}

template<typename El, bool h, bool o>
std::vector<std::string>& TexturePaths<El, h, o>::texPaths()
{
	return data.template get<El>(this).texPaths;
}

template<typename El, bool h, bool o>
const std::vector<std::string>& TexturePaths<El, h, o>::texPaths() const
{
	return data.template get<El>(this).texPaths;
}

template<typename El, bool h, bool o>
std::string& TexturePaths<El, h, o>::meshPath()
{
	return data.template get<El>(this).meshPath;
}

template<typename El, bool h, bool o>
const std::string& TexturePaths<El, h, o>::meshPath() const
{
	return data.template get<El>(this).meshPath;
}

} // namespace vcl::comp
