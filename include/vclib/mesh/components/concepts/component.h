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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_COMPONENT_H
#define VCL_MESH_COMPONENTS_CONCEPTS_COMPONENT_H

#include <vclib/misc/types.h>

namespace vcl::comp {

template<typename T>
concept HasInitMemberFunction = requires(T o)
{
	o.init();
};

template<typename T>
concept IsVerticalComponent = requires (T o)
{
	typename T::DataValueType;
	{ o.IS_VERTICAL } -> std::same_as<const bool&>;
	o.IS_VERTICAL == true;
};

template<typename T>
struct IsVerticalComponentPred
{
	static const bool value = IsVerticalComponent<T>;
};

template<typename T>
concept IsOptionalComponent = IsVerticalComponent<T> && requires(T o)
{
	{ o.IS_OPTIONAL } -> std::same_as<const bool&>;
	o.IS_OPTIONAL == true;
};

} // namespace vcl

#endif // VCL_MESH_COMPONENTS_CONCEPTS_COMPONENT_H
