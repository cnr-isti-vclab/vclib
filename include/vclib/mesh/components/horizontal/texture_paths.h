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

#ifndef VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
#define VCL_MESH_COMPONENTS_TEXTURE_PATHS_H

#include <string>
#include <vector>

#include <vclib/iterators/range_iterator.h>

#include "../detection/texture_paths_detection.h"

namespace vcl::comp {

class TexturePaths
{
public:
	// iterators
	using TextureFileNamesIterator      = std::vector<std::string>::iterator;
	using ConstTextureFileNamesIterator = std::vector<std::string>::const_iterator;
	using TextureFileNamesRangeIterator =
		RangeIterator<TexturePaths, TextureFileNamesIterator>;
	using ConstTextureFileNamesRangeIterator =
		ConstRangeIterator<TexturePaths, ConstTextureFileNamesIterator>;

	TexturePaths();
	uint textureNumber() const;

	const std::string& texturePath(uint i) const;
	std::string&       texturePath(uint i);
	const std::string& meshBasePath() const;
	std::string&       meshBasePath();

	void clearTextures();

	void pushTexturePath(const std::string& textName);

	TextureFileNamesIterator texturePathBegin();
	TextureFileNamesIterator texturePathEnd();
	ConstTextureFileNamesIterator texturePathBegin() const;
	ConstTextureFileNamesIterator texturePathEnd() const;
	TextureFileNamesRangeIterator texturePaths();
	ConstTextureFileNamesRangeIterator texturePaths() const;

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	std::vector<std::string> texPaths;
	std::string meshPath;
};

} // namespace vcl::comp

#include "texture_paths.cpp"

#endif // VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
