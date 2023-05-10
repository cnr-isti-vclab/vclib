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

#include "bases/component.h"

namespace vcl::comp {

namespace internal {

struct TPData {
	std::vector<std::string> texPaths;
	std::string meshPath;
};

} // namespace vcl::comp::internal

/**
 * @brief The TexturePaths class
 *
 * @ingroup components
 */
template<typename ElementType = void, bool optional = false>
class TexturePaths : public Component<internal::TPData, ElementType, optional>
{
	using Base = Component<internal::TPData, ElementType, optional>;
	using ThisType = TexturePaths<ElementType, optional>;

public:
	using TexturePathsComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	// iterators
	using TexFileNamesIterator      = std::vector<std::string>::iterator;
	using ConstTexFileNamesIterator = std::vector<std::string>::const_iterator;

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
	auto                      texturePaths();
	auto                      texturePaths() const;

protected:
	// PointersComponent interface functions
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the data
	std::vector<std::string>& texPaths();
	const std::vector<std::string>& texPaths() const;
	std::string& meshPath();
	const std::string& meshPath() const;
};

} // namespace vcl::comp

#include "texture_paths.cpp"

#endif // VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
