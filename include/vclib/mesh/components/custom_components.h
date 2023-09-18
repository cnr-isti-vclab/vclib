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

#ifndef VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
#define VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H

#include <any>
#include <string>
#include <vector>

#include <vclib/concepts/mesh/components/custom_components.h>

#include "internal/custom_components_data.h"

namespace vcl::comp {

/**
 * @brief The CustomComponents Component is a container of additional components
 * associated to an Element (e.g. Vertex, Face).
 *
 * CustomComponents are components that can be added at runtime. Each custom
 * component is composed of:
 * - a name, which is an std::string
 * - a type, which needs to be known at compile time
 *
 * E.g., to access to a CustomComponent of type 'int' called "myCustomComponent"
 * from an element el:
 *
 * @code{.cpp}
 * el.customComponent<int>("myCustomComponent");
 * @endcode
 *
 * CustomComponents can be stored horizontally or vertically.
 *
 * If the CustomComponents component is horizontal (this happens mostly on Mesh
 * data structures), a custom component can be added or removed from the object
 * itself.
 *
 * For example, having a Mesh m of type 'MyMesh' that has horizontal
 * CustomComponents:
 *
 * @code{.cpp}
 * m.addCustomComponent<int>("myCustomComponent");
 * @endcode
 *
 * Otherwise, if the CustomComponents component is vertical (this happens mostly
 * on Element types stored in a container, like Vertex, Face...), the
 * addition/deletion of a custom component cannot be performed by the object,
 * because its storage its managed by the Container of objects (that must
 * provide a proper member function to do that).
 *
 * For example, having a Mesh m of type MyMesh that has a VertexContainer where
 * its Vertex has (vertical) CustomComponents:
 *
 * @code{.cpp}
 * m.addPerVertexCustomComponent<int>("myCustomComponent");
 * @endcode
 *
 * After this call, all the Vertices of the VertexContainer will have their
 * custom component of type int called "myCustomComponent". The member function
 * addPerVertexCustomComponent is provided by the VertexContainer and can be
 * accessed directly from the Mesh.
 *
 * @tparam ElementType: This type is used to discriminate between horizontal and
 * vertical components. When a component is horizontal, this type must be void.
 * When a component is vertical, this type must be the type of the Element that
 * has the component, and it will be used by the vcl::Mesh to access to the data
 * stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void>
class CustomComponents
{
	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
public:
	/**
	 * @brief The ID of component.
	 */
	static const uint COMPONENT_ID = CUSTOM_COMPONENTS;

	bool hasCustomComponent(const std::string& compName) const;

	template<typename CompType>
	bool isCustomComponentOfType(const std::string& compName) const;
	
	std::type_index customComponentType(const std::string& compName) const;

	template<typename CompType>
	std::vector<std::string> customComponentNamesOfType() const;

	template<typename CompType>
	const CompType& customComponent(const std::string& compName) const;

	template<typename CompType>
	CompType& customComponent(const std::string& compName);

	template<typename CompType>
	void addCustomComponent(
		const std::string& compName,
		const CompType&    value = CompType()) requires (!IS_VERTICAL);

	void deleteCustomComponent(const std::string& compName)
		requires (!IS_VERTICAL);

protected:
	template <typename Element>
	void importFrom(const Element& e);

private:
	internal::CustomComponentsData<ElementType, IS_VERTICAL> data;
};

} // namespace vcl::comp

#include "custom_components.cpp"

#endif // VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
