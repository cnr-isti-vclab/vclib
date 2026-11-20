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

#include <cstdint>
#include <vector>

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
    REQUIRE(mesh.isVertexColorAvailable());
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
    REQUIRE(mesh.isVertexColorAvailable());
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
        REQUIRE(mesh.isFaceColorAvailable());
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
