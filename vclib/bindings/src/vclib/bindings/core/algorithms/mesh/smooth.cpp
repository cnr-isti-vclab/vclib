// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/smooth.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

namespace vcl::bind {

void initSmoothAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            m.def(
                "smooth_per_vertex_normals_point_cloud",
                [](MeshType& m, uint neighborCount, uint iterCount) -> void {
                    return smoothPerVertexNormalsPointCloud(
                        m, neighborCount, iterCount);
                },
                "mesh"_a,
                "neighbor_count"_a,
                "iter_count"_a);
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "laplacian_smoothing",
            [](MeshType& m,
               uint      step,
               bool      smoothSelected,
               bool      cotangentWeight) -> void {
                return laplacianSmoothing(
                    m, step, smoothSelected, cotangentWeight);
            },
            "mesh"_a,
            "step"_a,
            "smooth_selected"_a  = false,
            "cotangent_weight"_a = false);

        m.def(
            "taubin_smoothing",
            [](MeshType& m,
               uint      step,
               float     lambda,
               float     mu,
               bool      smoothSelected) -> void {
                return taubinSmoothing(m, step, lambda, mu, smoothSelected);
            },
            "mesh"_a,
            "step"_a,
            "lambda"_a,
            "mu"_a,
            "smooth_selected"_a = false);
    };

    defForAllMeshTypes(m, fFaceMeshes);
}

} // namespace vcl::bind
