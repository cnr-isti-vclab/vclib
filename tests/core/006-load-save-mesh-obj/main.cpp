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

std::istringstream objPolyCube()
{
    // string containing a cube in OBJ format

    std::string s =
        "# a comment\n"
        "v -0.500000 -0.500000 0.500000\n"
        "v 0.500000 -0.500000 0.500000\n"
        "v -0.500000 0.500000 0.500000\n"
        "v 0.500000 0.500000 0.500000\n"
        "v -0.500000 0.500000 -0.500000\n"
        "v 0.500000 0.500000 -0.500000\n"
        "v -0.500000 -0.500000 -0.500000\n"
        "v 0.500000 -0.500000 -0.500000\n"
        "# another comment\n"
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
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);

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
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);

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
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 4);
        REQUIRE(tm.materialCount() == 2);
        REQUIRE(tm.isPerFaceMaterialIndexEnabled());
        REQUIRE(tm.isPerFaceWedgeTexCoordsEnabled());
        for (const auto& f : tm.faces()) {
            // first two faces have material index 0, the other two have index 1
            REQUIRE(f.materialIndex() == f.index() % 2);
        }

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(!info.hasEdges());

        REQUIRE(info.isTriangleMesh());

        REQUIRE(info.hasPerFaceWedgeTexCoords());
        REQUIRE(info.hasMaterials());
    }

    SECTION("PolyMesh - PolyCube")
    {
        info.clear();

        PolyMesh pm;
        auto     ss = objPolyCube();
        vcl::loadObj(pm, ss, {}, info);
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.faceCount() == 6);

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
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.faceCount() == 12);

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
        REQUIRE(pm.vertexCount() == 60);
        REQUIRE(pm.faceCount() == 116);

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
        REQUIRE(pm.vertexCount() == 60);
        REQUIRE(pm.faceCount() == 62);

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
        REQUIRE(em.vertexCount() == 8);
        REQUIRE(em.edgeCount() == 4);

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
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.edgeCount() == 4);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(info.hasEdges());
    }
}
