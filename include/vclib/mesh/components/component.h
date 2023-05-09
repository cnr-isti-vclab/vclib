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

#ifndef VCL_MESH_COMPONENTS_COMPONENT_H
#define VCL_MESH_COMPONENTS_COMPONENT_H

#include <vclib/concepts/mesh/components/component.h>
#include "internal/component_data.h"

namespace vcl::comp {

/**
 * @brief The Component class is the base class for almost all the components of VCLib (with the
 * exception of CustomComponents).
 *
 * Inheriting from this class, a component will have all the necessary definitions to be used
 * smoothly by the vcl::Mesh class, managing all the possible features of a component automatically.
 *
 * All the features of a Component can be defined trough its template parameters.
 */
template<
	typename DataType,
	typename ElementType,
	bool optional,
	bool TTVN = false,
	int N = 0,
	typename... PointedTypes>
class Component : public PointersComponentTriggerer<PointedTypes>...
{
public:
	using DataValueType = DataType;

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

	/**
	 * @brief Boolean that tells if this component stores a container having its size tied to the
	 * number of the vertices of the Element.
	 *
	 * E.g. suppose to have a Polygonal Face f, having 5 vertices (and 5 edges).
	 * This means that it the Face has the AdjacentFaces component, then it should store 5 adjacent
	 * faces (the same number of the vertices). In this case, the AdjacentFaces component will have
	 * the boolean TIED_TO_VERTEX_NUMBER set to true.
	 */
	static const bool TIED_TO_VERTEX_NUMBER = TTVN;

	static const int SIZE = N;

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

#endif // VCL_MESH_COMPONENTS_COMPONENT_H
