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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_NAME_H
#define VCL_MESH_COMPONENTS_CONCEPTS_NAME_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasName concept is satisfied only if a Element or Mesh class provides the member
 * functions specified in this concept. These member functions allows to access to a Name
 * component of a given element/mesh.
 */
template<typename T>
concept HasName = requires(
	T o,
	const T& co)
{
	{ o.name() } -> std::same_as<std::string&>;
	{ co.name() } -> std::same_as<const std::string&>;
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_NAME_H
