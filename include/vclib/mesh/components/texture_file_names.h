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

#ifndef VCL_MESH_COMPONENTS_TEXTURE_FILE_NAMES_H
#define VCL_MESH_COMPONENTS_TEXTURE_FILE_NAMES_H

#include <string>
#include <vector>

#include <vclib/misc/vcl_types.h>

namespace vcl::comp {

class TextureFileNames {
public:
	TextureFileNames();
	uint textureNumber() const;

	const std::string& texture(uint i) const;
	std::string& texture(uint i);

	void clearTextures();

	void pushTexture(const std::string& textName);
private:
	std::vector<std::string> textureNames;
};

/**
 * Detector to check if a class has (inherits) TextureFileNames
 */

template<typename T>
using hasTextureFileNamesT = std::is_base_of<TextureFileNames, T>;

template<typename T>
constexpr bool hasTextureFileNames()
{
	return hasTextureFileNamesT<T>::value;
}

}

#include "texture_file_names.cpp"

#endif // VCL_MESH_COMPONENTS_TEXTURE_FILE_NAMES_H
