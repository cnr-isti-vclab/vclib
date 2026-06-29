// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESHES_H
#define VCL_BINDINGS_CORE_MESHES_H

#include "meshes/edge_mesh.h"
#include "meshes/point_cloud.h"
#include "meshes/poly_edge_mesh.h"
#include "meshes/poly_mesh.h"
#include "meshes/tri_edge_mesh.h"
#include "meshes/tri_mesh.h"

#include <pybind11/pybind11.h>

namespace vcl::bind {

inline void initMeshes(pybind11::module& m)
{
    namespace py = pybind11;

    // py::module_ sm = m.def_submodule("meshes", "Meshes");
    initEdgeMesh(m);
    initPointCloud(m);
    initPolyMesh(m);
    initPolyEdgeMesh(m);
    initTriMesh(m);
    initTriEdgeMesh(m);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESHES_H
