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

#ifndef VCL_TYPES_MESH_ELEMENTS_H
#define VCL_TYPES_MESH_ELEMENTS_H

#include <string>

#include "base.h"

namespace vcl {

/**
 * @brief The ElemId struct enumerates the elements that can compose a mesh.
 *
 * The elements are identified by an unsigned integer value, that can be used to
 * access the element of the mesh through a template parameter, without having
 * to know the actual type of the element.
 *
 * This is very useful when you want to write generic code that can work with
 * different types of elements.
 *
 * For example, you can iterate the vertices of a mesh with the following
 * code:
 * @code {.cpp}
 * MyMesh m;
 * for (auto& v : m.template elements<ElemId::VERTEX>()) {
 *    // do something with the vertex v
 * }
 * @endcode
 */
struct ElemId
{
    enum Enum : uint {
        VERTEX = 0,
        FACE,
        EDGE,
        ELEMENTS_NUMBER,
    };
};

/**
 * @brief The ELEMENT_ENUM_STRINGS array contains the string representation of
 * the elements that can compose a mesh.
 */
constexpr const char* ELEMENT_ENUM_STRINGS[ElemId::ELEMENTS_NUMBER] = {
    "Vertex",
    "Face",
    "Edge",
};

/**
 * @brief The ElementString class is used to retrieve the string associated
 * to a ELEM_ID value, trough its member 'str'.
 *
 * If you use a custom element class, you should specialize this struct with
 * your ELEM_ID value (that is >= ELEMENTS_NUMBER).
 *
 * @tparam ELEM_ID: The ELEM_ID value associated to the string.
 */
template<uint ELEM_ID>
struct ElementString
{
    /**
     * @brief The string associated to the ELEM_ID.
     */
    const char* str = ELEM_ID < ElemId::ELEMENTS_NUMBER ?
                          ELEMENT_ENUM_STRINGS[ELEM_ID] :
                          nullptr;
};

/**
 * @brief Returns the string associated to the ELEM_ID value.
 *
 * @tparam ELEM_ID: an unsigned integer that identifies the element.
 * @return The string associated to the ELEM_ID value.
 */
template<uint ELEM_ID>
constexpr const char* elementEnumCString()
{
    static_assert(
        ElementString<ELEM_ID>().str != nullptr,
        "Invalid ElementIDEnum. You should specialize 'the ElementString' "
        "struct with your ELEM_ID value.");

    return ElementString<ELEM_ID>().str;
}

/**
 * @brief Returns the string associated to the ELEM_ID value.
 *
 * @tparam ELEM_ID: an unsigned integer that identifies the element.
 * @return The string associated to the ELEM_ID value.
 */
// Todo: make this function constexpr when upgrading to C++23.
template<uint ELEM_ID>
const std::string& elementEnumString()
{
    static const std::string str = elementEnumCString<ELEM_ID>();

    return str;
}

} // namespace vcl

#endif // VCL_TYPES_MESH_ELEMENTS_H
