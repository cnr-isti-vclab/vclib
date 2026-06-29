// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/convex_hull.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initConvexHullAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        std::string meshName = camelCaseToSnakeCase(meshTypeName<MeshType>());

        m.def(
            ("convex_hull_" + meshName).c_str(),
            [](const std::vector<Point3d>& points, std::optional<uint> seed) {
                return vcl::convexHull<MeshType>(points, toRConfig(seed));
            },
            "points"_a,
            "seed"_a = py::none());
    };

    defForAllMeshTypes(m, fFaceMeshes);
}

} // namespace vcl::bind
