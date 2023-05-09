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

#ifndef VCL_MESH_COMPONENTS_ELEMENT_POINTERS_CONTAINER_H
#define VCL_MESH_COMPONENTS_ELEMENT_POINTERS_CONTAINER_H

#include <vclib/concepts/mesh/components/component.h>
#include <vclib/space/vector.h>

#include "component.h"

namespace vcl::comp {

/**
 * @brief The ElementPointersContainer class is a generic container of pointers to another Element
 * (that could be Vertex, Face...). This class is meant to be inherited and used by Components
 * like Vertex Pointers or Adjacent Elements : in general, a class that need to store a static or
 * dynamic number of pointers of another element type.
 *
 * Its major use is for adjacencies.
 */
template<typename Elem, int N, typename ElementType, bool optional>
class ElementPointersContainer :
		public Component<Vector<Elem*, N>, ElementType, optional, false, N, Elem>
{
private:
	using Base = Component<Vector<Elem*, N>, ElementType, optional, false, N, Elem>;

protected:
	static const int CONTAINER_SIZE = Vector<Elem*, N>::SIZE;

	/* Iterator Types declaration */

	using Iterator              = typename Vector<Elem*, N>::Iterator;
	using ConstIterator         = typename Vector<Elem*, N>::ConstIterator;

	/* Constructor and isEnabled */

	ElementPointersContainer();

	template<typename Comp>
	void init(Comp* comp);

	template<typename Comp>
	void updateElementPointers(const Elem* oldBase, const Elem* newBase, Comp* comp);

	template<typename Comp>
	void updateElementPointersAfterCompact(const Elem* base, const std::vector<int>& newIndices, Comp* comp);

	template<typename Comp>
	Vector<Elem*, N>& container(Comp* comp);

	template<typename Comp>
	const Vector<Elem*, N>& container(const Comp* comp) const;
};

} // namespace vcl::comp

#include "element_pointers_container.cpp"

#endif // VCL_MESH_COMPONENTS_ELEMENT_POINTERS_CONTAINER_H
