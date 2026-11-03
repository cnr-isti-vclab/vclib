// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh/import_export/export_matrix.h>
#include <vclib/algorithms/mesh/import_export/import_matrix.h>

#include <Eigen/Core>

// Full Mesh Creation / Population
BENCH(
    "import_export/import_matrix/mesh_from_matrices",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            using Scalar = typename Mesh::ScalarType;
            const auto V =
                vcl::vertexPositionsMatrix<Eigen::MatrixX3<Scalar>>(s.mesh());
            const auto F =
                vcl::faceVertexIndicesMatrix<Eigen::MatrixX3i>(s.mesh());

            s.run([&]() { return vcl::meshFromMatrices<Mesh>(V, F); });
        }
    });

BENCH(
    "import_export/import_matrix/mesh_from_matrices_into",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            using Scalar = typename Mesh::ScalarType;
            const auto V =
                vcl::vertexPositionsMatrix<Eigen::MatrixX3<Scalar>>(s.mesh());
            const auto F =
                vcl::faceVertexIndicesMatrix<Eigen::MatrixX3i>(s.mesh());
            Mesh m;
            s.run([&]() { vcl::meshFromMatrices(m, V, F); });
        }
    });

// Positions
BENCH(
    "import_export/import_matrix/vertex_positions_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        using Scalar = typename Mesh::ScalarType;
        const auto V =
            vcl::vertexPositionsMatrix<Eigen::MatrixX3<Scalar>>(s.mesh());
        Mesh m = s.copyMesh();
        s.run([&]() { vcl::vertexPositionsFromMatrix(m, V, false); });
    });

// Faces & Edges
BENCH(
    "import_export/import_matrix/face_indices_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const auto F =
                vcl::faceVertexIndicesMatrix<Eigen::MatrixX3i>(s.mesh());
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::faceIndicesFromMatrix(m, F, false); });
        }
    });

BENCH(
    "import_export/import_matrix/edge_indices_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::EdgeMeshConcept<Mesh>) {
            return s.skip("the mesh has no edges");
        }
        else {
            const auto E =
                vcl::edgeVertexIndicesMatrix<Eigen::MatrixX2i>(s.mesh());
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::edgeIndicesFromMatrix(m, E, false); });
        }
    });

// Selection
BENCH(
    "import_export/import_matrix/vertex_selection_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const auto S =
            vcl::vertexSelectionVector<Eigen::VectorXi>(s.meshWithSelection());
        Mesh m = s.copyMesh();
        s.run([&]() { vcl::vertexSelectionFromRange(m, S); });
    });

BENCH(
    "import_export/import_matrix/face_selection_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const auto S = vcl::faceSelectionVector<Eigen::VectorXi>(
                s.meshWithSelection());
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::faceSelectionFromRange(m, S); });
        }
    });

BENCH(
    "import_export/import_matrix/edge_selection_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::EdgeMeshConcept<Mesh>) {
            return s.skip("the mesh has no edges");
        }
        else {
            const auto S = vcl::edgeSelectionVector<Eigen::VectorXi>(
                s.meshWithSelection());
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::edgeSelectionFromRange(m, S); });
        }
    });

// Normals
BENCH(
    "import_export/import_matrix/vertex_normals_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexNormal<Mesh>) {
            return s.skip("the mesh type has no vertex normals");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerVertexNormalAvailable(mSrc)) {
                return s.skip("the mesh has no vertex normals");
            }
            using Scalar = typename Mesh::ScalarType;
            const auto VN =
                vcl::vertexNormalsMatrix<Eigen::MatrixX3<Scalar>>(mSrc);
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::vertexNormalsFromMatrix(m, VN); });
        }
    });

BENCH(
    "import_export/import_matrix/face_normals_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> || !vcl::HasPerFaceNormal<Mesh>) {
            return s.skip("the mesh has no face normals");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerFaceNormalAvailable(mSrc)) {
                return s.skip("the mesh has no face normals");
            }
            using Scalar = typename Mesh::ScalarType;
            const auto FN =
                vcl::faceNormalsMatrix<Eigen::MatrixX3<Scalar>>(mSrc);
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::faceNormalsFromMatrix(m, FN); });
        }
    });

BENCH(
    "import_export/import_matrix/edge_normals_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::HasPerEdgeNormal<Mesh>) {
            return s.skip("the mesh has no edge normals");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerEdgeNormalAvailable(mSrc)) {
                return s.skip("the mesh has no edge normals");
            }
            using Scalar  = typename Mesh::ScalarType;
            const auto EN = vcl::elementNormalsMatrix<
                vcl::ElemId::EDGE,
                Eigen::MatrixX3<Scalar>>(mSrc);
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::edgeNormalsFromMatrix(m, EN); });
        }
    });

// Colors
BENCH(
    "import_export/import_matrix/vertex_colors_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexColor<Mesh>) {
            return s.skip("the mesh type has no vertex colors");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerVertexColorAvailable(mSrc)) {
                return s.skip("the mesh has no vertex colors");
            }
            const auto VC = vcl::vertexColorsMatrix<Eigen::MatrixX4i>(mSrc);
            Mesh       m  = s.copyMesh();
            s.run([&]() { vcl::vertexColorsFromMatrix(m, VC); });
        }
    });

BENCH(
    "import_export/import_matrix/vertex_colors_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexColor<Mesh>) {
            return s.skip("the mesh type has no vertex colors");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerVertexColorAvailable(mSrc)) {
                return s.skip("the mesh has no vertex colors");
            }
            const auto VC = vcl::vertexColorsVector<Eigen::VectorXi>(
                mSrc, vcl::Color::Format::RGBA);
            Mesh m = s.copyMesh();
            s.run([&]() {
                vcl::vertexColorsFromRange(m, VC, vcl::Color::Format::RGBA);
            });
        }
    });

BENCH(
    "import_export/import_matrix/face_colors_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> || !vcl::HasPerFaceColor<Mesh>) {
            return s.skip("the mesh has no face colors");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerFaceColorAvailable(mSrc)) {
                return s.skip("the mesh has no face colors");
            }
            const auto FC = vcl::faceColorsMatrix<Eigen::MatrixX4i>(mSrc);
            Mesh       m  = s.copyMesh();
            s.run([&]() { vcl::faceColorsFromMatrix(m, FC); });
        }
    });

BENCH(
    "import_export/import_matrix/face_colors_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> || !vcl::HasPerFaceColor<Mesh>) {
            return s.skip("the mesh has no face colors");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerFaceColorAvailable(mSrc)) {
                return s.skip("the mesh has no face colors");
            }
            const auto FC = vcl::faceColorsVector<Eigen::VectorXi>(
                mSrc, vcl::Color::Format::RGBA);
            Mesh m = s.copyMesh();
            s.run([&]() {
                vcl::faceColorsFromRange(m, FC, vcl::Color::Format::RGBA);
            });
        }
    });

BENCH(
    "import_export/import_matrix/edge_colors_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::HasPerEdgeColor<Mesh>) {
            return s.skip("the mesh has no edge colors");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerEdgeColorAvailable(mSrc)) {
                return s.skip("the mesh has no edge colors");
            }
            const auto EC = vcl::edgeColorsMatrix<Eigen::MatrixX4i>(mSrc);
            Mesh       m  = s.copyMesh();
            s.run([&]() { vcl::edgeColorsFromMatrix(m, EC); });
        }
    });

BENCH(
    "import_export/import_matrix/edge_colors_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::HasPerEdgeColor<Mesh>) {
            return s.skip("the mesh has no edge colors");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerEdgeColorAvailable(mSrc)) {
                return s.skip("the mesh has no edge colors");
            }
            const auto EC = vcl::edgeColorsVector<Eigen::VectorXi>(
                mSrc, vcl::Color::Format::RGBA);
            Mesh m = s.copyMesh();
            s.run([&]() {
                vcl::edgeColorsFromRange(m, EC, vcl::Color::Format::RGBA);
            });
        }
    });

// Quality
BENCH(
    "import_export/import_matrix/vertex_quality_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexQuality<Mesh>) {
            return s.skip("the mesh type has no vertex quality component");
        }
        else {
            using Scalar  = typename Mesh::ScalarType;
            const auto VQ = vcl::vertexQualityVector<Eigen::VectorX<Scalar>>(
                s.meshWithQuality());
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::vertexQualityFromRange(m, VQ); });
        }
    });

BENCH(
    "import_export/import_matrix/face_quality_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> || !vcl::HasPerFaceQuality<Mesh>) {
            return s.skip("the mesh has no face quality component");
        }
        else {
            using Scalar  = typename Mesh::ScalarType;
            const auto FQ = vcl::faceQualityVector<Eigen::VectorX<Scalar>>(
                s.meshWithQuality());
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::faceQualityFromRange(m, FQ); });
        }
    });

BENCH(
    "import_export/import_matrix/edge_quality_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::EdgeMeshConcept<Mesh> || !vcl::HasPerEdgeQuality<Mesh>) {
            return s.skip("the mesh has no edge quality component");
        }
        else {
            using Scalar  = typename Mesh::ScalarType;
            const auto EQ = vcl::edgeQualityVector<Eigen::VectorX<Scalar>>(
                s.meshWithQuality());
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::edgeQualityFromRange(m, EQ); });
        }
    });

// Texture Coordinates & Materials
BENCH(
    "import_export/import_matrix/vertex_tex_coords_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexTexCoord<Mesh>) {
            return s.skip("the mesh type has no vertex texcoords");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerVertexTexCoordAvailable(mSrc)) {
                return s.skip("the mesh has no vertex texcoords");
            }
            using Scalar = typename Mesh::ScalarType;
            const auto VT =
                vcl::vertexTexCoordsMatrix<Eigen::MatrixX2<Scalar>>(mSrc);
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::vertexTexCoordsFromMatrix(m, VT); });
        }
    });

BENCH(
    "import_export/import_matrix/face_wedge_tex_coords_from_matrix",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> ||
            !vcl::HasPerFaceWedgeTexCoords<Mesh>) {
            return s.skip("the mesh has no face wedge texcoords");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerFaceWedgeTexCoordsAvailable(mSrc)) {
                return s.skip("the mesh has no face wedge texcoords");
            }
            using Scalar = typename Mesh::ScalarType;
            const auto FWT =
                vcl::faceWedgeTexCoordsMatrix<Eigen::MatrixX<Scalar>>(mSrc);
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::faceWedgeTexCoordsFromMatrix(m, FWT); });
        }
    });

BENCH(
    "import_export/import_matrix/vertex_material_indices_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexMaterialIndex<Mesh>) {
            return s.skip("the mesh type has no vertex material indices");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerVertexMaterialIndexAvailable(mSrc)) {
                return s.skip("the mesh has no vertex material indices");
            }
            const auto VMI =
                vcl::vertexMaterialIndicesVector<Eigen::VectorXi>(mSrc);
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::vertexMaterialIndicesFromRange(m, VMI); });
        }
    });

BENCH(
    "import_export/import_matrix/face_material_indices_from_range",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (
            !vcl::FaceMeshConcept<Mesh> ||
            !vcl::HasPerFaceMaterialIndex<Mesh>) {
            return s.skip("the mesh has no face material indices");
        }
        else {
            const Mesh& mSrc = s.mesh();
            if (!vcl::isPerFaceMaterialIndexAvailable(mSrc)) {
                return s.skip("the mesh has no face material indices");
            }
            const auto FMI =
                vcl::faceMaterialIndicesVector<Eigen::VectorXi>(mSrc);
            Mesh m = s.copyMesh();
            s.run([&]() { vcl::faceMaterialIndicesFromRange(m, FMI); });
        }
    });
