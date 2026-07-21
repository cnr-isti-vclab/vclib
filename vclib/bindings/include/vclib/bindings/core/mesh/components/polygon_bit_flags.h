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

    // c.def(
    //     "edge_user_bit",
    //     py::overload_cast<uint>(&ElementType::edgeUserBit, py::const_));
    // c.def("set_edge_user_bit", [](ElementType& e, uint i, bool b) {
    //     e.edgeUserBit(i) = b;
    // });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_POLYGON_BIT_FLAGS_H
