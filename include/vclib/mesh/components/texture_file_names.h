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

#include "detection/tex_file_names_detection.h"

namespace vcl::comp {

class TextureFileNames
{
public:
	TextureFileNames();
	uint textureNumber() const;

	const std::string& texture(uint i) const;
	std::string&       texture(uint i);

	void clearTextures();

	void pushTexture(const std::string& textName);

	template<typename Element>
	void importFrom(const Element& e);

private:
	std::vector<std::string> textureNames;
};

} // namespace vcl::comp

#include "texture_file_names.cpp"

#endif // VCL_MESH_COMPONENTS_TEXTURE_FILE_NAMES_H
