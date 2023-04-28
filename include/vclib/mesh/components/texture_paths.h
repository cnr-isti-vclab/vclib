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

#ifndef VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
#define VCL_MESH_COMPONENTS_TEXTURE_PATHS_H

#include <string>
#include <vector>

#include <vclib/concepts/mesh/components/texture_paths.h>
#include <vclib/views/view.h>

#include "internal/component_data.h"

namespace vcl::comp {

/**
 * @brief The TexturePaths class
 *
 * @ingroup components
 */
template<typename ElementType = void, bool optional = false>
class TexturePaths
{
	using ThisType = TexturePaths<ElementType, optional>;

	struct TPData {
		std::vector<std::string> texPaths;
		std::string meshPath;
	};
public:
	using DataValueType = TPData; // data that the component stores internally (or vertically)
	using TexturePathsComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	// iterators
	using TexFileNamesIterator      = std::vector<std::string>::iterator;
	using ConstTexFileNamesIterator = std::vector<std::string>::const_iterator;
	using TexFileNamesView          = vcl::View<TexFileNamesIterator>;
	using ConstTexFileNamesView     = vcl::View<ConstTexFileNamesIterator>;

	uint textureNumber() const;

	const std::string& texturePath(uint i) const;
	std::string&       texturePath(uint i);
	const std::string& meshBasePath() const;
	std::string&       meshBasePath();

	void clearTexturePaths();

	void pushTexturePath(const std::string& textName);

	TexFileNamesIterator      texturePathBegin();
	TexFileNamesIterator      texturePathEnd();
	ConstTexFileNamesIterator texturePathBegin() const;
	ConstTexFileNamesIterator texturePathEnd() const;
	TexFileNamesView          texturePaths();
	ConstTexFileNamesView     texturePaths() const;

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the data
	std::vector<std::string>& texPaths();
	const std::vector<std::string>& texPaths() const;
	std::string& meshPath();
	const std::string& meshPath() const;

	internal::ComponentData<TPData, IS_VERTICAL> data;
};

} // namespace vcl::comp

#include "texture_paths.cpp"

#endif // VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
