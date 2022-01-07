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

#include "texture_file_names.h"

namespace vcl::comp {

inline TextureFileNames::TextureFileNames()
{
}

inline uint TextureFileNames::textureNumber() const
{
	return textureNames.size();
}

inline const std::string& TextureFileNames::texture(uint i) const
{
	return textureNames[i];
}

inline std::string& TextureFileNames::texture(uint i)
{
	return textureNames[i];
}

inline void TextureFileNames::clearTextures()
{
	textureNames.clear();
}

inline void TextureFileNames::pushTexture(const std::string& textName)
{
	textureNames.push_back(textName);
}

inline TextureFileNames::TextureFileNamesIterator TextureFileNames::textureBegin()
{
	return textureNames.begin();
}

inline TextureFileNames::TextureFileNamesIterator TextureFileNames::textureEnd()
{
	return textureNames.end();
}

inline TextureFileNames::ConstTextureFileNamesIterator TextureFileNames::textureBegin() const
{
	return textureNames.begin();
}

inline TextureFileNames::ConstTextureFileNamesIterator TextureFileNames::textureEnd() const
{
	return textureNames.end();
}

inline TextureFileNames::TextureFileNamesRangeIterator TextureFileNames::textures()
{
	return TextureFileNamesRangeIterator(
		*this, &TextureFileNames::textureBegin, &TextureFileNames::textureEnd);
}

inline TextureFileNames::ConstTextureFileNamesRangeIterator TextureFileNames::textures() const
{
	return ConstTextureFileNamesRangeIterator(
		*this, &TextureFileNames::textureBegin, &TextureFileNames::textureEnd);
}

template<typename Element>
void TextureFileNames::importFrom(const Element& e)
{
	if constexpr (hasTextureFileNames<Element>()) {
		textureNames = e.TextureFileNames::textureNames;
	}
}

} // namespace vcl::comp
