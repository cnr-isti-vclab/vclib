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
 * @brief The TexturePaths class represents a component that stores the paths of
 * the textures used by a mesh. This component makes sense only if it is used by
 * meshes, and therefore it cannot be stored vertically or be optional (no
 * template arguments are needed for these options).
 *
 * The TexturePaths component stores a vector of texture paths that are relative
 * to the mesh path. The mesh path is stored as well, and it is used to
 * construct the absolute paths of the textures.
 *
 * The member functions of this class will be available in the instance of any
 * Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the TexturePaths component, you'll
 * be able to access to this component member functions from `m`:
 *
 * @code{.cpp}
 * for (const auto& texPath : m.texturePaths()) {
 *     auto absPath = m.meshBasePath() + texPath;
 *     // do something with absPath
 * }
 * @endcode
 *
 * @ingroup components
 */
class TexturePaths :
		public Component<
			TexturePaths,
			TEXTURE_PATHS,
			internal::TPData,
			void,
			false>
{
	using Base =
		Component<TexturePaths, TEXTURE_PATHS, internal::TPData, void, false>;

public:
	// iterators
	using TexFileNamesIterator      = std::vector<std::string>::iterator;
	using ConstTexFileNamesIterator = std::vector<std::string>::const_iterator;

	TexturePaths();

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
	// Component interface functions
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the data
	std::vector<std::string>& texPaths();
	const std::vector<std::string>& texPaths() const;
};

} // namespace vcl::comp

#include "texture_paths.cpp"

#endif // VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
