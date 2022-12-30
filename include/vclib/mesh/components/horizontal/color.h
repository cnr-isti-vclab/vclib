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

#ifndef VCL_MESH_COMPONENTS_COLOR_H
#define VCL_MESH_COMPONENTS_COLOR_H

#include <vclib/space/color.h>

#include "../concepts/color.h"

namespace vcl::comp {

/**
 * @brief The Color class represents a RGBA color that will be part of an Element
 * (e.g. Vertex, Face, ...).
 *
 * Allows to get and set an object of type Color.
 */
class Color
{
public:
	using ColorType = vcl::Color;

	const vcl::Color& color() const;

	vcl::Color& color();

	constexpr bool isColorEnabled() const { return true; }

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	vcl::Color c;
};

} // namespace vcl::comp

#include "color.cpp"

#endif // VCL_MESH_COMPONENTS_COLOR_H
