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

#ifndef VCL_MESH_COMPONENTS_COLOR_H
#define VCL_MESH_COMPONENTS_COLOR_H

#include <vclib/concepts/mesh/components/color.h>
#include <vclib/space/color.h>

#include "internal/component_data.h"

namespace vcl::comp {

/**
 * @brief The Color class represents a RGBA color that will be part of an Element
 * (e.g. Vertex, Face, ...).
 *
 * Allows to get and set an object of type Color.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool optional = false>
class Color
{
	using ThisType = Color<ElementType, optional>;
public:
	using DataValueType = vcl::Color; // data that the component stores internally (or vertically)
	using ColorComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	using ColorType = vcl::Color;

	bool isEnabled() const;
	bool isColorEnabled() const;

	const vcl::Color& color() const;

	vcl::Color& color();

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	vcl::Color& c();
	const vcl::Color& c() const;

	// contians the actual data of the component, if the component is horizontal
	internal::ComponentData<DataValueType, IS_VERTICAL> data;
};

/* Detector function to check if a class has Color enabled */

template <typename T>
bool isColorEnabledOn(const T& element);

} // namespace vcl::comp

#include "color.cpp"

#endif // VCL_MESH_COMPONENTS_COLOR_H
