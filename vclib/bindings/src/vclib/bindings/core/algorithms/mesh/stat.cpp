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

#include <vclib/bindings/core/algorithms/mesh/stat.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/stat.h>
#include <vclib/algorithms/mesh/type_name.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initStatAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            m.def("barycenter", [](const MeshType& m) {
                return vcl::barycenter(m);
            });

            m.def("quality_weighted_barycenter", [](const MeshType& m) {
                return vcl::qualityWeightedBarycenter(m);
            });

            m.def("bounding_box", [](const MeshType& m) {
                return vcl::boundingBox(m);
            });

            m.def("covariance_matrix_of_point_cloud", [](const MeshType& m) {
                return vcl::covarianceMatrixOfPointCloud(m);
            });
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def("shell_barycenter", [](const MeshType& m) {
            return vcl::shellBarycenter(m);
        });

        m.def("volume", [](const MeshType& m) {
            return vcl::volume(m);
        });

        m.def("surface_area", [](const MeshType& m) {
            return vcl::surfaceArea(m);
        });

        m.def("border_length", [](const MeshType& m) {
            return vcl::borderLength(m);
        });

        m.def("covariance_matrix_of_mesh", [](const MeshType& m) {
            return vcl::covarianceMatrixOfMesh(m);
        });

        m.def(
            "crease_face_edges",
            [](const MeshType& m,
               double          angleRadNeg,
               double          angleRadPos,
               bool            alsoBorderEdges) {
                return vcl::creaseFaceEdges(
                    m, angleRadNeg, angleRadPos, alsoBorderEdges);
            },
            py::arg("mesh"),
            py::arg("angle_rad_neg"),
            py::arg("angle_rad_pos"),
            py::arg("also_border_edges") = false);
    };

    defForAllMeshTypes(m, fFaceMeshes);
}

} // namespace vcl::bind
