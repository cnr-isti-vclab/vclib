/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include "filter_types.h"

namespace vcl {

/**
 * @brief The CompId struct enumerates the components that can compose a element
 * or a mesh.
 *
 * The components are identified by an unsigned integer value, that can be used
 * to access the component of the element or mesh through a template parameter,
 * without having to know the actual type of the component.
 *
 * For example, you can access the coordinate component of an element with the
 * following code:
 * @code{.cpp}
 * MyElement e;
 * auto& c = e.template component<CompId::COORDINATE>();
 * @endcode
 *
 * @ingroup types
 */
struct CompId
{
    enum Enum {
        BIT_FLAGS = 0,
        POSITION,
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
        TEXTURE_IMAGES,
        TEXTURE_PATHS,
        TRANSFORM_MATRIX,
        CUSTOM_COMPONENTS,
        // Additional components here

        COMPONENTS_NUMBER,
    };
};

/**
 * @brief The COMPONENT_ENUM_STRINGS array contains the string representation
 * of the CompId::Enum values.
 *
 * @ingroup types
 */
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
    "TextureImages",
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
 *
 * @ingroup types
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
 * @tparam COMP_ID: an unsigned integer that identifies the component.
 * @return The string associated to the COMP_ID value.
 *
 * @ingroup types
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

namespace comp {

namespace detail {

/**
 * @brief Given the ID of a Component and a list of Component types,
 * this predicate sets its bool `value` to `true` if there exists a Component in
 * the list having the COMP_ID, and sets `type` to the TypeWrapper of the
 * found component.
 *
 * If no Component was found, value will be set to `false` and type will contain
 * an empty TypeWrapper.
 */
template<uint COMP_ID, typename... Components>
struct ComponentOfTypePred
{
private:
    template<typename Comp>
    struct SameCompPred
    {
        static constexpr bool value = Comp::COMPONENT_ID == COMP_ID;
    };

public:
    // TypeWrapper of the found container, if any
    using type =
        typename FilterTypesByCondition<SameCompPred, Components...>::type;
    static constexpr bool value = NumberOfTypes<type>::value == 1;
};

// TypeWrapper specialization
template<uint COMP_ID, typename... Components>
struct ComponentOfTypePred<COMP_ID, TypeWrapper<Components...>> :
        public ComponentOfTypePred<COMP_ID, Components...>
{
};

} // namespace detail

/**
 * @brief Given the ID of a Component and a list of Component types (or a
 * TypeWrapper), this alias sets its type to the corresponding Component type
 * found in the list.
 *
 * @ingroup types
 */
template<uint COMP_ID, typename... Components>
using ComponentOfType = FirstTypeT<
    typename detail::ComponentOfTypePred<COMP_ID, Components...>::type>;

template<typename T>
class ReferencesComponentTriggerer
{
};

} // namespace comp

} // namespace vcl

#endif // VCL_TYPES_MESH_COMPONENTS_H
