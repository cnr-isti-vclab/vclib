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

#ifndef VCL_MESH_COMPONENTS_ELEMENT_REFERENCES_H
#define VCL_MESH_COMPONENTS_ELEMENT_REFERENCES_H

#include <vclib/space/vector.h>

#include "component_data.h"

namespace vcl::comp::internal {

/**
 * @brief The ElementReferences class is a generic container of references to another Element
 * (that could be Vertex, Face...). This class is meant to be inherited and used by Components
 * like Vertex References, Face References: in general, a class that need to store a static or
 * dynamic number of references of another element.
 *
 * Its major use is for adjacencies.
 */
template<typename Elem, int N, typename ElementType>
class ElementReferences
{
private:
	using Base = Vector<Elem*, N>;

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
public:
	using DataValueType = Vector<Elem*, N>; // data that the component stores internally (or vertically)
	
	static const int CONTAINER_SIZE = Base::SIZE;

	/* Iterator Types declaration */

	using Iterator              = typename Base::Iterator;
	using ConstIterator         = typename Base::ConstIterator;

	/* Constructor and isEnabled */

	ElementReferences();

	template<typename Comp>
	void init(Comp* comp);

	template<typename Comp>
	bool isEnabled(Comp* comp) const;

protected:
	template<typename Comp>
	void updateElementReferences(const Elem* oldBase, const Elem* newBase, Comp* comp);

	template<typename Comp>
	void updateElementReferencesAfterCompact(const Elem* base, const std::vector<int>& newIndices, Comp* comp);

	template<typename Comp>
	Vector<Elem*, N>& container(Comp* comp);

	template<typename Comp>
	const Vector<Elem*, N>& container(const Comp* comp) const;

	internal::ComponentData<DataValueType, IS_VERTICAL> data;
};

} // namespace vcl::comp::internal

#include "element_references.cpp"

#endif // VCL_MESH_COMPONENTS_ELEMENT_REFERENCES_H
