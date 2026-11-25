// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "helpers.h"

#include <vclib/algorithms/mesh/import_export/import_matrix.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <vector>

template<typename MeshType, typename VMatrix, typename FMatrix>
void testMeshFromMatrices()
{
    // Create vertex matrix with a simple cube vertices
    VMatrix vertices(8, 3);
    vertices << 0.0, 0.0, 0.0, // vertex 0
        1.0, 0.0, 0.0,         // vertex 1
        1.0, 1.0, 0.0,         // vertex 2
        0.0, 1.0, 0.0,         // vertex 3
        0.0, 0.0, 1.0,         // vertex 4
        1.0, 0.0, 1.0,         // vertex 5
        1.0, 1.0, 1.0,         // vertex 6
        0.0, 1.0, 1.0;         // vertex 7

    // Create face matrix with 12 triangular faces of the cube
    FMatrix faces(12, 3);
    faces << 0, 1, 2, // bottom face - triangle 1
        0, 2, 3,      // bottom face - triangle 2
        4, 7, 6,      // top face - triangle 1
        4, 6, 5,      // top face - triangle 2
        0, 4, 5,      // front face - triangle 1
        0, 5, 1,      // front face - triangle 2
        2, 6, 7,      // back face - triangle 1
        2, 7, 3,      // back face - triangle 2
        0, 3, 7,      // left face - triangle 1
        0, 7, 4,      // left face - triangle 2
        1, 5, 6,      // right face - triangle 1
        1, 6, 2;      // right face - triangle 2

    // Test meshFromMatrices function
    MeshType mesh = vcl::meshFromMatrices<MeshType>(vertices, faces);

    // Verify vertices
    REQUIRE(mesh.vertexCount() == 8);
    for (vcl::uint i = 0; i < 8; ++i) {
        const auto& v = mesh.vertex(i).position();
        REQUIRE(v.x() == vertices(i, 0));
        REQUIRE(v.y() == vertices(i, 1));
        REQUIRE(v.z() == vertices(i, 2));
    }

    // Verify faces
    if constexpr (vcl::HasFaces<MeshType>) {
        REQUIRE(mesh.faceCount() == 12);
        for (vcl::uint i = 0; i < 12; ++i) {
            const auto& f = mesh.face(i);
            for (vcl::uint j = 0; j < 3; ++j) {
                REQUIRE(f.vertexIndex(j) == faces(i, j));
            }
        }
    }
}

template<typename MeshType, typename VMatrix>
void testVertexPositionsFromMatrix()
{
    // Create a triangle mesh
    MeshType mesh;
    mesh.addVertices(3);
    if constexpr (vcl::HasFaces<MeshType>) {
        mesh.addFace(0, 1, 2);
    }

    // Create vertex position matrix
    VMatrix vertices(3, 3);
    vertices << 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.5, 1.0, 0.0;

    // Test vertexPositionsFromMatrix with clearBeforeSet = false
    vcl::vertexPositionsFromMatrix(mesh, vertices, false);

    // Verify vertices
    REQUIRE(mesh.vertexCount() == 3);
    for (vcl::uint i = 0; i < 3; ++i) {
        const auto& v = mesh.vertex(i).position();
        REQUIRE(v.x() == vertices(i, 0));
        REQUIRE(v.y() == vertices(i, 1));
        REQUIRE(v.z() == vertices(i, 2));
    }

    // Test with clearBeforeSet = true
    VMatrix newVertices(4, 3);
    newVertices << 2.0, 0.0, 0.0, 3.0, 0.0, 0.0, 2.5, 1.0, 0.0, 2.5, 0.5, 1.0;

    vcl::vertexPositionsFromMatrix(mesh, newVertices, true);

    REQUIRE(mesh.vertexCount() == 4);
    for (vcl::uint i = 0; i < 4; ++i) {
        const auto& v = mesh.vertex(i).position();
        REQUIRE(v.x() == newVertices(i, 0));
        REQUIRE(v.y() == newVertices(i, 1));
        REQUIRE(v.z() == newVertices(i, 2));
    }
}

template<typename MeshType, typename FMatrix>
void testFaceIndicesFromMatrix()
{
    if constexpr (vcl::HasFaces<MeshType>) {
        // Create a mesh with vertices
        MeshType mesh;
        mesh.addVertices(4);

        // Create face matrix
        FMatrix faces(2, 3);
        faces << 0, 1, 2, 0, 2, 3;

        // Test faceIndicesFromMatrix
        vcl::faceIndicesFromMatrix(mesh, faces);

        // Verify faces
        REQUIRE(mesh.faceCount() == 2);
        for (vcl::uint i = 0; i < 2; ++i) {
            const auto& f = mesh.face(i);
            for (vcl::uint j = 0; j < 3; ++j) {
                REQUIRE(f.vertexIndex(j) == faces(i, j));
            }
        }
    }
}

template<typename MeshType, typename FMatrix>
void testPolyFaceIndicesFromMatrix()
{
    if constexpr (vcl::HasFaces<MeshType> && vcl::HasPolygons<MeshType>) {
        using Scalar            = typename FMatrix::Scalar;
        constexpr auto SENTINEL = static_cast<Scalar>(-1);

        // Create a mesh with vertices
        MeshType mesh;
        mesh.addVertices(6);

        // Create face matrix with faces of different sizes
        // Face 0: triangle (0, 1, 2)
        // Face 1: quadrilateral (0, 2, 3, 4)
        // Face 2: pentagon (0, 4, 5, 1, 2)
        // Use sentinel (-1) to indicate end of face for smaller faces
        FMatrix faces(3, 5);
        faces << 0, 1, 2, SENTINEL, SENTINEL, // triangle
            0, 2, 3, 4, SENTINEL,             // quadrilateral
            0, 4, 5, 1, 2;                    // pentagon

        // Test faceIndicesFromMatrix
        vcl::faceIndicesFromMatrix(mesh, faces);

        // Verify faces
        REQUIRE(mesh.faceCount() == 3);

        // Verify faces using loops
        for (vcl::uint i = 0; i < 3; ++i) {
            const auto& f                   = mesh.face(i);
            vcl::uint   expectedVertexCount = 0;

            // Count non-null vertices in the row to get expected vertex count
            for (vcl::uint j = 0; j < faces.cols(); ++j) {
                if (faces(i, j) != SENTINEL) {
                    expectedVertexCount++;
                }
            }

            REQUIRE(f.vertexCount() == expectedVertexCount);

            // Check vertex indices (only non-null ones)
            for (vcl::uint j = 0; j < faces.cols(); ++j) {
                if (faces(i, j) != SENTINEL) {
                    REQUIRE(f.vertexIndex(j) == faces(i, j));
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE("Import mesh from matrices", "", vcl::TriMeshf, vcl::TriMesh)
{
    using MeshType = TestType;
    using Scalar   = vcl::ScalarType<MeshType>;

    SECTION("Row major matrices")
    {
        testMeshFromMatrices<
            MeshType,
            Eigen3RowMatrix<Scalar>,
            EigenRowMatrix<int>>();
    }

    SECTION("Column major matrices")
    {
        testMeshFromMatrices<
            MeshType,
            Eigen3ColMatrix<Scalar>,
            EigenColMatrix<int>>();
    }
}

TEMPLATE_TEST_CASE(
    "Import vertex positions from matrix",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    using Scalar   = vcl::ScalarType<MeshType>;

    SECTION("Row major matrix")
    {
        testVertexPositionsFromMatrix<MeshType, Eigen3RowMatrix<Scalar>>();
    }

    SECTION("Column major matrix")
    {
        testVertexPositionsFromMatrix<MeshType, Eigen3ColMatrix<Scalar>>();
    }
}

TEMPLATE_TEST_CASE(
    "Import face indices from matrix",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;

    SECTION("Row major matrix")
    {
        testFaceIndicesFromMatrix<MeshType, EigenRowMatrix<int>>();
    }

    SECTION("Column major matrix")
    {
        testFaceIndicesFromMatrix<MeshType, EigenColMatrix<int>>();
    }
}

TEMPLATE_TEST_CASE(
    "Import polygonal face indices from matrix",
    "",
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;

    SECTION("Row major matrix - mixed face sizes")
    {
        testPolyFaceIndicesFromMatrix<MeshType, EigenRowMatrix<int>>();
    }

    SECTION("Column major matrix - mixed face sizes")
    {
        testPolyFaceIndicesFromMatrix<MeshType, EigenColMatrix<int>>();
    }
}
