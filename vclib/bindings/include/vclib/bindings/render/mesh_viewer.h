// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_RENDER_MESH_VIEWER_H
#define VCL_BINDINGS_RENDER_MESH_VIEWER_H

#include <pybind11/pybind11.h>

namespace vcl::bind {

/**
 * @brief Initializes the MeshViewer Python bindings.
 * @param[in,out] m: The pybind11 module where the bindings will be registered.
 */
void initMeshViewer(pybind11::module& m);

} // namespace vcl::bind

#endif // VCL_BINDINGS_RENDER_MESH_VIEWER_H
