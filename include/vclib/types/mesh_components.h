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

#ifndef VCL_TYPES_MESH_COMPONENTS_H
#define VCL_TYPES_MESH_COMPONENTS_H

#include "base.h"

namespace vcl {

enum ComponentIDEnum : uint {
	BIT_FLAGS = 0,
	COORDINATE,
	NORMAL,
	COLOR,
	QUALITY,
	MARK,
	PRINCIPAL_CURVATURE,
	TEX_COORD,
	VERTEX_PTRS,
	ADJACENT_EDGES,
	ADJACENT_FACES,
	ADJACENT_VERTICES,
	WEDGE_COLORS,
	WEDGE_TEX_COORDS,
	BOUNDING_BOX,
	NAME,
	TEXTURE_PATHS,
	TRANSFORM_MATRIX,
	CUSTOM_COMPONENTS
};

inline static constexpr uint COMPONENTS_NUMBER = 19;

inline static constexpr const char* COMPONENT_ENUM_STRINGS[COMPONENTS_NUMBER] =
	{
		"BitFlags",
		"Coordinate",
		"Normal",
		"Color",
		"Quality",
		"Mark",
		"PrincipalCurvature",
		"TexCoord",
		"VertexPointers",
		"AdjacentEdges",
		"AdjacentFaces",
		"AdjacentVertices",
		"WedgeColors",
		"WedgeTexCoords",
		"BoundingBox",
		"Name",
		"TexturePaths",
		"TransformMatrix",
		"CustomComponents",
};

/**
 * @brief The ComponentString class is used to retrieve the string associated
 * to a COMPONENT_ID value, trough its member 'str'.
 *
 * If you use a custom component class, you should specialize this struct with
 * your COMPONENT_ID value (that is >= COMPONENTS_NUMBER).
 *
 * @tparam COMPONENT_ID The COMPONENT_ID value associated to the string.
 */
template<uint COMPONENT_ID>
struct ComponentString {
	/**
	 * @brief The string associated to the COMPONENT_ID.
	 */
	const char* str = COMPONENT_ID < COMPONENTS_NUMBER ?
						  COMPONENT_ENUM_STRINGS[COMPONENT_ID] :
						  nullptr;
};

/**
 * @brief Returns the string associated to the COMPONENT_ID value.
 *
 * @tparam COMPONENT_ID: an unsigned integer that idetinfies the component.
 * @return The string associated to the COMPONENT_ID value.
 */
template<uint COMPONENT_ID>
constexpr const char* componentEnumString()
{
	static_assert(
		ComponentString<COMPONENT_ID>().str != nullptr,
		"Invalid ComponentEnumType. You should specialize the "
		"'ComponentString' struct with "
		"your COMPONENT_ID value.");

	return ComponentString<COMPONENT_ID>().str;
}

} // namespace vcl

#endif // VCL_TYPES_MESH_COMPONENTS_H
