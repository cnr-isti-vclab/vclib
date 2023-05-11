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

#ifndef VCL_MESH_ELEMENTS_ELEMENT_H
#define VCL_MESH_ELEMENTS_ELEMENT_H

#include <vclib/concepts/mesh/elements/element.h>

#include <vclib/concepts/mesh/components/component.h>
#include <vclib/mesh/components/parent_mesh_pointer.h>
#include <vclib/types.h>

/**
 * @defgroup elements Elements
 * @ingroup mesh
 *
 * @brief List of all the Element classes, along with their concepts and functions.
 */

namespace vcl::mesh {

template<ElementConcept>
class ElementContainer;

} // namespace vcl::mesh

namespace vcl {

/**
 * @brief The Element class
 *
 * @ingroup elements
 */
template <uint ELEM_TYPE, typename MeshType, typename... Comps>
class Element : public comp::ParentMeshPointer<MeshType>, public Comps...
{
	template<ElementConcept>
	friend class mesh::ElementContainer;

public:
	using ParentMeshType = MeshType;
	using Components = TypeWrapper<Comps...>;

	static const uint ELEMENT_TYPE = ELEM_TYPE;

	uint index() const;

	template<typename ElType>
	void importFrom(const ElType& v);

private:
	// hide init and isEnabled members
	void init() {}
	bool isEnabled() { return true; }

	// init to call after set parent mesh
	void initVerticalComponents();

	template<typename Comp>
	void construct();
};

} // namespace vcl

#include "element.cpp"

#endif // VCL_MESH_ELEMENTS_ELEMENT_H
