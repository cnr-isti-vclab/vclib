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

template<typename El, bool O>
uint TexturePaths<El, O>::textureNumber() const
{
	return texPaths().size();
}

template<typename El, bool O>
const std::string& TexturePaths<El, O>::texturePath(uint i) const
{
	return texPaths()[i];
}

template<typename El, bool O>
std::string& TexturePaths<El, O>::texturePath(uint i)
{
	return texPaths()[i];
}

template<typename El, bool O>
const std::string &TexturePaths<El, O>::meshBasePath() const
{
	return meshPath();
}

template<typename El, bool O>
std::string &TexturePaths<El, O>::meshBasePath()
{
	return meshPath();
}

template<typename El, bool O>
void TexturePaths<El, O>::clearTexturePaths()
{
	texPaths().clear();
}

template<typename El, bool O>
void TexturePaths<El, O>::pushTexturePath(const std::string& textName)
{
	texPaths().push_back(textName);
}

template<typename El, bool O>
typename TexturePaths<El, O>::TexFileNamesIterator TexturePaths<El, O>::texturePathBegin()
{
	return texPaths().begin();
}

template<typename El, bool O>
typename TexturePaths<El, O>::TexFileNamesIterator TexturePaths<El, O>::texturePathEnd()
{
	return texPaths().end();
}

template<typename El, bool O>
typename TexturePaths<El, O>::ConstTexFileNamesIterator TexturePaths<El, O>::texturePathBegin() const
{
	return texPaths().begin();
}

template<typename El, bool O>
typename TexturePaths<El, O>::ConstTexFileNamesIterator TexturePaths<El, O>::texturePathEnd() const
{
	return texPaths().end();
}

template<typename El, bool O>
auto TexturePaths<El, O>::texturePaths()
{
	return View(texturePathBegin(), texturePathEnd());
}

template<typename El, bool O>
auto TexturePaths<El, O>::texturePaths() const
{
	return View(texturePathBegin(), texturePathEnd());
}

template<typename El, bool O>
template<typename Element>
void TexturePaths<El, O>::importFrom(const Element& e)
{
	if constexpr (HasTexturePaths<Element>) {
		texPaths() = e.texPaths();
		meshPath() = e.meshPath();
	}
}

template<typename El, bool O>
std::vector<std::string>& TexturePaths<El, O>::texPaths()
{
	return Base::data(this).texPaths;
}

template<typename El, bool O>
const std::vector<std::string>& TexturePaths<El, O>::texPaths() const
{
	return Base::data(this).texPaths;
}

template<typename El, bool O>
std::string& TexturePaths<El, O>::meshPath()
{
	return Base::data(this).meshPath;
}

template<typename El, bool O>
const std::string& TexturePaths<El, O>::meshPath() const
{
	return Base::data(this).meshPath;
}

} // namespace vcl::comp
