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

#include "internal/component_data.h"

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
class ElementPointersContainer : public PointersComponentTriggerer<Elem>
{
private:
	using Base = Vector<Elem*, N>;

public:
	/** @private data that the component stores internally (or vertically) */
	using DataValueType = Vector<Elem*, N>;

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

protected:
	static const int CONTAINER_SIZE = Base::SIZE;

	/* Iterator Types declaration */

	using Iterator              = typename Base::Iterator;
	using ConstIterator         = typename Base::ConstIterator;

	/* Constructor and isEnabled */

	ElementPointersContainer();

	template<typename Comp>
	void init(Comp* comp);

	template<typename Comp>
	bool isEnabled(Comp* comp) const;

	template<typename Comp>
	void updateElementPointers(const Elem* oldBase, const Elem* newBase, Comp* comp);

	template<typename Comp>
	void updateElementPointersAfterCompact(const Elem* base, const std::vector<int>& newIndices, Comp* comp);

	template<typename Comp>
	Vector<Elem*, N>& container(Comp* comp);

	template<typename Comp>
	const Vector<Elem*, N>& container(const Comp* comp) const;

private:
	internal::ComponentData<DataValueType, IS_VERTICAL> data;
};

} // namespace vcl::comp

#include "element_pointers_container.cpp"

#endif // VCL_MESH_COMPONENTS_ELEMENT_POINTERS_CONTAINER_H
