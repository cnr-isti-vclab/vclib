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
#include <pybind11/stl.h>

namespace vcl::bind {

void initImportExportAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        // export_matrix.h

        m.def(
            "vertex_positions_matrix",
            [](const MeshType& m) {
                return vcl::vertexPositionsMatrix<Eigen::MatrixX3d>(m);
            },
            "mesh"_a);

        m.def(
            "vertex_selection_array",
            [](const MeshType& m) {
                return vcl::vertexSelectionVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "vertex_selection_list",
            [](const MeshType& m) {
                return vcl::vertexSelectionVector<std::vector<int>>(m);
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
            "vertex_colors_array",
            [](const MeshType& m, Color::Format colorFormat) {
                return vcl::vertexColorsVector<Eigen::VectorXi>(m, colorFormat);
            },
            "mesh"_a,
            "color_format"_a = Color::Format::RGBA);

        m.def(
            "vertex_colors_list",
            [](const MeshType& m, Color::Format colorFormat) {
                return vcl::vertexColorsVector<std::vector<uint>>(
                    m, colorFormat);
            },
            "mesh"_a,
            "color_format"_a = Color::Format::RGBA);

        m.def(
            "vertex_quality_array",
            [](const MeshType& m) {
                return vcl::vertexQualityVector<Eigen::VectorXd>(m);
            },
            "mesh"_a);

        m.def(
            "vertex_quality_list",
            [](const MeshType& m) {
                return vcl::vertexQualityVector<std::vector<double>>(m);
            },
            "mesh"_a);

        // import_matrix.h

        m.def(
            camelCaseToSnakeCase(meshTypeName<MeshType>() + "_from_matrices")
                .c_str(),
            [](Eigen::MatrixX3d& V, Eigen::MatrixXi& F, Eigen::MatrixX2i& E) {
                return vcl::meshFromMatrices<MeshType>(V, F, E);
            },
            "vertices"_a,
            "faces"_a = Eigen::MatrixXi(),
            "edges"_a = Eigen::MatrixX2i());

        m.def(
            "mesh_from_matrices",
            [](MeshType&         mesh,
               Eigen::MatrixX3d& V,
               Eigen::MatrixXi&  F,
               Eigen::MatrixX2i& E) {
                return vcl::meshFromMatrices(mesh, V, F, E);
            },
            "mesh"_a,
            "vertices"_a,
            "faces"_a = Eigen::MatrixXi(),
            "edges"_a = Eigen::MatrixX2i());

        m.def(
            "vertex_positions_from_matrix",
            [](MeshType&               mesh,
               const Eigen::MatrixX3d& vertexPositions,
               bool                    clearBeforeSet) {
                return vcl::vertexPositionsFromMatrix(
                    mesh, vertexPositions, clearBeforeSet);
            },
            "mesh"_a,
            "vertex_positions"_a,
            "clear_before_set"_a = true);

        m.def(
            "vertex_selection_from_array",
            [](MeshType& mesh, const Eigen::VectorXi& vertexSelection) {
                return vcl::vertexSelectionFromRange(mesh, vertexSelection);
            },
            "mesh"_a,
            "vertex_selection"_a);

        m.def(
            "vertex_selection_from_list",
            [](MeshType& mesh, const std::vector<int>& vertexSelection) {
                return vcl::vertexSelectionFromRange(mesh, vertexSelection);
            },
            "mesh"_a,
            "vertex_selection"_a);

        m.def(
            "vertex_normals_from_matrix",
            [](MeshType& mesh, const Eigen::MatrixX3d& vertexNormals) {
                return vcl::vertexNormalsFromMatrix(mesh, vertexNormals);
            },
            "mesh"_a,
            "vertex_normals"_a);
    };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // export_matrix.h

        m.def(
            "face_sizes_array",
            [](const MeshType& m) {
                return vcl::faceSizesVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "face_sizes_list",
            [](const MeshType& m) {
                return vcl::faceSizesVector<std::vector<uint>>(m);
            },
            "mesh"_a);

        m.def(
            "face_indices_array",
            [](const MeshType& m) {
                return vcl::faceIndicesVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "face_indices_list",
            [](const MeshType& m) {
                return vcl::faceIndicesVector<std::vector<uint>>(m);
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
            "face_selection_array",
            [](const MeshType& m) {
                return vcl::faceSelectionVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "face_selection_list",
            [](const MeshType& m) {
                return vcl::faceSelectionVector<std::vector<int>>(m);
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
            "face_colors_array",
            [](const MeshType& m, Color::Format colorFormat) {
                return vcl::faceColorsVector<Eigen::VectorXi>(m, colorFormat);
            },
            "mesh"_a,
            "color_format"_a = Color::Format::RGBA);

        m.def(
            "face_colors_list",
            [](const MeshType& m, Color::Format colorFormat) {
                return vcl::faceColorsVector<std::vector<uint>>(m, colorFormat);
            },
            "mesh"_a,
            "color_format"_a = Color::Format::RGBA);

        m.def(
            "face_quality_array",
            [](const MeshType& m) {
                return vcl::faceQualityVector<Eigen::VectorXd>(m);
            },
            "mesh"_a);

        m.def(
            "face_quality_list",
            [](const MeshType& m) {
                return vcl::faceQualityVector<std::vector<double>>(m);
            },
            "mesh"_a);

        // import_matrix.h

        m.def(
            "face_indices_from_matrix",
            [](MeshType&              mesh,
               const Eigen::MatrixXi& faces,
               bool                   clearBeforeSet) {
                return vcl::faceIndicesFromMatrix(mesh, faces, clearBeforeSet);
            },
            "mesh"_a,
            "faces"_a,
            "clear_before_set"_a = true);

        m.def(
            "face_selection_from_array",
            [](MeshType& mesh, const Eigen::VectorXi& faceSelection) {
                return vcl::faceSelectionFromRange(mesh, faceSelection);
            },
            "mesh"_a,
            "face_selection"_a);

        m.def(
            "face_selection_from_list",
            [](MeshType& mesh, const std::vector<int>& faceSelection) {
                return vcl::faceSelectionFromRange(mesh, faceSelection);
            },
            "mesh"_a,
            "face_selection"_a);

        m.def(
            "face_normals_from_matrix",
            [](MeshType& mesh, const Eigen::MatrixX3d& faceNormals) {
                return vcl::faceNormalsFromMatrix(mesh, faceNormals);
            },
            "mesh"_a,
            "face_normals"_a);
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
            "edge_selection_array",
            [](const MeshType& m) {
                return vcl::edgeSelectionVector<Eigen::VectorXi>(m);
            },
            "mesh"_a);

        m.def(
            "edge_selection_list",
            [](const MeshType& m) {
                return vcl::edgeSelectionVector<std::vector<int>>(m);
            },
            "mesh"_a);

        m.def(
            "edge_colors_matrix",
            [](const MeshType& m) {
                return vcl::edgeColorsMatrix<Eigen::MatrixX4i>(m);
            },
            "mesh"_a);

        m.def(
            "edge_colors_array",
            [](const MeshType& m, Color::Format colorFormat) {
                return vcl::edgeColorsVector<Eigen::VectorXi>(m, colorFormat);
            },
            "mesh"_a,
            "color_format"_a = Color::Format::RGBA);

        m.def(
            "edge_colors_list",
            [](const MeshType& m, Color::Format colorFormat) {
                return vcl::edgeColorsVector<std::vector<uint>>(m, colorFormat);
            },
            "mesh"_a,
            "color_format"_a = Color::Format::RGBA);

        m.def(
            "edge_quality_array",
            [](const MeshType& m) {
                return vcl::edgeQualityVector<Eigen::VectorXd>(m);
            },
            "mesh"_a);

        m.def(
            "edge_quality_list",
            [](const MeshType& m) {
                return vcl::edgeQualityVector<std::vector<double>>(m);
            },
            "mesh"_a);

        // import_matrix.h

        m.def(
            "edge_indices_from_matrix",
            [](MeshType&               mesh,
               const Eigen::MatrixX2i& edges,
               bool                    clearBeforeSet) {
                return vcl::edgeIndicesFromMatrix(mesh, edges, clearBeforeSet);
            },
            "mesh"_a,
            "edges"_a,
            "clear_before_set"_a = true);

        m.def(
            "edge_selection_from_array",
            [](MeshType& mesh, const Eigen::VectorXi& edgeSelection) {
                return vcl::edgeSelectionFromRange(mesh, edgeSelection);
            },
            "mesh"_a,
            "edge_selection"_a);

        m.def(
            "edge_selection_from_list",
            [](MeshType& mesh, const std::vector<int>& edgeSelection) {
                return vcl::edgeSelectionFromRange(mesh, edgeSelection);
            },
            "mesh"_a,
            "edge_selection"_a);

        m.def(
            "edge_normals_from_matrix",
            [](MeshType& mesh, const Eigen::MatrixX3d& edgeNormals) {
                return vcl::edgeNormalsFromMatrix(mesh, edgeNormals);
            },
            "mesh"_a,
            "edge_normals"_a);
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
