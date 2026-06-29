// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/distance.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initDistanceAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    // bind the HausdorffDistResult struct
    py::class_<HausdorffDistResult>(m, "HausdorffDistResult")
        .def_readonly("minDist", &HausdorffDistResult::minDist)
        .def_readonly("maxDist", &HausdorffDistResult::maxDist)
        .def_readonly("meanDist", &HausdorffDistResult::meanDist)
        .def_readonly("RMSDist", &HausdorffDistResult::RMSDist)
        .def_readonly("histogram", &HausdorffDistResult::histogram);

    // bind enum HausdorffSamplingMethod
    py::enum_<HausdorffSamplingMethod>(m, "HausdorffSamplingMethod")
        .value(
            "HAUSDORFF_VERTEX_UNIFORM",
            HausdorffSamplingMethod::HAUSDORFF_VERTEX_UNIFORM)
        .value(
            "HAUSDORFF_EDGE_UNIFORM",
            HausdorffSamplingMethod::HAUSDORFF_EDGE_UNIFORM)
        .value(
            "HAUSDORFF_MONTECARLO",
            HausdorffSamplingMethod::HAUSDORFF_MONTECARLO)
        .export_values();

    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        // TODO: bind between different types of meshes

        m.def(
            "hausdorff_distance",
            [](const MeshType&         m1,
               const MeshType&         m2,
               HausdorffSamplingMethod sampMethod,
               uint                    nSamples,
               std::optional<uint>     seed) -> HausdorffDistResult {
                return hausdorffDistance(
                    m1, m2, nullLogger, sampMethod, nSamples, toRConfig(seed));
            },
            "mesh1"_a,
            "mesh2"_a,
            "samp_method"_a = HAUSDORFF_VERTEX_UNIFORM,
            "n_samples"_a   = 0,
            "seed"_a        = py::none());
    };

    defForAllMeshTypes(m, fAllMeshes);
}

} // namespace vcl::bind
