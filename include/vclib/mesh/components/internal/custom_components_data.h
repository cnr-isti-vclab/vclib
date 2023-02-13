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

#ifndef VCL_MESH_COMPONENTS_INTERNAL_CUSTOM_COMPONENTS_DATA_H
#define VCL_MESH_COMPONENTS_INTERNAL_CUSTOM_COMPONENTS_DATA_H

#include <any>
#include <unordered_map>
#include <typeindex>

namespace vcl::comp::internal {

//store the data if horizontal

template<bool vertical>
struct CustomComponentsData
{
	std::unordered_map<std::string, std::any> map;
	std::unordered_map<std::string, std::type_index> componentType;
};

template<>
struct CustomComponentsData<true>
{
};

} // namespace vcl::comp::internal

#endif // VCL_MESH_COMPONENTS_INTERNAL_CUSTOM_COMPONENTS_DATA_H
