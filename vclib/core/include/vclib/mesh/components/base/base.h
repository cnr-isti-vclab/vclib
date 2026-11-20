// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BASE_BASE_H
#define VCL_MESH_COMPONENTS_BASE_BASE_H

#include <vclib/base.h>

namespace vcl {

/**
 * @brief The CompId struct enumerates the components that can compose a element
 * or a mesh.
 *
 * The components are identified by an unsigned integer value, that can be used
 * to access the component of the element or mesh through a template parameter,
 * without having to know the actual type of the component.
 *
 * For example, you can access the position component of an element with the
 * following code:
 * @code{.cpp}
 * MyElement e;
 * auto& p = e.template component<CompId::POSITION>();
 * @endcode
 *
 * @ingroup base
 */
enum class CompId : uint {
    BIT_FLAGS = 0,
    POSITION,
    NORMAL,
    COLOR,
    QUALITY,
    MATERIAL_INDEX,
    PRINCIPAL_CURVATURE,
    TANGENT,
    TEX_COORD,
    VERTEX_REFERENCES,
    ADJACENT_EDGES,
    ADJACENT_FACES,
    ADJACENT_VERTICES,
    WEDGE_COLORS,
    WEDGE_NORMALS,
    WEDGE_TEX_COORDS,
    BOUNDING_BOX,
    NAME,
    MATERIALS,
    TRANSFORM_MATRIX,
    CUSTOM_COMPONENTS,
    // Additional components here

    COMPONENT_COUNT,
};

/**
 * @brief The COMPONENT_ENUM_STRINGS array contains the string representation
 * of the CompId values.
 *
 * @ingroup base
 */
constexpr const char*
    COMPONENT_ENUM_STRINGS[static_cast<uint>(CompId::COMPONENT_COUNT)] = {
        "BitFlags",
        "Position",
        "Normal",
        "Color",
        "Quality",
        "MaterialIndex",
        "PrincipalCurvature",
        "Tangent",
        "TexCoord",
        "VertexReferences",
        "AdjacentEdges",
        "AdjacentFaces",
        "AdjacentVertices",
        "WedgeColors",
        "WedgeNormals",
        "WedgeTexCoords",
        "BoundingBox",
        "Name",
        "Materials",
        "TransformMatrix",
        "CustomComponents",
};

/**
 * @brief The ComponentString class is used to retrieve the string associated
 * to a COMP_ID value, trough its member 'str'.
 *
 * @tparam COMP_ID: The COMP_ID value associated to the string.
 *
 * @ingroup base
 */
template<CompId COMP_ID>
struct ComponentString
{
    /**
     * @brief The string associated to the COMPONENT_ID.
     */
    const char* str = COMP_ID < CompId::COMPONENT_COUNT ?
                          COMPONENT_ENUM_STRINGS[static_cast<uint>(COMP_ID)] :
                          nullptr;
};

/**
 * @brief Returns the string associated to the COMP_ID value.
 *
 * @tparam COMP_ID: an unsigned integer that identifies the component.
 * @return The string associated to the COMP_ID value.
 *
 * @ingroup base
 */
template<CompId COMP_ID>
constexpr const char* componentEnumString()
{
    static_assert(
        ComponentString<COMP_ID>().str != nullptr,
        "Invalid ComponentIDEnum. You should specialize the 'ComponentString' "
        "struct with your COMP_ID value.");

    return ComponentString<COMP_ID>().str;
}

} // namespace vcl

#endif // VCL_MESH_COMPONENTS_BASE_BASE_H
