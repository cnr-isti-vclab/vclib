// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_SPACE_COMPLEX_H
#define VCL_BINDINGS_CORE_SPACE_COMPLEX_H

#include "complex/mesh_info.h"
#include "complex/point_sampler.h"
#include "complex/tri_poly_index_bimap.h"

#include <pybind11/pybind11.h>

namespace vcl::bind {

inline void initComplex(pybind11::module& m)
{
    namespace py = pybind11;

    // py::module_ sm = m.def_submodule("complex", "Complex Spatial Data
    // Structures");

    initMeshInfo(m);
    initPointSampler(m);
    initTriPolyIndexBimap(m);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_SPACE_COMPLEX_H
