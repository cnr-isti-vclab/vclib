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

#ifndef VCL_CONCEPT_MESH_COMPONENT_CUSTOM_COMPONENTS_H
#define VCL_CONCEPT_MESH_COMPONENT_CUSTOM_COMPONENTS_H

#include "component.h"

#include <string>

namespace vcl::comp {

/**
 * @brief HasCustomComponents concept is satisfied only if a Element class provides the types and
 * member functions specified in this concept. These types and member functions allow to access to a
 * CustomComponents component of a given element.
 */
template<typename T>
concept HasCustomComponents = requires(
	T o,
	const T& co,
	std::string s)
{
	{ o.hasCustomComponent( std::string() ) } -> std::same_as<bool>;
	{ o.template customComponent<int>(s) } -> std::same_as<int&>;
	{ co.template customComponent<int>(s) } -> std::same_as<const int&>;
};

} // namespace vcl::comp

#endif // VCL_CONCEPT_MESH_COMPONENT_CUSTOM_COMPONENTS_H
