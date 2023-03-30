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

#include <vclib/concept/mesh/element/element.h>

#include <vclib/concept/mesh/component/component.h>
#include <vclib/mesh/components/parent_mesh_pointer.h>
#include <vclib/types.h>

namespace vcl::mesh {

template<ElementConcept>
class ElementContainer;

} // namespace vcl::mesh

namespace vcl {

template <typename MeshType, typename... Args>
class Element : public comp::ParentMeshPointer<MeshType>, public Args...
{
	template<ElementConcept>
	friend class mesh::ElementContainer;
public:
	using ParentMeshType = MeshType;
	using Components = TypeWrapper<Args...>;

	template<typename ElType>
	void importFrom(const ElType& v);

protected:

	template<typename ElType>
	uint index() const;

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
