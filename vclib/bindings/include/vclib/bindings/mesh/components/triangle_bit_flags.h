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

#ifndef VCL_BINDINGS_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
#define VCL_BINDINGS_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H

#include "bit_flags.h"

namespace vcl::bind {

template<ElementConcept ElementType>
void initTriangleBitFlags(pybind11::class_<ElementType>& c)
{
    namespace py = pybind11;

    detail::initCommonFlags(c);

    c.def(
        "edge_on_border",
        py::overload_cast<uint>(&ElementType::edgeOnBorder, py::const_));
    c.def(
        "set_edge_on_border",
        [](ElementType& e, uint i, bool b) {
            e.edgeOnBorder(i) = b;
        });
    c.def(
        "edge_selected",
        py::overload_cast<uint>(&ElementType::edgeSelected, py::const_));
    c.def(
        "set_edge_selected",
        [](ElementType& e, uint i, bool b) {
            e.edgeSelected(i) = b;
        });
    c.def(
        "edge_visited",
        py::overload_cast<uint>(&ElementType::edgeVisited, py::const_));
    c.def(
        "set_edge_visited",
        [](ElementType& e, uint i, bool b) {
            e.edgeVisited(i) = b;
        });
    c.def(
        "edge_faux",
        py::overload_cast<uint>(&ElementType::edgeFaux, py::const_));
    c.def(
        "set_edge_faux",
        [](ElementType& e, uint i, bool b) {
            e.edgeFaux(i) = b;
        });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
