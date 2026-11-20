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

template<typename MeshType>
void testVertexSelectionFromRange()
{
    // Create a mesh with vertices
    MeshType mesh;
    mesh.addVertices(4);

    // Create selection vector
    std::vector<bool> selection = {true, false, true, false};

    // Test vertexSelectionFromRange
    vcl::vertexSelectionFromRange(mesh, selection);

    // Verify selection
    REQUIRE(mesh.vertex(0).selected() == true);
    REQUIRE(mesh.vertex(1).selected() == false);
    REQUIRE(mesh.vertex(2).selected() == true);
    REQUIRE(mesh.vertex(3).selected() == false);
}

template<typename MeshType>
void testFaceSelectionFromRange()
{
    if constexpr (vcl::HasFaces<MeshType>) {
        // Create a mesh with vertices and faces
        MeshType mesh;
        mesh.addVertices(4);
        mesh.addFace(0, 1, 2);
        mesh.addFace(0, 2, 3);

        // Create selection vector
        std::vector<bool> selection = {true, false};

        // Test faceSelectionFromRange
        vcl::faceSelectionFromRange(mesh, selection);

        // Verify selection
        REQUIRE(mesh.face(0).selected() == true);
        REQUIRE(mesh.face(1).selected() == false);
    }
}

template<typename MeshType, typename NMatrix>
void testVertexNormalsFromMatrix()
{
    // Create a mesh with vertices
    MeshType mesh;
    mesh.addVertices(3);

    // Create normals matrix
    NMatrix normals(3, 3);
    normals << 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0;

    // Test vertexNormalsFromMatrix
    vcl::vertexNormalsFromMatrix(mesh, normals);

    // Verify normals
    for (vcl::uint i = 0; i < 3; ++i) {
        const auto& n = mesh.vertex(i).normal();
        REQUIRE(n.x() == normals(i, 0));
        REQUIRE(n.y() == normals(i, 1));
        REQUIRE(n.z() == normals(i, 2));
    }
}

template<typename MeshType>
void testVertexQualityFromRange()
{
    using QualityType = vcl::VertexType<MeshType>::QualityType;

    // Create a mesh with vertices
    MeshType mesh;
    mesh.addVertices(4);
    mesh.deleteVertex(2); // keep 3 vertices

    // Create quality vector
    std::vector<QualityType> quality = {1.5, 2.7, 3.8};

    // Test vertexQualityFromRange
    vcl::vertexQualityFromRange(mesh, quality);

    // Verify quality values
    vcl::uint c = 0;
    REQUIRE(mesh.isVertexQualityAvailable());
    REQUIRE(mesh.vertex(0).quality() == quality[c++]);
    REQUIRE(mesh.vertex(1).quality() == quality[c++]);
    REQUIRE(mesh.vertex(3).quality() == quality[c++]);
}

template<typename MeshType>
void testFaceQualityFromRange()
{
    if constexpr (vcl::HasFaces<MeshType>) {
        using QualityType = vcl::FaceType<MeshType>::QualityType;

        // Create a mesh with vertices and faces
        MeshType mesh;
        mesh.addVertices(4);
        mesh.addFace(0, 1, 2);
        mesh.addFace(0, 2, 3);

        // Create quality vector
        std::vector<QualityType> quality = {0.5, 1.2};

        // Test faceQualityFromRange
        vcl::faceQualityFromRange(mesh, quality);

        // Verify quality values
        vcl::uint c = 0;
        REQUIRE(mesh.isFaceQualityAvailable());
        REQUIRE(mesh.face(0).quality() == quality[c++]);
        REQUIRE(mesh.face(1).quality() == quality[c++]);
    }
}

TEMPLATE_TEST_CASE(
    "Import vertex selection from range",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    testVertexSelectionFromRange<MeshType>();
}

TEMPLATE_TEST_CASE(
    "Import face selection from range",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    testFaceSelectionFromRange<MeshType>();
}

TEMPLATE_TEST_CASE(
    "Import vertex normals from matrix",
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
        testVertexNormalsFromMatrix<MeshType, Eigen3RowMatrix<Scalar>>();
    }

    SECTION("Column major matrix")
    {
        testVertexNormalsFromMatrix<MeshType, Eigen3ColMatrix<Scalar>>();
    }
}

TEMPLATE_TEST_CASE(
    "Import vertex quality from range",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    testVertexQualityFromRange<MeshType>();
}

TEMPLATE_TEST_CASE(
    "Import face quality from range",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    testFaceQualityFromRange<MeshType>();
}
