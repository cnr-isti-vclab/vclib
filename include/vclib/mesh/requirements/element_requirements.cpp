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

#include "element_requirements.h"

namespace vcl {

/**
 * @brief Returns `true` if the given mesh has its Container of the given Element compact.
 *
 * @tparam ELEMENT_TYPE: the ElementEnumType of the ElementContainer to check.
 * @tparam MeshType: the type of the Mesh to check.
 *
 * @param m: the mesh on which check the compactness of the Container of the given Element.
 * @return `true` if the Container of the given Element is compact.
 */
template<uint ELEMENT_TYPE, typename MeshType>
bool isElementContainerCompact(const MeshType& m)
{
	return (m.template elementNumber<ELEMENT_TYPE>() == m.template elementContainerSize<ELEMENT_TYPE>());
}

/**
 * @brief Returns true if the given component is available (enabled) in the given element of the
 * input mesh m.
 *
 * This function returns `true` when the component can be used on the element, whether the component
 * is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Mesh does not have an ElementContainer of the given ElementEnumType, the function returns `false`;
 * - if the Element does not have a Component of the given ComponentEnumType, the function returns `false`;
 * - if the Element has a non-optional Component of the given ComponentEnumType, the function returns `true`;
 * - if the Element has an optional Component of the given ComponentEnumType, the function returns `true` if the
 *   component is enabled, false otherwise (this check is the only one that is made at runtime);
 *
 * @tparam ELEMENT_TYPE: the ElementEnumType of the Element to check.
 * @tparam COMPONENT_TYPE: the ComponentEnumType of the Component to check.
 * @tparam MeshType: the type of the Mesh to check.
 *
 * @param[in] m: the mesh on which check the availability of the Component in the Element.
 * @return `true` if the Component is available in the Element of the given Mesh.
 */
template<uint ELEMENT_TYPE, uint COMPONENT_TYPE, MeshConcept MeshType>
bool isPerElementComponentEnabled(const MeshType& m)
{
	if constexpr (mesh::HasElementContainer<MeshType, ELEMENT_TYPE>) {
		using Container = mesh::ContainerOfElementType<ELEMENT_TYPE, MeshType>;
		using Element = typename Container::ElementType;
		if constexpr (comp::HasOptionalComponentOfType<Element, COMPONENT_TYPE>) {
			return m.template isPerElementComponentEnabled<ELEMENT_TYPE, COMPONENT_TYPE>();
		}
		else {
			return comp::HasComponentOfType<Element, COMPONENT_TYPE>;
		}
	}
	else {
		return false;
	}
}

template<uint ELEMENT_TYPE, uint COMPONENT_TYPE, MeshConcept MeshType>
bool enableIfPerElementComponentOptional(MeshType& m)
{
	if constexpr (mesh::HasElementContainer<MeshType, ELEMENT_TYPE>) {
		using Container = mesh::ContainerOfElementType<ELEMENT_TYPE, MeshType>;
		using Element = typename Container::ElementType;
		if constexpr (comp::HasOptionalComponentOfType<Element, COMPONENT_TYPE>) {
			m.template enablePerElementComponent<ELEMENT_TYPE, COMPONENT_TYPE>();
			return true;
		}
		else {
			return comp::HasComponentOfType<Element, COMPONENT_TYPE>;
		}
	}
	else {
		return false;
	}
}

template<uint ELEMENT_TYPE, MeshConcept MeshType>
void requireElementContainerCompactness(const MeshType& m)
{
	if (!isElementContainerCompact<ELEMENT_TYPE>(m))
		throw vcl::MissingCompactnessException(
			std::string(elementEnumString<ELEMENT_TYPE>()) +
			" Container of the Mesh is not compact.");
}

template<uint ELEMENT_TYPE, uint COMPONENT_TYPE, MeshConcept MeshType>
void requirePerElementComponent(const MeshType& m)
{
	if (!isPerElementComponentEnabled<ELEMENT_TYPE, COMPONENT_TYPE>(m)) {
		throw vcl::MissingComponentException(
			"Per " + std::string(elementEnumString<ELEMENT_TYPE>()) +
			" " + std::string(componentEnumString<COMPONENT_TYPE>()) + " Component is not enabled.");
	}
}

} // namespace vcl
