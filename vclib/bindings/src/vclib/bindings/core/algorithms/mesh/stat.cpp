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
            // barycenter.h

            m.def("barycenter", [](const MeshType& m) {
                return vcl::barycenter(m);
            });

            m.def("quality_weighted_barycenter", [](const MeshType& m) {
                return vcl::qualityWeightedBarycenter(m);
            });

            // bounding_box.h

            m.def("bounding_box", [](const MeshType& m) {
                return vcl::boundingBox(m);
            });

            // geometry.h

            m.def("covariance_matrix_of_point_cloud", [](const MeshType& m) {
                return vcl::covarianceMatrixOfPointCloud(m);
            });

            m.def(
                "vertex_radius_from_wheights",
                [](const MeshType&            m,
                   const std::vector<double>& weights,
                   double                     diskRadius,
                   double                     radiusVariance,
                   bool                       invert) {
                    return vcl::vertexRadiusFromWeights<double>(
                        m, weights, diskRadius, radiusVariance, invert);
                },
                py::arg("mesh"),
                py::arg("weights"),
                py::arg("disk_radius"),
                py::arg("radius_variance"),
                py::arg("invert") = false);

            // quality.h

            m.def("vertex_quality_min_max", [](const MeshType& m) {
                return vcl::vertexQualityMinMax(m);
            });

            m.def("vertex_quality_average", [](const MeshType& m) {
                return vcl::vertexQualityAverage(m);
            });

            // selection.h

            m.def("vertex_selection_number", [](const MeshType& m) {
                return vcl::vertexSelectionNumber(m);
            });

            // topology.h

            m.def(
                "referenced_vertices",
                [](const MeshType& m, bool onlyFaces) {
                    uint nUnref = 0;
                    return vcl::referencedVertices<std::vector<bool>>(
                        m, nUnref, onlyFaces);
                },
                py::arg("mesh"),
                py::arg("only_faces") = false);

            m.def(
                "number_unreferenced_vertices",
                [](const MeshType& m, bool onlyFaces) {
                    return vcl::numberUnreferencedVertices(m, onlyFaces);
                },
                py::arg("mesh"),
                py::arg("only_faces") = false);
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // barycenter.h

        m.def("shell_barycenter", [](const MeshType& m) {
            return vcl::shellBarycenter(m);
        });

        // geometry.h

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

        // quality.h

        m.def("face_quality_min_max", [](const MeshType& m) {
            return vcl::faceQualityMinMax(m);
        });

        m.def("face_quality_average", [](const MeshType& m) {
            return vcl::faceQualityAverage(m);
        });

        // todo: face_quality_histogram - need histogram class

        // selection.h

        m.def("face_selection_number", [](const MeshType& m) {
            return vcl::faceSelectionNumber(m);
        });

        m.def("face_edges_selection_number", [](const MeshType& m) {
            return vcl::faceEdgesSelectionNumber(m);
        });

        // topology.h

        m.def("count_per_face_vertex_references", [](const MeshType& m) {
            return vcl::countPerFaceVertexReferences(m);
        });

        m.def("largest_face_size", [](const MeshType& m) {
            return vcl::largestFaceSize(m);
        });

        m.def("count_trinagulated_triangles", [](const MeshType& m) {
            return vcl::countTriangulatedTriangles(m);
        });

        m.def("number_non_manifold_vertices", [](const MeshType& m) {
            return vcl::numberNonManifoldVertices(m);
        });

        m.def("is_water_tight", [](const MeshType& m) {
            return vcl::isWaterTight(m);
        });

        m.def("number_holes", [](const MeshType& m) {
            return vcl::numberHoles(m);
        });

        m.def("connected_components", [](const MeshType& m) {
            return vcl::connectedComponents(m);
        });

        m.def("number_connected_components", [](const MeshType& m) {
            return vcl::numberConnectedComponents(m);
        });
    };

    defForAllMeshTypes(m, fFaceMeshes);
}

} // namespace vcl::bind
