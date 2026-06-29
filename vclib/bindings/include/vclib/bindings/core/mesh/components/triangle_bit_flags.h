// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H

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
    c.def("set_edge_on_border", [](ElementType& e, uint i, bool b) {
        e.edgeOnBorder(i) = b;
    });
    c.def(
        "edge_selected",
        py::overload_cast<uint>(&ElementType::edgeSelected, py::const_));
    c.def("set_edge_selected", [](ElementType& e, uint i, bool b) {
        e.edgeSelected(i) = b;
    });
    c.def(
        "edge_visited",
        py::overload_cast<uint>(&ElementType::edgeVisited, py::const_));
    c.def("set_edge_visited", [](ElementType& e, uint i, bool b) {
        e.edgeVisited(i) = b;
    });
    c.def(
        "edge_faux",
        py::overload_cast<uint>(&ElementType::edgeFaux, py::const_));
    c.def("set_edge_faux", [](ElementType& e, uint i, bool b) {
        e.edgeFaux(i) = b;
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
