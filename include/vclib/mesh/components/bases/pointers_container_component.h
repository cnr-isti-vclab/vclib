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

#ifndef VCL_MESH_COMPONENTS_BASES_POINTERS_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_POINTERS_CONTAINER_COMPONENT_H

#include <vclib/space/vector.h>

#include "container_component.h"

namespace vcl::comp {

/**
 * @brief The PointersContainerComponent class is a generic component that stores a container of
 * pointers to another Element (that could be Vertex, Face...). This class is meant to be inherited
 * and used by Components like Vertex Pointers or Adjacent Elements : in general, a class that need
 * to store a static or dynamic number of pointers of another element type.
 *
 * Its major use is for adjacencies.
 */
template<typename Elem, int N, typename ElementType, bool optional>
class PointersContainerComponent :
		public ContainerComponent<Elem*, N, void, ElementType, optional, true, Elem>
{
	using Base = ContainerComponent<Elem*, N, void, ElementType, optional, true, Elem>;

protected:
	using Base::container;

	template<typename Comp>
	void updateElementPointers(const Elem* oldBase, const Elem* newBase, Comp* comp);

	template<typename Comp>
	void updateElementPointersAfterCompact(
		const Elem*             base,
		const std::vector<int>& newIndices,
		Comp*                   comp);
};

} // namespace vcl::comp

#include "pointers_container_component.cpp"

#endif // VCL_MESH_COMPONENTS_BASES_POINTERS_CONTAINER_COMPONENT_H
