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

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

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

std::istringstream plyTriCube()
{
    // string containing a triangulted cube in OBJ format

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

using Meshes  = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using MeshesIndexed =
    std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed, vcl::EdgeMeshIndexed>;
using MeshesIndexedf = std::
    tuple<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf, vcl::EdgeMeshIndexedf>;

// Test to load obj from a istringstream
TEMPLATE_TEST_CASE(
    "Load PLY cube from istringstream",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;
    using EdgeMesh = std::tuple_element_t<2, TestType>;

    SECTION("TriMesh - PolyCube")
    {
        TriMesh tm;
        auto    ss = plyPolyCube();
        vcl::loadPly(tm, ss);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("TriMesh - TriCube")
    {
        TriMesh tm;
        auto    ss = plyTriCube();
        vcl::loadPly(tm, ss);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    SECTION("TriMesh - VertTextureDouble")
    {
        TriMesh tm;
        vcl::loadPly(tm, VCLIB_EXAMPLE_MESHES_PATH "/VertTextureDouble.ply");
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 4);
        REQUIRE(tm.textureNumber() == 2);
        REQUIRE(tm.isPerVertexTexCoordEnabled());
        for(const auto& v : tm.vertices())
        {
            // first four vertices have index 0, the other four have index 1
            REQUIRE(v.texCoord().index() == v.index() / 4);
        }
    }

    SECTION("TriMesh - Wedge TextureDouble")
    {
        TriMesh tm;
        vcl::loadPly(tm, VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply");
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 4);
        REQUIRE(tm.textureNumber() == 2);
        REQUIRE(tm.isPerFaceWedgeTexCoordsEnabled());
        for(const auto& f : tm.faces())
        {
            // first two faces have texture index 0, the other two have index 1
            REQUIRE(f.textureIndex() == f.index() / 2);
        }
    }

    SECTION("PolyMesh - PolyCube")
    {
        PolyMesh pm;
        auto     ss = plyPolyCube();
        vcl::loadPly(pm, ss);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 6);
    }

    SECTION("PolyMesh - TriCube")
    {
        PolyMesh pm;
        auto     ss = plyTriCube();
        vcl::loadPly(pm, ss);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 12);
    }

    SECTION("EdgeMesh - PolyCube")
    {
        EdgeMesh em;
        auto     ss = plyPolyCube();
        vcl::loadPly(em, ss);
        REQUIRE(em.vertexNumber() == 8);
        REQUIRE(em.edgeNumber() == 4);
    }

    SECTION("EdgeMesh - TriCube")
    {
        EdgeMesh pm;
        auto     ss = plyTriCube();
        vcl::loadPly(pm, ss);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.edgeNumber() == 4);
    }
}
