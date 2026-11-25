// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/check_references.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

namespace vcl::bind {

void initCheckReferencesAlgorithms(pybind11::module& m)
{
    using namespace pybind11::literals;

    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "check_mesh_references",
            [](const MeshType& m) { return vcl::checkMeshReferences(m); },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fAllMeshes);
}

} // namespace vcl::bind
