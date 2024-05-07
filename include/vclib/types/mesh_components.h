/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_TYPES_MESH_COMPONENTS_H
#define VCL_TYPES_MESH_COMPONENTS_H

#include "base.h"

namespace vcl {

struct CompId
{
    enum Enum {
        BIT_FLAGS = 0,
        COORDINATE,
        NORMAL,
        COLOR,
        QUALITY,
        MARK,
        PRINCIPAL_CURVATURE,
        TEX_COORD,
        VERTEX_REFERENCES,
        ADJACENT_EDGES,
        ADJACENT_FACES,
        ADJACENT_VERTICES,
        WEDGE_COLORS,
        WEDGE_TEX_COORDS,
        BOUNDING_BOX,
        NAME,
        TEXTURE_PATHS,
        TRANSFORM_MATRIX,
        CUSTOM_COMPONENTS,
        // Additonal components here

        COMPONENTS_NUMBER,
    };
};

constexpr const char* COMPONENT_ENUM_STRINGS[CompId::COMPONENTS_NUMBER] = {
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
 * to a COMP_ID value, trough its member 'str'.
 *
 * If you use a custom component class, you should specialize this struct with
 * your COMP_ID value (that is >= COMPONENTS_NUMBER).
 *
 * @tparam COMP_ID: The COMP_ID value associated to the string.
 */
template<uint COMP_ID>
struct ComponentString
{
    /**
     * @brief The string associated to the COMPONENT_ID.
     */
    const char* str = COMP_ID < CompId::COMPONENTS_NUMBER ?
                          COMPONENT_ENUM_STRINGS[COMP_ID] :
                          nullptr;
};

/**
 * @brief Returns the string associated to the COMP_ID value.
 *
 * @tparam COMP_ID: an unsigned integer that idetinfies the component.
 * @return The string associated to the COMP_ID value.
 */
template<uint COMP_ID>
constexpr const char* componentEnumString()
{
    static_assert(
        ComponentString<COMP_ID>().str != nullptr,
        "Invalid ComponentIDEnum. You should specialize the 'ComponentString' "
        "struct with your COMP_ID value.");

    return ComponentString<COMP_ID>().str;
}

} // namespace vcl

#endif // VCL_TYPES_MESH_COMPONENTS_H
