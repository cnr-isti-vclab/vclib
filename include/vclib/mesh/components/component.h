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

#ifndef VCL_MESH_COMPONENTS_COMPONENT_H
#define VCL_MESH_COMPONENTS_COMPONENT_H

#include <vclib/concepts/mesh/components/component.h>

namespace vcl::comp {

template<
	typename DataType,
	typename ElementType,
	bool optional,
	bool TTVN = false,
	int N = 0,
	typename... PointedTypes>
class Component : public PointersComponentTriggerer<PointedTypes>...
{
public:
	using DataValueType = DataType;

	/**
	 * @brief Boolean that tells if this component type stores its data vertically (not in the
	 * Element frame memory, but in another vector).
	 */
	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;

	/**
	 * @brief Boolean that tells if this component is optional. Makes sense only when the component
	 * is vertical.
	 */
	static const bool IS_OPTIONAL = optional;


};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_COMPONENT_H
