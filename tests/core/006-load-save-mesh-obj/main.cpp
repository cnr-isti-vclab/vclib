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

#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

std::istringstream objPolyCube()
{
    // string containing a cube in OBJ format

    std::string s =
        "v -0.500000 -0.500000 0.500000\n"
        "v 0.500000 -0.500000 0.500000\n"
        "v -0.500000 0.500000 0.500000\n"
        "v 0.500000 0.500000 0.500000\n"
        "v -0.500000 0.500000 -0.500000\n"
        "v 0.500000 0.500000 -0.500000\n"
        "v -0.500000 -0.500000 -0.500000\n"
        "v 0.500000 -0.500000 -0.500000\n"
        "f 1 2 4 3\n"
        "f 3 4 6 5\n"
        "f 5 6 8 7\n"
        "f 7 8 2 1\n"
        "f 2 8 6 4\n"
        "f 7 1 3 5\n"
        "l 1 5\n"
        "l 2 6\n"
        "l 3 7\n"
        "l 4 8\n";

    std::istringstream ss(s);
    return ss;
}

std::istringstream objTriCube()
{
    // string containing a triangulted cube in OBJ format

    std::string s =
        "v -0.500000 -0.500000 0.500000\n"
        "v 0.500000 -0.500000 0.500000\n"
        "v -0.500000 0.500000 0.500000\n"
        "v 0.500000 0.500000 0.500000\n"
        "v -0.500000 0.500000 -0.500000\n"
        "v 0.500000 0.500000 -0.500000\n"
        "v -0.500000 -0.500000 -0.500000\n"
        "v 0.500000 -0.500000 -0.500000\n"
        "f 1 2 4\n"
        "f 1 4 3\n"
        "f 3 4 6\n"
        "f 3 6 5\n"
        "f 5 6 8\n"
        "f 5 8 7\n"
        "f 7 8 2\n"
        "f 7 2 1\n"
        "f 2 8 6\n"
        "f 2 6 4\n"
        "f 7 1 3\n"
        "f 7 3 5\n"
        "l 1 5\n"
        "l 2 6\n"
        "l 3 7\n"
        "l 4 8\n";

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
    "Load OBJ cube from istringstream",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;
    using EdgeMesh = std::tuple_element_t<2, TestType>;

    vcl::MeshInfo info;

    SECTION("TriMesh - PolyCube")
    {
        info.clear();

        TriMesh tm;
        auto    ss = objPolyCube();

        vcl::loadObj(tm, ss, {}, info);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(info.hasEdges());

        REQUIRE(info.isQuadMesh());
    }

    SECTION("TriMesh - TriCube")
    {
        info.clear();

        TriMesh tm;
        auto    ss = objTriCube();
        vcl::loadObj(tm, ss, {}, info);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(info.hasEdges());

        REQUIRE(info.isTriangleMesh());
    }

    SECTION("TriMesh - Wedge TextureDouble")
    {
        info.clear();

        TriMesh tm;
        vcl::loadObj(tm, VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.obj", info);
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 4);
        REQUIRE(tm.textureNumber() == 2);
        REQUIRE(tm.isPerFaceWedgeTexCoordsEnabled());
        for (const auto& f : tm.faces()) {
            // first two faces have texture index 0, the other two have index 1
            REQUIRE(f.textureIndex() == f.index() / 2);
        }

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(!info.hasEdges());

        REQUIRE(info.isTriangleMesh());

        REQUIRE(info.hasFaceWedgeTexCoords());
        REQUIRE(info.hasTextures());
    }

    SECTION("PolyMesh - PolyCube")
    {
        info.clear();

        PolyMesh pm;
        auto     ss = objPolyCube();
        vcl::loadObj(pm, ss, {}, info);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 6);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(info.hasEdges());

        REQUIRE(info.isQuadMesh());
    }

    SECTION("PolyMesh - TriCube")
    {
        info.clear();

        PolyMesh pm;
        auto     ss = objTriCube();
        vcl::loadObj(pm, ss, {}, info);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 12);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(info.hasEdges());

        REQUIRE(info.isTriangleMesh());
    }

    SECTION("TriMesh - Rhombicosidodecahedron")
    {
        info.clear();

        TriMesh pm;
        vcl::loadObj(
            pm, VCLIB_EXAMPLE_MESHES_PATH "/rhombicosidodecahedron.obj", info);
        REQUIRE(pm.vertexNumber() == 60);
        REQUIRE(pm.faceNumber() == 116);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(!info.hasEdges());

        REQUIRE(info.isPolygonMesh());
    }

    SECTION("PolyMesh - Rhombicosidodecahedron")
    {
        info.clear();

        PolyMesh pm;
        vcl::loadObj(
            pm, VCLIB_EXAMPLE_MESHES_PATH "/rhombicosidodecahedron.obj", info);
        REQUIRE(pm.vertexNumber() == 60);
        REQUIRE(pm.faceNumber() == 62);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(!info.hasEdges());

        REQUIRE(info.isPolygonMesh());
    }

    SECTION("EdgeMesh - PolyCube")
    {
        info.clear();

        EdgeMesh em;
        auto     ss = objPolyCube();
        vcl::loadObj(em, ss, {}, info);
        REQUIRE(em.vertexNumber() == 8);
        REQUIRE(em.edgeNumber() == 4);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(info.hasEdges());
    }

    SECTION("EdgeMesh - TriCube")
    {
        info.clear();

        EdgeMesh pm;
        auto     ss = objTriCube();
        vcl::loadObj(pm, ss, {}, info);
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.edgeNumber() == 4);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(info.hasEdges());
    }
}
