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

namespace detail {

template<MeshConcept MeshType1, MeshConcept MeshType2>
void defHausdorffDistance(pybind11::module& m)
{
    using namespace pybind11::literals;

    m.def(
        "hausdorff_distance",
        [](const MeshType1&        m1,
           const MeshType2&        m2,
           HausdorffSamplingMethod sampMethod,
           uint                    nSamples,
           std::optional<uint>     seed,
           AbstractLogger&         log) -> HausdorffDistResult {
            return hausdorffDistance(
                m1, m2, log, sampMethod, nSamples, toRConfig(seed));
        },
        "mesh1"_a,
        "mesh2"_a,
        "samp_method"_a = HAUSDORFF_VERTEX_UNIFORM,
        "n_samples"_a   = 0,
        "seed"_a        = pybind11::none(),
        "log"_a         = pybind11::cast(&vcl::nullLogger));
}

template<MeshConcept MeshType1>
void defAllHausdorffDistances(pybind11::module& m)
{
    auto fMesh2 = []<MeshConcept MeshType2>(
                      pybind11::module& m, MeshType2 = MeshType2()) {
        defHausdorffDistance<MeshType1, MeshType2>(m);
    };

    defForAllMeshTypes(m, fMesh2);
}

} // namespace detail

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
        detail::defAllHausdorffDistances<MeshType>(m);
    };

    defForAllMeshTypes(m, fAllMeshes);
}

} // namespace vcl::bind
