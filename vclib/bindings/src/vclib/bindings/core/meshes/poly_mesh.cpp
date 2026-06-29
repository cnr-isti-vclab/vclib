// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/meshes/poly_mesh.h>

#include <vclib/bindings/core/mesh/mesh.h>
#include <vclib/bindings/utils.h>

#include <vclib/meshes.h>

namespace vcl::bind {

void initPolyMesh(pybind11::module& m)
{
    namespace py = pybind11;

    initMesh<vcl::PolyMesh>(m, "PolyMesh");
}

} // namespace vcl::bind
