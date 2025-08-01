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

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_BIT_FLAGS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_BIT_FLAGS_H

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

namespace detail {

template<ElementConcept ElementType>
void initCommonFlags(pybind11::class_<ElementType>& c)
{
    namespace py = pybind11;

    c.def("deleted", &ElementType::deleted);
    c.def("selected", py::overload_cast<>(&ElementType::selected, py::const_));
    c.def("set_selected", [](ElementType& e, bool s) {
        e.selected() = s;
    });
    c.def("on_border", py::overload_cast<>(&ElementType::onBorder, py::const_));
    c.def("visited", py::overload_cast<>(&ElementType::visited, py::const_));
    c.def("set_visited", [](ElementType& e, bool v) {
        e.visited() = v;
    });
    c.def("user_bit", [](ElementType& e, uint i) {
        return e.userBit(i);
    });
    c.def("set_user_bit", [](ElementType& e, uint i, bool b) {
        e.userBit(i) = b;
    });
    c.def("reset_bit_flags", &ElementType::resetBitFlags);
}

} // namespace detail

template<ElementConcept ElementType>
void initBitFlags(pybind11::class_<ElementType>& c)
{
    namespace py = pybind11;

    c.def("set_on_border", [](ElementType& e, bool b) {
        e.onBorder() = b;
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_BIT_FLAGS_H
