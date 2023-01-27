/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_COMPONENTS_VERTICAL_COMPONENT_H
#define VCL_MESH_COMPONENTS_VERTICAL_COMPONENT_H

#include <vclib/misc/types.h>

namespace vcl::mesh {

template<typename T>
class ElementContainer;

}

namespace vcl::internal {

template <typename T>
class VerticalComponentsVector;

}

namespace vcl::comp {

/**
 * @brief The VerticalComponent class is a class that is meant to be inherited *virtually* by all
 * vertical components (optional or persistent).
 *
 * It provides a pointer to the container that actually stores the all the components of a mesh, in
 * order to allow to access a component without storing it internally.
 */
template<typename T>
class VerticalComponent
{
public:
	VerticalComponent() {}

	// copy and move semantics: nothing will be done here
	// this instance will be updated only by the mesh, and optional data
	// can be accessed only when an element is part of a mesh
	VerticalComponent(const VerticalComponent<T>&) {}
	VerticalComponent(VerticalComponent<T>&&) {}
	VerticalComponent& operator=(const VerticalComponent<T>&) {return *this;}

protected:
	// this function sets the pointer of the container of this istance of an element.
	// it is called by the ElementContainer when a new element with optional info is created.
	// elements created outside an Element Container will have this pointer set to nullptr.
	void setContainerPointer(vcl::mesh::ElementContainer<T>* cp) { contPtr = cp; }

	// get the reference of the optional container.
	// this function is called by this element when it wants to access to optional data, that it is
	// stored in the ElementContainer.
	// then, the OptionalComponent of this element will access to the desired component stored in
	// the optionalVec.
	vcl::internal::VerticalComponentsVector<T>& optCont()
	{
		assert(contPtr != nullptr); // be sure that this element is part of an ElementContainer
		return contPtr->optionalVec;
	}

	// get the const reference of the optional container.
	// this function is called by this element when it wants to access to optional data, that it is
	// stored in the ElementContainer.
	// then, the OptionalComponent of this element will access to the desired component stored in
	// the optionalVec.
	const vcl::internal::VerticalComponentsVector<T>& optCont() const
	{
		assert(contPtr != nullptr); // be sure that this element is part of an ElementContainer
		return contPtr->optionalVec;
	}

	// compute the index of this element.
	// this function is called by the element when it wants to access to optional data, and it needs
	// to know its index in the optional components vector.
	uint index(const T* e) const
	{
		assert(contPtr != nullptr); // be sure that this element is part of an ElementContainer
		// sanity check of this element pointer w.r.t. its container; should be always fine...
		assert(!contPtr->vec.empty() && e >= contPtr->vec.data() && e <= &contPtr->vec.back());
		return e - contPtr->vec.data();
	}

	vcl::mesh::ElementContainer<T>* contPtr = nullptr;
};

/**
 * Detector to check if a class has (inherits) VerticalComponent
 */

template<typename T>
concept HasVerticalComponent =
#ifndef __APPLE__
	std::derived_from<T, VerticalComponent<T>>;
#else
	std::is_base_of<VerticalComponent<T>, T>::value;
#endif

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_VERTICAL_COMPONENT_H
