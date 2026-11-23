// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms/mesh/import_export/import_matrix.h>
#include <vclib/meshes/tri_mesh.h>

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <vector>

TEST_CASE("Import mesh - error handling")
{
    vcl::TriMeshf mesh;

    SECTION("Wrong vertex matrix size")
    {
        // Matrix with wrong number of columns (should be 3)
        Eigen::MatrixXf wrongVertices(3, 2);
        wrongVertices << 0.0, 0.0, 1.0, 0.0, 0.5, 1.0;

        REQUIRE_THROWS_AS(
            vcl::vertexPositionsFromMatrix(mesh, wrongVertices),
            vcl::WrongSizeException);
    }

    SECTION("Wrong selection range size")
    {
        mesh.addVertices(3);
        std::vector<bool> wrongSelection = {true, false}; // size 2, should be 3

        REQUIRE_THROWS_AS(
            vcl::vertexSelectionFromRange(mesh, wrongSelection),
            vcl::WrongSizeException);
    }

    SECTION("Wrong normals matrix size")
    {
        mesh.addVertices(3);
        // Matrix with wrong number of columns (should be 3)
        Eigen::MatrixXf wrongNormals(3, 2);
        wrongNormals << 0.0, 0.0, 1.0, 0.0, 0.0, 1.0;

        REQUIRE_THROWS_AS(
            vcl::vertexNormalsFromMatrix(mesh, wrongNormals),
            vcl::WrongSizeException);
    }

    SECTION("Wrong colors matrix size - wrong columns")
    {
        mesh.addVertices(3);
        // Matrix with wrong number of columns (should be 3 or 4)
        Eigen::MatrixXi wrongColors(3, 2);
        wrongColors << 255, 0, 0, 255, 0, 0;

        REQUIRE_THROWS_AS(
            vcl::elementColorsFromMatrix<vcl::ElemId::VERTEX>(
                mesh, wrongColors),
            vcl::WrongSizeException);
    }

    SECTION("Wrong colors matrix size - wrong rows")
    {
        mesh.addVertices(3);
        // Matrix with wrong number of rows (should be 3)
        Eigen::MatrixXi wrongColors(2, 3);
        wrongColors << 255, 0, 0, 0, 255, 0;

        REQUIRE_THROWS_AS(
            vcl::elementColorsFromMatrix<vcl::ElemId::VERTEX>(
                mesh, wrongColors),
            vcl::WrongSizeException);
    }

    SECTION("Wrong quality range size")
    {
        mesh.addVertices(3);
        std::vector<double> wrongQuality = {1.0, 2.0}; // size 2, should be 3

        REQUIRE_THROWS_AS(
            vcl::vertexQualityFromRange(mesh, wrongQuality),
            vcl::WrongSizeException);
    }

    SECTION("Wrong texture coordinates matrix size - wrong columns")
    {
        mesh.addVertices(3);
        // Matrix with wrong number of columns (should be 2)
        Eigen::MatrixXf wrongTexCoords(3, 3);
        wrongTexCoords << 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.5, 1.0, 0.0;

        REQUIRE_THROWS_AS(
            vcl::vertexTexCoordsFromMatrix(mesh, wrongTexCoords),
            vcl::WrongSizeException);
    }

    SECTION("Wrong texture coordinates matrix size - wrong rows")
    {
        mesh.addVertices(3);
        // Matrix with wrong number of rows (should be 3)
        Eigen::MatrixXf wrongTexCoords(2, 2);
        wrongTexCoords << 0.0, 0.0, 1.0, 0.0;

        REQUIRE_THROWS_AS(
            vcl::vertexTexCoordsFromMatrix(mesh, wrongTexCoords),
            vcl::WrongSizeException);
    }

    SECTION("Wrong vertex material indices range size")
    {
        mesh.addVertices(3);
        std::vector<vcl::uint> wrongMaterialIndices = {
            0, 1}; // size 2, should be 3

        REQUIRE_THROWS_AS(
            vcl::vertexMaterialIndicesFromRange(mesh, wrongMaterialIndices),
            vcl::WrongSizeException);
    }

    SECTION("Wrong face wedge texture coordinates matrix size - wrong rows")
    {
        mesh.addVertices(4);
        mesh.addFace(0, 1, 2);
        mesh.addFace(0, 2, 3); // 2 triangular faces
        // Matrix with wrong number of rows (should be 2)
        Eigen::MatrixXf wrongWedgeTexCoords(1, 6);
        wrongWedgeTexCoords << 0.0, 0.0, 1.0, 0.0, 0.5, 1.0;

        REQUIRE_THROWS_AS(
            vcl::faceWedgeTexCoordsFromMatrix(mesh, wrongWedgeTexCoords),
            vcl::WrongSizeException);
    }

    SECTION("Wrong face material indices range size")
    {
        mesh.addVertices(3);
        mesh.addFace(0, 1, 2); // 1 triangular face, needs 3 indices
        std::vector<vcl::uint> wrongFaceMaterialIndices = {
            0, 1}; // size 2, should be 1

        REQUIRE_THROWS_AS(
            vcl::faceMaterialIndicesFromRange(mesh, wrongFaceMaterialIndices),
            vcl::WrongSizeException);
    }

    SECTION("Wrong vertex colors range size")
    {
        mesh.addVertices(3);
        std::vector<uint32_t> wrongColors = {
            0xFF0000FF, 0xFF00FF00}; // size 2, should be 3

        REQUIRE_THROWS_AS(
            vcl::vertexColorsFromRange(
                mesh, wrongColors, vcl::Color::Format::ABGR),
            vcl::WrongSizeException);
    }

    SECTION("Wrong face colors range size")
    {
        mesh.addVertices(4);
        mesh.addFace(0, 1, 2);
        mesh.addFace(0, 2, 3);                            // 2 triangular faces
        std::vector<uint32_t> wrongColors = {0xFF0000FF}; // size 1, should be 2

        REQUIRE_THROWS_AS(
            vcl::faceColorsFromRange(
                mesh, wrongColors, vcl::Color::Format::ABGR),
            vcl::WrongSizeException);
    }
}

TEST_CASE("Import Material Index Sentinels", "[import][sentinels]")
{
    using TriMesh = vcl::TriMesh;
    TriMesh mesh;
    mesh.addVertices(3);
    mesh.addFace(0, 1, 2);

    SECTION("Import from signed integers")
    {
        std::vector<int> vertMat = {5, -1, 100};
        vcl::vertexMaterialIndicesFromRange(mesh, vertMat);
        REQUIRE(mesh.isVertexMaterialIndexAvailable());
        REQUIRE(mesh.vertex(0).materialIndex() == 5);
        REQUIRE(mesh.vertex(1).materialIndex() == vcl::USHORT_NULL);
        REQUIRE(mesh.vertex(2).materialIndex() == 100);

        std::vector<int> faceMat = {-1};
        vcl::faceMaterialIndicesFromRange(mesh, faceMat);
        REQUIRE(mesh.isFaceMaterialIndexAvailable());
        REQUIRE(mesh.face(0).materialIndex() == vcl::USHORT_NULL);
    }

    SECTION("Import from floating point")
    {
        std::vector<double> vertMat = {12.0, -1.0, 99.0};
        vcl::vertexMaterialIndicesFromRange(mesh, vertMat);
        REQUIRE(mesh.vertex(0).materialIndex() == 12);
        REQUIRE(mesh.vertex(1).materialIndex() == vcl::USHORT_NULL);
        REQUIRE(mesh.vertex(2).materialIndex() == 99);

        std::vector<double> faceMat = {-1.0};
        vcl::faceMaterialIndicesFromRange(mesh, faceMat);
        REQUIRE(mesh.face(0).materialIndex() == vcl::USHORT_NULL);
    }
}
