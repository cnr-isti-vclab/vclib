// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_CONTAINERS_EDGE_CONTAINER_H
#define VCL_BINDINGS_CORE_MESH_CONTAINERS_EDGE_CONTAINER_H

#include "container.h"

#include <vclib/space/core.h>

#include <pybind11/stl.h>

namespace vcl::bind {

template<MeshConcept MeshType, typename... Options>
void initEdgeContainer(pybind11::class_<MeshType, Options...>& ct)
{
    namespace py = pybind11;

    using EdgeType = MeshType::EdgeType;

    initContainer<EdgeType>(ct, "edge");
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_CONTAINERS_EDGE_CONTAINER_H
