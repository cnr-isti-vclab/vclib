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

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<typename ScalarType>
using EigenRowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

template<typename ScalarType>
using Eigen3RowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 3, Eigen::RowMajor>;

template<typename ScalarType>
using Eigen4RowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 4, Eigen::RowMajor>;

template<typename ScalarType>
using EigenColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;

template<typename ScalarType>
using Eigen3ColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 3, Eigen::ColMajor>;

template<typename ScalarType>
using Eigen4ColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 4, Eigen::ColMajor>;

// test functions

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
    REQUIRE(mesh.vertexNumber() == 8);
    for (vcl::uint i = 0; i < 8; ++i) {
        const auto& v = mesh.vertex(i).position();
        REQUIRE(v.x() == vertices(i, 0));
        REQUIRE(v.y() == vertices(i, 1));
        REQUIRE(v.z() == vertices(i, 2));
    }

    // Verify faces
    if constexpr (vcl::HasFaces<MeshType>) {
        REQUIRE(mesh.faceNumber() == 12);
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
    REQUIRE(mesh.vertexNumber() == 3);
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

    REQUIRE(mesh.vertexNumber() == 4);
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
        REQUIRE(mesh.faceNumber() == 2);
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
        // Create a mesh with vertices
        MeshType mesh;
        mesh.addVertices(6);

        // Create face matrix with faces of different sizes
        // Face 0: triangle (0, 1, 2)
        // Face 1: quadrilateral (0, 2, 3, 4)
        // Face 2: pentagon (0, 4, 5, 1, 2)
        // Use UINT_NULL (-1) to indicate end of face for smaller faces
        FMatrix faces(3, 5);
        faces << 0, 1, 2, vcl::UINT_NULL, vcl::UINT_NULL, // triangle
            0, 2, 3, 4, vcl::UINT_NULL,                   // quadrilateral
            0, 4, 5, 1, 2;                                // pentagon

        // Test faceIndicesFromMatrix
        vcl::faceIndicesFromMatrix(mesh, faces);

        // Verify faces
        REQUIRE(mesh.faceNumber() == 3);

        // Verify faces using loops
        for (vcl::uint i = 0; i < 3; ++i) {
            const auto& f                   = mesh.face(i);
            vcl::uint   expectedVertexCount = 0;

            // Count non-null vertices in the row to get expected vertex count
            for (vcl::uint j = 0; j < faces.cols(); ++j) {
                if (faces(i, j) != vcl::UINT_NULL) {
                    expectedVertexCount++;
                }
            }

            REQUIRE(f.vertexNumber() == expectedVertexCount);

            // Check vertex indices (only non-null ones)
            for (vcl::uint j = 0; j < faces.cols(); ++j) {
                if (faces(i, j) != vcl::UINT_NULL) {
                    REQUIRE(f.vertexIndex(j) == faces(i, j));
                }
            }
        }
    }
}

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

template<typename MeshType, typename CMatrix>
void testVertexColorsFromMatrix()
{
    // Create a mesh with vertices
    MeshType mesh;
    mesh.addVertices(4);
    mesh.deleteVertex(2); // keep 3 vertices

    // Create colors matrix (3 columns - RGB)
    CMatrix colors(3, 3);
    colors << 255, 0, 0, // red
        0, 255, 0,       // green
        0, 0, 255;       // blue

    // Test elementColorsFromMatrix for vertices
    vcl::elementColorsFromMatrix<vcl::ElemId::VERTEX>(mesh, colors);

    // Verify colors
    REQUIRE(mesh.isPerVertexColorEnabled());
    REQUIRE(mesh.vertex(0).color().red() == 255);
    REQUIRE(mesh.vertex(0).color().green() == 0);
    REQUIRE(mesh.vertex(0).color().blue() == 0);
    REQUIRE(mesh.vertex(1).color().red() == 0);
    REQUIRE(mesh.vertex(1).color().green() == 255);
    REQUIRE(mesh.vertex(1).color().blue() == 0);
    REQUIRE(mesh.vertex(3).color().red() == 0);
    REQUIRE(mesh.vertex(3).color().green() == 0);
    REQUIRE(mesh.vertex(3).color().blue() == 255);
}

template<typename MeshType>
void testVertexColorsFromRange()
{
    // Create a mesh with vertices
    MeshType mesh;
    mesh.addVertices(4);
    mesh.deleteVertex(2); // keep 3 vertices

    // Create colors vector using packed 32-bit format
    // Red: 0xFF0000FF (ABGR format: A=255, B=0, G=0, R=255)
    // Green: 0xFF00FF00 (ABGR format: A=255, B=0, G=255, R=0)
    // Blue: 0xFFFF0000 (ABGR format: A=255, B=255, G=0, R=0)
    std::vector<uint32_t> colors = {0xFF0000FF, 0xFF00FF00, 0xFFFF0000};

    // Test vertexColorsFromRange with ABGR format
    vcl::vertexColorsFromRange(mesh, colors, vcl::Color::Format::ABGR);

    // Verify colors
    vcl::uint c = 0;
    REQUIRE(mesh.isPerVertexColorEnabled());
    REQUIRE(mesh.vertex(0).color().red() == 255);
    REQUIRE(mesh.vertex(0).color().green() == 0);
    REQUIRE(mesh.vertex(0).color().blue() == 0);
    REQUIRE(mesh.vertex(0).color().alpha() == 255);
    REQUIRE(mesh.vertex(1).color().red() == 0);
    REQUIRE(mesh.vertex(1).color().green() == 255);
    REQUIRE(mesh.vertex(1).color().blue() == 0);
    REQUIRE(mesh.vertex(1).color().alpha() == 255);
    REQUIRE(mesh.vertex(3).color().red() == 0);
    REQUIRE(mesh.vertex(3).color().green() == 0);
    REQUIRE(mesh.vertex(3).color().blue() == 255);
    REQUIRE(mesh.vertex(3).color().alpha() == 255);

    // Test with different color format (RGBA)
    // Red: 0xFF0000FF (RGBA format: R=255, G=0, B=0, A=255)
    // Green: 0x00FF00FF (RGBA format: R=0, G=255, B=0, A=255)
    // Blue: 0x0000FFFF (RGBA format: R=0, G=0, B=255, A=255)
    std::vector<uint32_t> rgbaColors = {0xFF0000FF, 0x00FF00FF, 0x0000FFFF};
    vcl::vertexColorsFromRange(mesh, rgbaColors, vcl::Color::Format::RGBA);

    // Verify colors with RGBA format
    REQUIRE(mesh.vertex(0).color().red() == 255);
    REQUIRE(mesh.vertex(0).color().green() == 0);
    REQUIRE(mesh.vertex(0).color().blue() == 0);
    REQUIRE(mesh.vertex(0).color().alpha() == 255);
    REQUIRE(mesh.vertex(1).color().red() == 0);
    REQUIRE(mesh.vertex(1).color().green() == 255);
    REQUIRE(mesh.vertex(1).color().blue() == 0);
    REQUIRE(mesh.vertex(1).color().alpha() == 255);
    REQUIRE(mesh.vertex(3).color().red() == 0);
    REQUIRE(mesh.vertex(3).color().green() == 0);
    REQUIRE(mesh.vertex(3).color().blue() == 255);
    REQUIRE(mesh.vertex(3).color().alpha() == 255);
}

template<typename MeshType>
void testVertexQualityFromRange()
{
    using QualityType = MeshType::VertexType::QualityType;

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
    REQUIRE(mesh.isPerVertexQualityEnabled());
    REQUIRE(mesh.vertex(0).quality() == quality[c++]);
    REQUIRE(mesh.vertex(1).quality() == quality[c++]);
    REQUIRE(mesh.vertex(3).quality() == quality[c++]);
}

template<typename MeshType>
void testFaceQualityFromRange()
{
    if constexpr (vcl::HasFaces<MeshType>) {
        using QualityType = MeshType::FaceType::QualityType;

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
        REQUIRE(mesh.isPerFaceQualityEnabled());
        REQUIRE(mesh.face(0).quality() == quality[c++]);
        REQUIRE(mesh.face(1).quality() == quality[c++]);
    }
}

template<typename MeshType>
void testFaceColorsFromRange()
{
    if constexpr (vcl::HasFaces<MeshType>) {
        // Create a mesh with vertices and faces
        MeshType mesh;
        mesh.addVertices(4);
        mesh.addFace(0, 1, 2);
        mesh.addFace(0, 2, 3);

        // Create colors vector using packed 32-bit format
        // Red: 0xFF0000FF (ABGR format: A=255, B=0, G=0, R=255)
        // Green: 0xFF00FF00 (ABGR format: A=255, B=0, G=255, R=0)
        std::vector<uint32_t> colors = {0xFF0000FF, 0xFF00FF00};

        // Test faceColorsFromRange with ABGR format
        vcl::faceColorsFromRange(mesh, colors, vcl::Color::Format::ABGR);

        // Verify colors
        REQUIRE(mesh.isPerFaceColorEnabled());
        REQUIRE(mesh.face(0).color().red() == 255);
        REQUIRE(mesh.face(0).color().green() == 0);
        REQUIRE(mesh.face(0).color().blue() == 0);
        REQUIRE(mesh.face(0).color().alpha() == 255);
        REQUIRE(mesh.face(1).color().red() == 0);
        REQUIRE(mesh.face(1).color().green() == 255);
        REQUIRE(mesh.face(1).color().blue() == 0);
        REQUIRE(mesh.face(1).color().alpha() == 255);

        // Test with different color format (RGBA)
        // Red: 0xFF0000FF (RGBA format: R=255, G=0, B=0, A=255)
        // Green: 0x00FF00FF (RGBA format: R=0, G=255, B=0, A=255)
        std::vector<uint32_t> rgbaColors = {0xFF0000FF, 0x00FF00FF};
        vcl::faceColorsFromRange(mesh, rgbaColors, vcl::Color::Format::RGBA);

        // Verify colors with RGBA format
        REQUIRE(mesh.face(0).color().red() == 255);
        REQUIRE(mesh.face(0).color().green() == 0);
        REQUIRE(mesh.face(0).color().blue() == 0);
        REQUIRE(mesh.face(0).color().alpha() == 255);
        REQUIRE(mesh.face(1).color().red() == 0);
        REQUIRE(mesh.face(1).color().green() == 255);
        REQUIRE(mesh.face(1).color().blue() == 0);
        REQUIRE(mesh.face(1).color().alpha() == 255);
    }
}

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
    REQUIRE(mesh.isPerVertexTexCoordEnabled());
    REQUIRE(mesh.isPerVertexMaterialIndexEnabled());
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
        REQUIRE(mesh.isPerFaceMaterialIndexEnabled());
        REQUIRE(mesh.isPerFaceWedgeTexCoordsEnabled());

        for (vcl::uint i = 0; const auto& f : mesh.faces()) {
            for (vcl::uint j = 0; const auto& w : f.wedgeTexCoords()) {
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
        REQUIRE(mesh.isPerFaceMaterialIndexEnabled());
        REQUIRE(mesh.isPerFaceWedgeTexCoordsEnabled());

        for (vcl::uint i = 0; const auto& f : mesh.faces()) {
            for (vcl::uint j = 0; const auto& w : f.wedgeTexCoords()) {
                REQUIRE(w.u() == wedgeTexCoords(i, j++));
                REQUIRE(w.v() == wedgeTexCoords(i, j++));
            }
            REQUIRE(f.materialIndex() == faceMaterialIndices[i++]);
        }
    }
}

// Test cases

TEMPLATE_TEST_CASE("Import mesh from matrices", "", vcl::TriMeshf, vcl::TriMesh)
{
    using MeshType = TestType;
    using Scalar   = MeshType::VertexType::PositionType::ScalarType;

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
    using Scalar   = MeshType::VertexType::PositionType::ScalarType;

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
    using Scalar   = MeshType::VertexType::PositionType::ScalarType;

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
    "Import vertex colors from matrix",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;

    SECTION("Row major matrix - int colors")
    {
        testVertexColorsFromMatrix<MeshType, Eigen3RowMatrix<int>>();
    }

    SECTION("Column major matrix - int colors")
    {
        testVertexColorsFromMatrix<MeshType, Eigen3ColMatrix<int>>();
    }
}

TEMPLATE_TEST_CASE(
    "Import vertex colors from range",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    testVertexColorsFromRange<MeshType>();
}

TEMPLATE_TEST_CASE(
    "Import face colors from range",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    testFaceColorsFromRange<MeshType>();
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

TEMPLATE_TEST_CASE(
    "Import vertex texture coordinates and indices",
    "",
    vcl::TriMeshf,
    vcl::TriMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh)
{
    using MeshType = TestType;
    using Scalar   = MeshType::VertexType::PositionType::ScalarType;

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
    using Scalar   = MeshType::VertexType::PositionType::ScalarType;

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
    using Scalar   = MeshType::VertexType::PositionType::ScalarType;

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
            vcl::faceMaterialIndicesFromRange(
                mesh, wrongFaceMaterialIndices),
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
