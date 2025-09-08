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

#include <vclib/bindings/core/algorithms/mesh/import_export.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/eigen.h>

namespace vcl::bind {

void initImportExportAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            // export_matrix.h

            m.def(
                "vertex_positions_matrix",
                [](const MeshType& m) {
                    return vcl::vertexPositionsMatrix<Eigen::MatrixX3d>(m);
                },
                "mesh"_a);

            m.def(
                "vertex_selection_vector",
                [](const MeshType& m) {
                    return vcl::vertexSelectionVector<Eigen::VectorXi>(m);
                },
                "mesh"_a);

            m.def(
                "vertex_normals_matrix",
                [](const MeshType& m) {
                    return vcl::vertexNormalsMatrix<Eigen::MatrixX3d>(m);
                },
                "mesh"_a);

            m.def(
                "vertex_colors_matrix",
                [](const MeshType& m) {
                    return vcl::vertexColorsMatrix<Eigen::MatrixX4i>(m);
                },
                "mesh"_a);

            m.def(
                "vertex_colors_vector",
                [](const MeshType& m, Color::Format colorFormat) {
                    return vcl::vertexColorsVector<Eigen::VectorXi>(m, colorFormat);
                },
                "mesh"_a,
                "color_format"_a = Color::Format::RGBA);

            m.def(
                "vertex_quality_vector",
                [](const MeshType& m) {
                    return vcl::vertexQualityVector<Eigen::VectorXd>(m);
                },
                "mesh"_a);

            // import_matrix.h

            m.def(camelCaseToSnakeCase(meshTypeName<MeshType>() + "_from_matrices").c_str(),
                  [](Eigen::MatrixX3d& V,
                     Eigen::MatrixXi& F,
                     Eigen::MatrixX3d& VN,
                     Eigen::MatrixX3d& FN) {
                      return vcl::meshFromMatrices<MeshType>(V, F, VN, FN);
                  },
                  "vertices"_a,
                  "faces"_a = Eigen::MatrixXi(),
                  "vertex_normals"_a = Eigen::MatrixX3d(),
                  "face_normals"_a = Eigen::MatrixX3d());

            m.def(
                "mesh_from_matrices",
                [](MeshType& mesh,
                   Eigen::MatrixX3d& V,
                   Eigen::MatrixXi& F,
                   Eigen::MatrixX3d& VN,
                   Eigen::MatrixX3d& FN) {
                    return vcl::importMeshFromMatrices<MeshType>(mesh, V, F, VN, FN);
                },
                "mesh"_a,
                "vertices"_a,
                "faces"_a = Eigen::MatrixXi(),
                "vertex_normals"_a = Eigen::MatrixX3d(),
                "face_normals"_a = Eigen::MatrixX3d());
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // export_matrix.h

        m.def(
            "face_sizes_vector",
            [](const MeshType& m) {
                return vcl::faceSizesVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "face_indices_vector",
            [](const MeshType& m) {
                return vcl::faceIndicesVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "face_indices_matrix",
            [](const MeshType& m) {
                return vcl::faceIndicesMatrix<Eigen::MatrixXi>(m);
            },
            "mesh"_a);

        m.def(
            "triangulated_face_indices_matrix",
            [](const MeshType& m) {
                return vcl::triangulatedFaceIndicesMatrix<Eigen::MatrixXi>(m);
            },
            "mesh"_a);

        m.def(
            "triangulated_face_indices_matrix",
            [](const MeshType& m, TriPolyIndexBiMap& indexMap) {
                return vcl::triangulatedFaceIndicesMatrix<Eigen::MatrixX3i>(
                    m, indexMap);
            },
            "mesh"_a,
            "index_map"_a = TriPolyIndexBiMap());

        m.def(
            "face_selection_vector",
            [](const MeshType& m) {
                return vcl::faceSelectionVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "face_normals_matrix",
            [](const MeshType& m) {
                return vcl::faceNormalsMatrix<Eigen::MatrixX3d>(m);
            },
            "mesh"_a);

        m.def(
            "face_colors_matrix",
            [](const MeshType& m) {
                return vcl::faceColorsMatrix<Eigen::MatrixX4i>(m);
            },
            "mesh"_a);

        m.def(
            "face_colors_vector",
            [](const MeshType& m, Color::Format colorFormat) {
                return vcl::faceColorsVector<Eigen::VectorXi>(m, colorFormat);
            },
            "mesh"_a,
            "color_format"_a = Color::Format::RGBA);

        m.def(
            "face_quality_vector",
            [](const MeshType& m) {
                return vcl::faceQualityVector<Eigen::VectorXd>(m);
            },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // export_matrix.h

        m.def(
            "edge_indices_matrix",
            [](const MeshType& m) {
                return vcl::edgeIndicesMatrix<Eigen::MatrixX2i>(m);
            },
            "mesh"_a);

        m.def(
            "edge_selection_vector",
            [](const MeshType& m) {
                return vcl::edgeSelectionVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "edge_colors_matrix",
            [](const MeshType& m) {
                return vcl::edgeColorsMatrix<Eigen::MatrixX4i>(m);
            },
            "mesh"_a);

        m.def(
            "edge_colors_vector",
            [](const MeshType& m, Color::Format colorFormat) {
                return vcl::edgeColorsVector<Eigen::VectorXi>(m, colorFormat);
            },
            "mesh"_a,
            "color_format"_a = Color::Format::RGBA);

        m.def(
            "edge_quality_vector",
            [](const MeshType& m) {
                return vcl::edgeQualityVector<Eigen::VectorXd>(m);
            },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
