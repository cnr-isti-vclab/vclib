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

template<typename El, bool h>
uint TexturePaths<El, h>::textureNumber() const
{
	return texPaths().size();
}

template<typename El, bool h>
const std::string& TexturePaths<El, h>::texturePath(uint i) const
{
	return texPaths()[i];
}

template<typename El, bool h>
std::string& TexturePaths<El, h>::texturePath(uint i)
{
	return texPaths()[i];
}

template<typename El, bool h>
const std::string &TexturePaths<El, h>::meshBasePath() const
{
	return meshPath();
}

template<typename El, bool h>
std::string &TexturePaths<El, h>::meshBasePath()
{
	return meshPath();
}

template<typename El, bool h>
void TexturePaths<El, h>::clearTexturePaths()
{
	texPaths().clear();
}

template<typename El, bool h>
void TexturePaths<El, h>::pushTexturePath(const std::string& textName)
{
	texPaths().push_back(textName);
}

template<typename El, bool h>
typename TexturePaths<El, h>::TexFileNamesIterator TexturePaths<El, h>::texturePathBegin()
{
	return texPaths().begin();
}

template<typename El, bool h>
typename TexturePaths<El, h>::TexFileNamesIterator TexturePaths<El, h>::texturePathEnd()
{
	return texPaths().end();
}

template<typename El, bool h>
typename TexturePaths<El, h>::ConstTexFileNamesIterator TexturePaths<El, h>::texturePathBegin() const
{
	return texPaths().begin();
}

template<typename El, bool h>
typename TexturePaths<El, h>::ConstTexFileNamesIterator TexturePaths<El, h>::texturePathEnd() const
{
	return texPaths().end();
}

template<typename El, bool h>
typename TexturePaths<El, h>::TexFileNamesRangeIterator TexturePaths<El, h>::texturePaths()
{
	return TexFileNamesRangeIterator(
		*this, &TexturePaths::texturePathBegin, &TexturePaths::texturePathEnd);
}

template<typename El, bool h>
typename TexturePaths<El, h>::ConstTexFileNamesRangeIterator TexturePaths<El, h>::texturePaths() const
{
	return ConstTexFileNamesRangeIterator(
		*this, &TexturePaths::texturePathBegin, &TexturePaths::texturePathEnd);
}

template<typename El, bool h>
template<typename Element>
void TexturePaths<El, h>::importFrom(const Element& e)
{
	if constexpr (HasTexturePaths<Element>) {
		texPaths() = e.texPaths();
		meshPath() = e.meshPath();
	}
}

template<typename El, bool h>
std::vector<std::string>& TexturePaths<El, h>::texPaths()
{
	return data.template get<El>(this).texPaths;
}

template<typename El, bool h>
const std::vector<std::string>& TexturePaths<El, h>::texPaths() const
{
	return data.template get<El>(this).texPaths;
}

template<typename El, bool h>
std::string& TexturePaths<El, h>::meshPath()
{
	return data.template get<El>(this).meshPath;
}

template<typename El, bool h>
const std::string& TexturePaths<El, h>::meshPath() const
{
	return data.template get<El>(this).meshPath;
}

} // namespace vcl::comp
