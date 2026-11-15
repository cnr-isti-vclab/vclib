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

template<ElementConcept ElementType, typename... Options>
void initTriangleBitFlags(pybind11::class_<ElementType, Options...>& c)
{
    namespace py = pybind11;

    detail::initCommonFlags(c);

    c.def("edge_on_border", [](const ElementType& e, uint i) {
        detail::checkBitFlags(e);
        if (i >= 3) {
            throw py::index_error();
        }
        return e.edgeOnBorder(i);
    });
    c.def("set_edge_on_border", [](ElementType& e, uint i, bool b) {
        detail::checkBitFlags(e);
        if (i >= 3) {
            throw py::index_error();
        }
        e.edgeOnBorder(i) = b;
    });
    c.def("edge_selected", [](const ElementType& e, uint i) {
        detail::checkBitFlags(e);
        if (i >= 3) {
            throw py::index_error();
        }
        return e.edgeSelected(i);
    });
    c.def("set_edge_selected", [](ElementType& e, uint i, bool b) {
        detail::checkBitFlags(e);
        if (i >= 3) {
            throw py::index_error();
        }
        e.edgeSelected(i) = b;
    });
    c.def("edge_visited", [](const ElementType& e, uint i) {
        detail::checkBitFlags(e);
        if (i >= 3) {
            throw py::index_error();
        }
        return e.edgeVisited(i);
    });
    c.def("set_edge_visited", [](ElementType& e, uint i, bool b) {
        detail::checkBitFlags(e);
        if (i >= 3) {
            throw py::index_error();
        }
        e.edgeVisited(i) = b;
    });
    c.def("edge_faux", [](const ElementType& e, uint i) {
        detail::checkBitFlags(e);
        if (i >= 3) {
            throw py::index_error();
        }
        return e.edgeFaux(i);
    });
    c.def("set_edge_faux", [](ElementType& e, uint i, bool b) {
        detail::checkBitFlags(e);
        if (i >= 3) {
            throw py::index_error();
        }
        e.edgeFaux(i) = b;
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_TRIANGLE_BIT_FLAGS_H
