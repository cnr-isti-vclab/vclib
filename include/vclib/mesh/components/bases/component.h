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

#ifndef VCL_MESH_COMPONENTS_BASES_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_COMPONENT_H

#include <vclib/concepts/mesh/components/component.h>
#include "../internal/component_data.h"

namespace vcl::comp {

/**
 * @brief The Component class is the base class for almost all the components of VCLib (with the
 * exception of CustomComponents).
 *
 * Inheriting from this class, a component will have some necessary definitions to be used
 * smoothly by the vcl::Mesh class, managing all the possible features of a component automatically.
 *
 * The features that a Component could have are:
 *
 * - possibility to be horizontal, vertical or optional:
 *   - a component is horizontal when its data is stored in the memory frame of the Element that
 *     has the component;
 *   - a component is vertical when its data is not stored in the memory frame of the Element, but
 *     in a separated Conainer; in this case, the data will be in a contiguous array;
 *   - a component is optional if it is vertical and can be enabled/disabled at runtime;
 * - possibility to store pointers to other Elements that must be updated when a reallocation
 *   happens
 *   - An example is the VertexPointers component: it stores the pointers to the Vertices of an
 *     Element (e.g. a Face). When a a reallocation of the VertexContainer happens, all the pointers
 *     to the vertices must be updated, and this operation will be made automatically if the Vertex
 *     type will be part of the PointedTypes.
 *
 * There are also some additional features that are given by the @ref ContainerComponent and
 * @ref PointersContainerComponent classes. If you need to implement a Component that stores a
 * Container of data or a Container of Pointers, take a look to that classes.
 *
 * All the features of a Component can be defined trough its template parameters, and implementing
 * some protected member functions. To properly implement a Component class, must define the
 * following protected member function:
 *
 * ```cpp
 * template <typename Element>
 * void importFrom(const Element& e);
 * ```
 *
 * Moreover, if the component has at least one PointedType, it must define the following protected
 * member functions:
 *
 * ```cpp
 * template<typename Element, typename ElEType>
 * void importPointersFrom(const Element& e, PointedType* base, const ElEType* ebase);
 *
 * void updatePointers(const PointedType* oldBase, const PointedType* newBase);
 *
 * void updatePointersAfterCompact(const PointedType* base, const std::vector<int>& newIndices);
 * ```
 *
 * If your component stores a Container of pointers, look for the @ref PointersContainerComponent
 * class, that provides the implementation of these functions!
 *
 * For further details , please refer to the page @ref implement_component page.
 *
 * @tparam DataType: The type of the data that the component needs to store. E.g. a Normal component
 * would store a vcl::Point3d.
 * @tparam ElementType: This type is used to discriminate between horizontal and vertical
 * components. When a component is horizontal, this type must be void. When a component is vertical,
 * this type must be the type of the Element that has the component, and it will be used by the
 * vcl::Mesh to access to the data stored vertically.
 * @tparam OPT: When a component is vertical, it could be optional, that means that could be
 * enabled/disabled at runtime. To make the component optional, this template parameter must be
 * true.
 * @tparam PointedTypes: Variadic Template types of all the Pointer types that the component stores,
 * and that need to be updated when some reallocation happens.
 */
template<
	uint COMP_TYPE,
	typename DataType,
	typename ElementType,
	bool OPT,
	typename... PointedTypes>
class Component : public PointersComponentTriggerer<PointedTypes>...
{
public:
	/**
	 * @brief The Data that the component will store (e.g. a vcl::Point3d for a Normal component).
	 */
	using DataValueType = DataType;

	static const uint COMPONENT_TYPE = COMP_TYPE;

	/**
	 * @brief Boolean that tells if this component type stores its data vertically (not in the
	 * Element frame memory, but in another vector).
	 */
	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;

	/**
	 * @brief Boolean that tells if this component is optional. Makes sense only when the component
	 * is vertical.
	 */
	static const bool IS_OPTIONAL = OPT;

protected:
	template<typename Comp>
	bool isEnabled(const Comp* c) const
	{
		return cdata.template isComponentEnabled<ElementType>(c);
	}

	template<typename Comp>
	DataValueType& data(Comp* c)
	{
		return cdata.template get<ElementType>(c);
	}

	template<typename Comp>
	const DataValueType& data(const Comp* c) const
	{
		return cdata.template get<ElementType>(c);
	}

private:
	internal::ComponentData<DataValueType, IS_VERTICAL> cdata;
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASES_COMPONENT_H
