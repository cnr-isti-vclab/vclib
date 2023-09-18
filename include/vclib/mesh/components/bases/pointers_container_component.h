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
 * @brief The PointersContainerComponent is the base class for all the
 * components of VCLib that store a container of pointers to an Element.
 *
 * A component that inherits from this class will have all the features of the
 * @ref Component and @ref ContainerComponent classes, and will have access to
 * the implementations of updateElementPointers member functions, that should be
 * used by the Derived Component.
 *
 * For further details , please refer to the page @ref implement_component page.
 *
 * @tparam DerivedComponent: The type of the Derived Component. It is used to
 * implement the CRTP pattern.
 * @tparam COMP_ID: The id of the component. It is a value of the enum
 * ComponentIDEnum, or an integer value that is not already used by any other
 * component. It is used to identify the component at compile time.
 * @tparam Elem: The type of the Element of which the pointers are stored. E.g.
 * a VertexPointers component would have VertexType as Elem.
 * @tparam N: The size of the container: if >= 0 the size is static, if < 0 the
 * size is dynamic.
 * @tparam ElementType: This type is used to discriminate between horizontal and
 * vertical components. When a component is horizontal, this type must be void.
 * When a component is vertical, this type must be the type of the Element that
 * has the component, and it will be used by the vcl::Mesh to access to the data
 * stored vertically.
 * @tparam OPT: When a component is vertical, it could be optional, that means
 * that could be enabled/disabled at runtime. To make the component optional,
 * this template parameter must be true.
 * @tparam TTVN: "Tied To Vertex Number", this boolean tells whether the
 * component should leave liberty to control the size of the Container to the
 * user, or it should be tied to the vertex number and changed automatically
 * when the number of vertices changes. E.g. a AdjacentFaces component, if part
 * of a Face element, should be tied to the vertex number of the Face, meaning
 * that the number of face pointers should be always be the same of the number
 * of vertices. Setting this value to true will tie the size of this container
 * to the number of vertices.
 */
template<
	typename DerivedComponent, // CRTP pattern, derived class
	uint COMP_ID,              // component id
	typename Elem,             // element type for which the pointers are stored
	int N,                     // container size
	typename ElementType,      // element type, void if horizontal
	bool OPT,                  // true if component vertical and optional
	bool TTVN>                 // true if container size tied to vertex number
class PointersContainerComponent :
		public ContainerComponent<
			DerivedComponent,
			COMP_ID,
			Elem*,
			N,
			void,
			ElementType,
			OPT,
			TTVN,
			Elem>
{
	using Base = ContainerComponent<
		DerivedComponent,
		COMP_ID,
		Elem*,
		N,
		void,
		ElementType,
		OPT,
		TTVN,
		Elem>;

protected:
	using Base::container;

	void updatePointers(const Elem* oldBase, const Elem* newBase);

	void updatePointers(const Elem* base, const std::vector<uint>& newIndices);
};

} // namespace vcl::comp

#include "pointers_container_component.cpp"

#endif // VCL_MESH_COMPONENTS_BASES_POINTERS_CONTAINER_COMPONENT_H
