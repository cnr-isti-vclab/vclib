// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H

#include "triangle_bit_flags.h"

namespace vcl::bind {

template<ElementConcept ElementType, typename... Options>
void initPolygonBitFlags(pybind11::class_<ElementType, Options...>& c)
{
    namespace py = pybind11;

    initTriangleBitFlags(c);

    c.def("edge_count", [](const ElementType& e) {
        detail::checkBitFlags(e);
        return e.edgeCount();
    });
    c.def("edge_user_bit", [](const ElementType& e, uint i, uint bit) {
        detail::checkBitFlags(e);
        if (i >= e.edgeCount()) {
            throw py::index_error();
        }
        return e.edgeUserBit(i, bit);
    });
    c.def("set_edge_user_bit", [](ElementType& e, uint i, uint bit, bool b) {
        detail::checkBitFlags(e);
        if (i >= e.edgeCount()) {
            throw py::index_error();
        }
        e.edgeUserBit(i, bit) = b;
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
