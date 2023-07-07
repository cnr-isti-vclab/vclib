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

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The Color class represents a RGBA color that will be part of an
 * Element (e.g. Vertex, Face, ...) or Mesh.
 *
 * Allows to get and set an object of type Color.
 *
 * @note This component can be both used for Elements and Meshes.
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
class Color : public Component<COLOR, vcl::Color, ElementType, OPT>
{
	using Base = Component<COLOR, vcl::Color, ElementType, OPT>;

public:
	bool isEnabled() const;

	const vcl::Color& color() const;

	vcl::Color& color();

protected:
	// Component interface function
	template<typename Element>
	void importFrom(const Element& e);
};

/* Detector function to check if a class has Color enabled */

bool isColorEnabledOn(const ElementOrMeshConcept auto& element);

} // namespace vcl::comp

#include "color.cpp"

#endif // VCL_MESH_COMPONENTS_COLOR_H
