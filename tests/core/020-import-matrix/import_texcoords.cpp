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

template<typename MeshType, typename TMatrix>
void testVertexTexCoords()
{
    // Create a mesh with vertices
    MeshType mesh;
    mesh.addVertices(4);
    mesh.deleteVertex(2); // keep 3 vertices

    // Test: Import texture coordinates from matrix and indices from range
    // Create texture coordinates matrix (3 vertices, 2 columns for UV)
    TMatrix texCoords(3, 2);
    texCoords << 0.0, 0.0, // vertex 0: (0,0)
        1.0, 0.0,          // vertex 1: (1,0)
        0.5, 1.0;          // vertex 3: (0.5,1)

    // Test vertexTexCoordsFromMatrix
    vcl::vertexTexCoordsFromMatrix(mesh, texCoords);

    // Create material indices vector
    std::vector<vcl::uint> materialIndices = {10, 20, 30};

    // Test vertexMaterialIndicesFromRange
    vcl::vertexMaterialIndicesFromRange(mesh, materialIndices);

    // Verify texture coordinate and material indices
    vcl::uint c = 0;
    REQUIRE(mesh.isVertexTexCoordAvailable());
    REQUIRE(mesh.isVertexMaterialIndexAvailable());
    REQUIRE(mesh.vertex(0).texCoord().u() == texCoords(c, 0));
    REQUIRE(mesh.vertex(0).texCoord().v() == texCoords(c, 1));
    REQUIRE(mesh.vertex(0).materialIndex() == materialIndices[c]);
    c++;
    REQUIRE(mesh.vertex(1).texCoord().u() == texCoords(c, 0));
    REQUIRE(mesh.vertex(1).texCoord().v() == texCoords(c, 1));
    REQUIRE(mesh.vertex(1).materialIndex() == materialIndices[c]);
    c++;
    REQUIRE(mesh.vertex(3).texCoord().u() == texCoords(c, 0));
    REQUIRE(mesh.vertex(3).texCoord().v() == texCoords(c, 1));
    REQUIRE(mesh.vertex(3).materialIndex() == materialIndices[c]);
}

template<typename MeshType, typename TMatrix>
void testFaceWedgeTexCoords()
{
    if constexpr (vcl::HasFaces<MeshType>) {
        // Create a mesh with vertices and triangular faces
        MeshType mesh;
        mesh.addVertices(4);
        mesh.addFace(0, 1, 2); // face 0
        mesh.addFace(0, 2, 3); // face 1

        // Create wedge texture coordinates matrix
        // For triangle meshes: 2 faces * 3 vertices per face * 2 components
        // (UV) = 2x6 matrix
        TMatrix wedgeTexCoords(2, 6);
        // Face 0: vertex 0 (0.0, 0.0), vertex 1 (1.0, 0.0), vertex 2 (0.5, 1.0)
        // Face 1: vertex 0 (0.2, 0.1), vertex 2 (0.7, 0.9), vertex 3 (0.9, 0.8)
        wedgeTexCoords << 0.0, 0.0, 1.0, 0.0, 0.5, 1.0, // face 0
            0.2, 0.1, 0.7, 0.9, 0.9, 0.8;               // face 1

        // Test faceWedgeTexCoordsFromMatrix
        vcl::faceWedgeTexCoordsFromMatrix(mesh, wedgeTexCoords);

        // Create face material indices
        // For triangle meshes: 2 faces = 2 indices
        std::vector<vcl::uint> faceMaterialIndices = {0, 1};

        // Test faceMaterialIndicesFromRange
        vcl::faceMaterialIndicesFromRange(mesh, faceMaterialIndices);

        // Verify wedge texture coordinates and face material indices
        REQUIRE(mesh.isFaceMaterialIndexAvailable());
        REQUIRE(mesh.isFaceWedgeTexCoordsAvailable());

        for (vcl::uint i = 0; const auto& f : mesh.faces()) {
            for (vcl::uint j = 0; const auto& w : f.wedges().texCoords()) {
                REQUIRE(w.u() == wedgeTexCoords(i, j++));
                REQUIRE(w.v() == wedgeTexCoords(i, j++));
            }
            REQUIRE(f.materialIndex() == faceMaterialIndices[i++]);
        }
    }
}

template<typename MeshType, typename TMatrix>
void testPolyFaceWedgeTexCoords()
{
    if constexpr (vcl::HasPolygons<MeshType>) {
        // Create a mesh with vertices and mixed faces
        MeshType mesh;
        mesh.addVertices(6);

        // Add a triangle face
        std::vector<vcl::uint> tri = {0, 1, 2};
        mesh.addFace(tri);

        // Add a quadrilateral face
        std::vector<vcl::uint> quad = {0, 2, 3, 4};
        mesh.addFace(quad);

        // Create wedge texture coordinates matrix
        // Triangle: 3 vertices * 2 coords = 6 values
        // Quad: 4 vertices * 2 coords = 8 values
        // Total: 8 columns
        // For polymesh: each row has variable number of coordinates
        TMatrix wedgeTexCoords(
            2, 8); // 2 faces, max 4 vertices per face * 2 = 8 cols
        wedgeTexCoords.setConstant(-1); // Initialize with invalid values

        // Face 0 (triangle): 3 vertices
        wedgeTexCoords(0, 0) = 0.0;
        wedgeTexCoords(0, 1) = 0.0; // vertex 0
        wedgeTexCoords(0, 2) = 1.0;
        wedgeTexCoords(0, 3) = 0.0; // vertex 1
        wedgeTexCoords(0, 4) = 0.5;
        wedgeTexCoords(0, 5) = 1.0; // vertex 2
        // Leave columns 6,7 as -1 (unused for triangle)

        // Face 1 (quad): 4 vertices
        wedgeTexCoords(1, 0) = 0.1;
        wedgeTexCoords(1, 1) = 0.1; // vertex 0
        wedgeTexCoords(1, 2) = 0.8;
        wedgeTexCoords(1, 3) = 0.2; // vertex 2
        wedgeTexCoords(1, 4) = 0.9;
        wedgeTexCoords(1, 5) = 0.9; // vertex 3
        wedgeTexCoords(1, 6) = 0.2;
        wedgeTexCoords(1, 7) = 0.8; // vertex 4

        // Test faceWedgeTexCoordsFromMatrix
        vcl::faceWedgeTexCoordsFromMatrix(mesh, wedgeTexCoords);

        // Create face material indices
        std::vector<vcl::uint> faceMaterialIndices = {0, 1};

        // Test faceMaterialIndicesFromRange
        vcl::faceMaterialIndicesFromRange(mesh, faceMaterialIndices);

        // Verify wedge texture coordinates and indices
        REQUIRE(mesh.isFaceMaterialIndexAvailable());
        REQUIRE(mesh.isFaceWedgeTexCoordsAvailable());

        for (vcl::uint i = 0; const auto& f : mesh.faces()) {
            for (vcl::uint j = 0; const auto& w : f.wedges().texCoords()) {
                REQUIRE(w.u() == wedgeTexCoords(i, j++));
                REQUIRE(w.v() == wedgeTexCoords(i, j++));
            }
            REQUIRE(f.materialIndex() == faceMaterialIndices[i++]);
        }
    }
}

TEMPLATE_TEST_CASE(
    "Import vertex texture coordinates and indices",
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
        using Matrix =
            Eigen::Matrix<Scalar, Eigen::Dynamic, 2, Eigen::RowMajor>;
        testVertexTexCoords<MeshType, Matrix>();
    }

    SECTION("Column major matrix")
    {
        using Matrix =
            Eigen::Matrix<Scalar, Eigen::Dynamic, 2, Eigen::ColMajor>;
        testVertexTexCoords<MeshType, Matrix>();
    }
}

TEMPLATE_TEST_CASE(
    "Import face wedge texture coordinates and indices",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    using Scalar   = vcl::ScalarType<MeshType>;

    SECTION("Row major matrix - triangular faces")
    {
        using Matrix = Eigen::
            Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
        testFaceWedgeTexCoords<MeshType, Matrix>();
    }

    SECTION("Column major matrix - triangular faces")
    {
        using Matrix = Eigen::
            Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;
        testFaceWedgeTexCoords<MeshType, Matrix>();
    }
}

TEMPLATE_TEST_CASE(
    "Import polygonal face wedge texture coordinates and indices",
    "",
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    using Scalar   = vcl::ScalarType<MeshType>;

    SECTION("Row major matrix - mixed face sizes")
    {
        using Matrix = Eigen::
            Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
        testPolyFaceWedgeTexCoords<MeshType, Matrix>();
    }

    SECTION("Column major matrix - mixed face sizes")
    {
        using Matrix = Eigen::
            Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;
        testPolyFaceWedgeTexCoords<MeshType, Matrix>();
    }
}
