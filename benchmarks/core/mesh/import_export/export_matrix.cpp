// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh/import_export/export_matrix.h>

#include <Eigen/Core>

// Positions
BENCH(
    "import_export/export_matrix/vertex_positions_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        using Scalar  = typename Mesh::ScalarType;
        const Mesh& m = s.mesh();
        s.run([&]() {
            return vcl::vertexPositionsMatrix<Eigen::MatrixX3<Scalar>>(m);
        });
    });

// Faces
BENCH(
    "import_export/export_matrix/face_sizes_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m = s.mesh();
            s.run([&]() { return vcl::faceSizesVector<Eigen::VectorXi>(m); });
        }
    });

BENCH(
    "import_export/export_matrix/face_vertex_indices_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m = s.mesh();
            s.run([&]() {
                return vcl::faceVertexIndicesVector<Eigen::VectorXi>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/face_vertex_indices_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m = s.mesh();
            s.run([&]() {
                return vcl::faceVertexIndicesMatrix<Eigen::MatrixX3i>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/triangulated_face_vertex_indices_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m = s.mesh();
            s.run([&]() {
                return vcl::triangulatedFaceVertexIndicesMatrix<
                    Eigen::MatrixX3i>(m);
            });
        }
    });

// Edges
BENCH(
    "import_export/export_matrix/edge_vertex_indices_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::EdgeMeshConcept<Mesh>) {
            return s.skip("the mesh has no edges");
        }
        else {
            const Mesh& m = s.mesh();
            s.run([&]() {
                return vcl::edgeVertexIndicesMatrix<Eigen::MatrixX2i>(m);
            });
        }
    });

// Selection
BENCH(
    "import_export/export_matrix/vertex_selection_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.meshWithSelection();
        s.run([&]() { return vcl::vertexSelectionVector<Eigen::VectorXi>(m); });
    });

BENCH(
    "import_export/export_matrix/face_selection_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m = s.meshWithSelection();
            s.run(
                [&]() { return vcl::faceSelectionVector<Eigen::VectorXi>(m); });
        }
    });

BENCH(
    "import_export/export_matrix/edge_selection_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::EdgeMeshConcept<Mesh>) {
            return s.skip("the mesh has no edges");
        }
        else {
            const Mesh& m = s.meshWithSelection();
            s.run(
                [&]() { return vcl::edgeSelectionVector<Eigen::VectorXi>(m); });
        }
    });

// Normals
BENCH(
    "import_export/export_matrix/vertex_normals_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexNormal<Mesh>) {
            return s.skip("the mesh type has no vertex normals");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerVertexNormalAvailable(m)) {
                return s.skip("the mesh has no vertex normals");
            }
            using Scalar = typename Mesh::ScalarType;
            s.run([&]() {
                return vcl::vertexNormalsMatrix<Eigen::MatrixX3<Scalar>>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/face_normals_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> || !vcl::HasPerFaceNormal<Mesh>) {
            return s.skip("the mesh has no face normals");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerFaceNormalAvailable(m)) {
                return s.skip("the mesh has no face normals");
            }
            using Scalar = typename Mesh::ScalarType;
            s.run([&]() {
                return vcl::faceNormalsMatrix<Eigen::MatrixX3<Scalar>>(m);
            });
        }
    });

// Colors
BENCH(
    "import_export/export_matrix/vertex_colors_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexColor<Mesh>) {
            return s.skip("the mesh type has no vertex colors");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerVertexColorAvailable(m)) {
                return s.skip("the mesh has no vertex colors");
            }
            s.run(
                [&]() { return vcl::vertexColorsMatrix<Eigen::MatrixX4i>(m); });
        }
    });

BENCH(
    "import_export/export_matrix/vertex_colors_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexColor<Mesh>) {
            return s.skip("the mesh type has no vertex colors");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerVertexColorAvailable(m)) {
                return s.skip("the mesh has no vertex colors");
            }
            s.run([&]() {
                return vcl::vertexColorsVector<Eigen::VectorXi>(
                    m, vcl::Color::Format::RGBA);
            });
        }
    });

BENCH(
    "import_export/export_matrix/face_colors_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> || !vcl::HasPerFaceColor<Mesh>) {
            return s.skip("the mesh has no face colors");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerFaceColorAvailable(m)) {
                return s.skip("the mesh has no face colors");
            }
            s.run([&]() { return vcl::faceColorsMatrix<Eigen::MatrixX4i>(m); });
        }
    });

BENCH(
    "import_export/export_matrix/face_colors_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> || !vcl::HasPerFaceColor<Mesh>) {
            return s.skip("the mesh has no face colors");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerFaceColorAvailable(m)) {
                return s.skip("the mesh has no face colors");
            }
            s.run([&]() {
                return vcl::faceColorsVector<Eigen::VectorXi>(
                    m, vcl::Color::Format::RGBA);
            });
        }
    });

BENCH(
    "import_export/export_matrix/edge_colors_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::HasPerEdgeColor<Mesh>) {
            return s.skip("the mesh has no edge colors");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerEdgeColorAvailable(m)) {
                return s.skip("the mesh has no edge colors");
            }
            s.run([&]() { return vcl::edgeColorsMatrix<Eigen::MatrixX4i>(m); });
        }
    });

BENCH(
    "import_export/export_matrix/edge_colors_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::HasPerEdgeColor<Mesh>) {
            return s.skip("the mesh has no edge colors");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerEdgeColorAvailable(m)) {
                return s.skip("the mesh has no edge colors");
            }
            s.run([&]() {
                return vcl::edgeColorsVector<Eigen::VectorXi>(
                    m, vcl::Color::Format::RGBA);
            });
        }
    });

// Quality
BENCH(
    "import_export/export_matrix/vertex_quality_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexQuality<Mesh>) {
            return s.skip("the mesh type has no vertex quality component");
        }
        else {
            const Mesh& m = s.meshWithQuality();
            using Scalar  = typename Mesh::ScalarType;
            s.run([&]() {
                return vcl::vertexQualityVector<Eigen::VectorX<Scalar>>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/face_quality_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> || !vcl::HasPerFaceQuality<Mesh>) {
            return s.skip("the mesh has no face quality component");
        }
        else {
            const Mesh& m = s.meshWithQuality();
            using Scalar  = typename Mesh::ScalarType;
            s.run([&]() {
                return vcl::faceQualityVector<Eigen::VectorX<Scalar>>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/edge_quality_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::HasPerEdgeQuality<Mesh>) {
            return s.skip("the mesh has no edge quality component");
        }
        else {
            const Mesh& m = s.meshWithQuality();
            using Scalar  = typename Mesh::ScalarType;
            s.run([&]() {
                return vcl::edgeQualityVector<Eigen::VectorX<Scalar>>(m);
            });
        }
    });

// Texture Coordinates & Materials
BENCH(
    "import_export/export_matrix/vertex_tex_coords_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexTexCoord<Mesh>) {
            return s.skip("the mesh type has no vertex texcoords");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerVertexTexCoordAvailable(m)) {
                return s.skip("the mesh has no vertex texcoords");
            }
            using Scalar = typename Mesh::ScalarType;
            s.run([&]() {
                return vcl::vertexTexCoordsMatrix<Eigen::MatrixX2<Scalar>>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/face_wedge_tex_coords_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> ||
            !vcl::HasPerFaceWedgeTexCoords<Mesh>) {
            return s.skip("the mesh has no face wedge texcoords");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerFaceWedgeTexCoordsAvailable(m)) {
                return s.skip("the mesh has no face wedge texcoords");
            }
            using Scalar = typename Mesh::ScalarType;
            s.run([&]() {
                return vcl::faceWedgeTexCoordsMatrix<Eigen::MatrixX<Scalar>>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/vertex_material_indices_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexMaterialIndex<Mesh>) {
            return s.skip("the mesh type has no vertex material indices");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerVertexMaterialIndexAvailable(m)) {
                return s.skip("the mesh has no vertex material indices");
            }
            s.run([&]() {
                return vcl::vertexMaterialIndicesVector<Eigen::VectorXi>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/face_material_indices_vector",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> ||
            !vcl::HasPerFaceMaterialIndex<Mesh>) {
            return s.skip("the mesh has no face material indices");
        }
        else {
            const Mesh& m = s.mesh();
            if (!vcl::isPerFaceMaterialIndexAvailable(m)) {
                return s.skip("the mesh has no face material indices");
            }
            s.run([&]() {
                return vcl::faceMaterialIndicesVector<Eigen::VectorXi>(m);
            });
        }
    });

// Adjacency
BENCH(
    "import_export/export_matrix/vertex_adjacent_vertices_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexAdjacentVertices<Mesh>) {
            return s.skip("the mesh type has no per-vertex adjacent vertices");
        }
        else {
            const Mesh& m = s.meshWithTopology();
            if (!vcl::isPerVertexAdjacentVerticesAvailable(m)) {
                return s.skip("per-vertex adjacent vertices not available");
            }
            s.run([&]() {
                return vcl::vertexAdjacentVerticesMatrix<Eigen::MatrixXi>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/vertex_adjacent_faces_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> ||
            !vcl::HasPerVertexAdjacentFaces<Mesh>) {
            return s.skip("the mesh has no per-vertex adjacent faces");
        }
        else {
            const Mesh& m = s.meshWithTopology();
            if (!vcl::isPerVertexAdjacentFacesAvailable(m)) {
                return s.skip("per-vertex adjacent faces not available");
            }
            s.run([&]() {
                return vcl::vertexAdjacentFacesMatrix<Eigen::MatrixXi>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/face_adjacent_faces_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> ||
            !vcl::HasPerFaceAdjacentFaces<Mesh>) {
            return s.skip("the mesh has no per-face adjacent faces");
        }
        else {
            const Mesh& m = s.meshWithTopology();
            if (!vcl::isPerFaceAdjacentFacesAvailable(m)) {
                return s.skip("per-face adjacent faces not available");
            }
            s.run([&]() {
                return vcl::faceAdjacentFacesMatrix<Eigen::MatrixXi>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/edge_adjacent_faces_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::FaceMeshConcept<Mesh> ||
            !vcl::HasPerEdgeAdjacentFaces<Mesh>) {
            return s.skip("the mesh has no per-edge adjacent faces");
        }
        else {
            const Mesh& m = s.meshWithTopology();
            if (!vcl::isPerEdgeAdjacentFacesAvailable(m)) {
                return s.skip("per-edge adjacent faces not available");
            }
            s.run([&]() {
                return vcl::edgeAdjacentFacesMatrix<Eigen::MatrixXi>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/vertex_adjacent_edges_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> ||
            !vcl::HasPerVertexAdjacentEdges<Mesh>) {
            return s.skip("the mesh has no per-vertex adjacent edges");
        }
        else {
            const Mesh& m = s.meshWithTopology();
            if (!vcl::isPerElementComponentAvailable<
                    vcl::ElemId::VERTEX,
                    vcl::CompId::ADJACENT_EDGES>(m)) {
                return s.skip("per-vertex adjacent edges not available");
            }
            s.run([&]() {
                return vcl::vertexAdjacentEdgesMatrix<Eigen::MatrixXi>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/face_adjacent_edges_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::FaceMeshConcept<Mesh> ||
            !vcl::HasPerFaceAdjacentEdges<Mesh>) {
            return s.skip("the mesh has no per-face adjacent edges");
        }
        else {
            const Mesh& m = s.meshWithTopology();
            if (!vcl::isPerFaceAdjacentEdgesAvailable(m)) {
                return s.skip("per-face adjacent edges not available");
            }
            s.run([&]() {
                return vcl::faceAdjacentEdgesMatrix<Eigen::MatrixXi>(m);
            });
        }
    });

BENCH(
    "import_export/export_matrix/edge_adjacent_edges_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> ||
            !vcl::HasPerEdgeAdjacentEdges<Mesh>) {
            return s.skip("the mesh has no per-edge adjacent edges");
        }
        else {
            const Mesh& m = s.meshWithTopology();
            if (!vcl::isPerEdgeAdjacentEdgesAvailable(m)) {
                return s.skip("per-edge adjacent edges not available");
            }
            s.run([&]() {
                return vcl::edgeAdjacentEdgesMatrix<Eigen::MatrixXi>(m);
            });
        }
    });
