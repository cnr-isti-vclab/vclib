// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/face_topology.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace vcl::bind {

void initFaceTopologyAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllFaces =
        []<FaceConcept FaceType>(pybind11::module& m, FaceType = FaceType()) {
            m.def(
                "is_face_manifold_on_edge",
                [](const FaceType& f, uint edge) -> bool {
                    return isFaceManifoldOnEdge(f, edge);
                },
                "face"_a,
                "edge"_a);

            m.def(
                "is_face_edge_on_border",
                [](const FaceType& f, uint edge) -> bool {
                    return isFaceEdgeOnBorder(f, edge);
                },
                "face"_a,
                "edge"_a);

            // TODO: add other functions from face_topology.h

            m.def(
                "flood_face_patch",
                [](const FaceType&                       seed,
                   std::function<bool(const FaceType&)>& selector) {
                    return floodFacePatch(seed, selector);
                },
                "seed"_a,
                "face_selector"_a,
                py::return_value_policy::reference);
        };

    defForAllFaceTypes(m, fAllFaces);
}

} // namespace vcl::bind
