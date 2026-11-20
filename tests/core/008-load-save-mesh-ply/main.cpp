// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

std::istringstream plyPolyCube()
{
    // string containing a cube in PLY format

    std::string s =
        "ply\n"
        "format ascii 1.0\n"
        "comment Created by VCLib\n"
        "element vertex 8\n"
        "property float x\n"
        "property float y\n"
        "property float z\n"
        "element face 6\n"
        "property list uchar int vertex_indices\n"
        "element edge 4\n"
        "property int vertex1\n"
        "property int vertex2\n"
        "end_header\n"
        "-0.500000 -0.500000 0.500000\n"
        "0.500000 -0.500000 0.500000\n"
        "-0.500000 0.500000 0.500000\n"
        "0.500000 0.500000 0.500000\n"
        "-0.500000 0.500000 -0.500000\n"
        "0.500000 0.500000 -0.500000\n"
        "-0.500000 -0.500000 -0.500000\n"
        "0.500000 -0.500000 -0.500000\n"
        "4 0 1 3 2\n"
        "4 2 3 5 4\n"
        "4 4 5 7 6\n"
        "4 6 7 1 0\n"
        "4 1 7 5 3\n"
        "4 6 0 2 4\n"
        "0 4\n"
        "1 5\n"
        "2 6\n"
        "3 7\n";

    std::istringstream ss(s);
    return ss;
}

std::istringstream plyPolyWithWedgeTexCoords()
{
    std::string s =
        "ply\n"
        "format ascii 1.0\n"
        "element vertex 4\n"
        "property float x\n"
        "property float y\n"
        "property float z\n"
        "element face 1\n"
        "property list uchar int vertex_indices\n"
        "property list uchar float texcoord\n"
        "end_header\n"
        "0.0 0.0 0.0\n"
        "1.0 0.0 0.0\n"
        "1.0 1.0 0.0\n"
        "0.0 1.0 0.0\n"
        "4 0 1 2 3 8 0.0 0.0 1.0 0.0 1.0 1.0 0.0 1.0\n";
    return std::istringstream(s);
}

std::istringstream plyEdgeWithProperties()
{
    std::string s =
        "ply\n"
        "format ascii 1.0\n"
        "element vertex 2\n"
        "property float x\n"
        "property float y\n"
        "property float z\n"
        "element edge 1\n"
        "property int vertex1\n"
        "property int vertex2\n"
        "property float nx\n"
        "property float ny\n"
        "property float nz\n"
        "property uchar red\n"
        "property uchar green\n"
        "property uchar blue\n"
        "property float quality\n"
        "end_header\n"
        "0.0 0.0 0.0\n"
        "1.0 0.0 0.0\n"
        "0 1 0.0 1.0 0.0 255 128 64 42.5\n";
    return std::istringstream(s);
}

std::istringstream plyTriCube()
{
    // string containing a triangulted cube in PLY format

    std::string s =
        "ply\n"
        "format ascii 1.0\n"
        "comment Created by VCLib\n"
        "element vertex 8\n"
        "property float x\n"
        "property float y\n"
        "property float z\n"
        "element face 12\n"
        "property list uchar int vertex_indices\n"
        "element edge 4\n"
        "property int vertex1\n"
        "property int vertex2\n"
        "end_header\n"
        "-0.500000 -0.500000 0.500000\n"
        "0.500000 -0.500000 0.500000\n"
        "-0.500000 0.500000 0.500000\n"
        "0.500000 0.500000 0.500000\n"
        "-0.500000 0.500000 -0.500000\n"
        "0.500000 0.500000 -0.500000\n"
        "-0.500000 -0.500000 -0.500000\n"
        "0.500000 -0.500000 -0.500000\n"
        "3 0 1 2\n"
        "3 1 3 2\n"
        "3 2 3 4\n"
        "3 3 5 4\n"
        "3 4 5 6\n"
        "3 5 7 6\n"
        "3 6 7 0\n"
        "3 7 1 0\n"
        "3 1 7 3\n"
        "3 7 5 3\n"
        "3 5 7 6\n"
        "3 7 5 4\n"
        "0 4\n"
        "1 5\n"
        "2 6\n"
        "3 7\n";

    std::istringstream ss(s);
    return ss;
}

using Meshes   = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf  = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using Meshes64 = std::tuple<vcl::TriMesh64, vcl::PolyMesh64, vcl::EdgeMesh64>;
using Meshes64f =
    std::tuple<vcl::TriMesh64f, vcl::PolyMesh64f, vcl::EdgeMesh64f>;

// Test to load ply from a istringstream
TEMPLATE_TEST_CASE(
    "Load PLY cube from istringstream",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;
    using EdgeMesh = std::tuple_element_t<2, TestType>;

    vcl::MeshInfo info;

    SECTION("TriMesh - PolyCube")
    {
        TriMesh tm;
        auto    ss = plyPolyCube();
        vcl::loadPly(tm, ss, info);
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);
    }

    SECTION("TriMesh - TriCube")
    {
        TriMesh tm;
        auto    ss = plyTriCube();
        vcl::loadPly(tm, ss, info);
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);
    }

    SECTION("TriMesh - VertTextureDouble")
    {
        TriMesh tm;
        vcl::loadPly(
            tm, VCLIB_EXAMPLE_MESHES_PATH "/VertTextureDouble.ply", info);
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 4);
        REQUIRE(tm.materialCount() == 2);
        REQUIRE(tm.isVertexTexCoordAvailable());
        REQUIRE(tm.isVertexMaterialIndexAvailable());
        for (const auto& v : tm.vertices()) {
            // first four vertices have index 0, the other four have index 1
            REQUIRE(v.materialIndex() == v.index() / 4);
        }
    }

    SECTION("TriMesh - Wedge TextureDouble")
    {
        TriMesh tm;
        vcl::loadPly(tm, VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply", info);
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 4);
        REQUIRE(tm.materialCount() == 2);
        REQUIRE(tm.isFaceMaterialIndexAvailable());
        REQUIRE(tm.isFaceWedgeTexCoordsAvailable());
        for (const auto& f : tm.faces()) {
            // first two faces have material index 0, the other two have index 1
            REQUIRE(f.materialIndex() == f.index() / 2);
        }
    }

    SECTION("PolyMesh - PolyCube")
    {
        PolyMesh pm;
        auto     ss = plyPolyCube();
        vcl::loadPly(pm, ss, info);
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.faceCount() == 6);
    }

    SECTION("PolyMesh - TriCube")
    {
        PolyMesh pm;
        auto     ss = plyTriCube();
        vcl::loadPly(pm, ss, info);
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.faceCount() == 12);
    }

    SECTION("EdgeMesh - PolyCube")
    {
        EdgeMesh em;
        auto     ss = plyPolyCube();
        vcl::loadPly(em, ss, info);
        REQUIRE(em.vertexCount() == 8);
        REQUIRE(em.edgeCount() == 4);
    }

    SECTION("EdgeMesh - TriCube")
    {
        EdgeMesh pm;
        auto     ss = plyTriCube();
        vcl::loadPly(pm, ss, info);
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.edgeCount() == 4);
    }

    SECTION("TriMesh - PolyWithWedgeTexCoords (split face with wedge UV)")
    {
        TriMesh tm;
        auto    ss = plyPolyWithWedgeTexCoords();
        vcl::loadPly(tm, ss, info);
        REQUIRE(tm.vertexCount() == 4);
        REQUIRE(tm.faceCount() == 2);
        REQUIRE(tm.isFaceWedgeTexCoordsAvailable());
        // Verify texcoords for both generated triangles match polygon vertices
        for (vcl::uint ff = 0; ff < tm.faceCount(); ++ff) {
            for (vcl::uint i = 0; i < 3; ++i) {
                vcl::uint vid       = tm.face(ff).vertexIndex(i);
                float     expectedU = (vid == 1 || vid == 2) ? 1.0f : 0.0f;
                float     expectedV = (vid >= 2) ? 1.0f : 0.0f;
                REQUIRE(tm.face(ff).wedge(i).texCoord().u() == expectedU);
                REQUIRE(tm.face(ff).wedge(i).texCoord().v() == expectedV);
            }
        }
    }

    SECTION("EdgeMesh - Edge with Normal, Color, Quality")
    {
        EdgeMesh em;
        auto     ss = plyEdgeWithProperties();
        vcl::loadPly(em, ss, info);
        REQUIRE(em.vertexCount() == 2);
        REQUIRE(em.edgeCount() == 1);
        REQUIRE(em.isEdgeNormalAvailable());
        REQUIRE(em.isEdgeColorAvailable());
        REQUIRE(em.isEdgeQualityAvailable());
        REQUIRE(em.edge(0).normal().y() == 1.0);
        REQUIRE(em.edge(0).color().red() == 255);
        REQUIRE(em.edge(0).color().green() == 128);
        REQUIRE(em.edge(0).color().blue() == 64);
        REQUIRE(em.edge(0).quality() == 42.5);
    }
}
