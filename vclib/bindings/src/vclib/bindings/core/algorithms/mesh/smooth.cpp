/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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
                [](MeshType& m, uint neighborNum, uint iterNum) -> void {
                    return smoothPerVertexNormalsPointCloud(
                        m, neighborNum, iterNum);
                },
                "mesh"_a,
                "neighbor_number"_a,
                "iter_number"_a);
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
