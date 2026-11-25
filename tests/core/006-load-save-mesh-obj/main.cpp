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

using Meshes   = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf  = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using Meshes64 = std::tuple<vcl::TriMesh64, vcl::PolyMesh64, vcl::EdgeMesh64>;
using Meshes64f =
    std::tuple<vcl::TriMesh64f, vcl::PolyMesh64f, vcl::EdgeMesh64f>;

// Test to load obj from a istringstream
TEMPLATE_TEST_CASE(
    "Load OBJ cube from istringstream",
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
        REQUIRE(tm.isFaceMaterialIndexAvailable());
        REQUIRE(tm.isFaceWedgeTexCoordsAvailable());
        for (const auto& f : tm.faces()) {
            // first two faces have material index 0, the other two have index 1
            REQUIRE(f.materialIndex() == f.index() % 2);
        }

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(!info.hasEdges());

        REQUIRE(info.isTriangleMesh());

        REQUIRE(info.hasFaceWedgeTexCoords());
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

    SECTION("Face 0 without usemtl followed by usemtl")
    {
        info.clear();
        TriMesh     tm;
        std::string objStr =
            "v 0 0 0\nv 1 0 0\nv 0 1 0\n"
            "v 0 0 1\nv 1 0 1\nv 0 1 1\n"
            "f 1 2 3\n"
            "usemtl mat1\n"
            "f 4 5 6\n";
        std::string mtlStr =
            "newmtl mat1\n"
            "Kd 1.0 0.5 0.2\n";
        std::istringstream         objStream(objStr);
        std::istringstream         mtlStream(mtlStr);
        std::vector<std::istream*> mtlStreams = {&mtlStream};
        vcl::loadObj(tm, objStream, mtlStreams, info);
        REQUIRE(tm.vertexCount() == 6);
        REQUIRE(tm.faceCount() == 2);
        REQUIRE(tm.isFaceMaterialIndexAvailable());
        REQUIRE(tm.face(0).materialIndex() == vcl::USHORT_NULL);
        REQUIRE(tm.face(1).materialIndex() == 0);
    }
}

TEST_CASE("ObjMaterial serialization with map_Ke", "[io][obj]")
{
    vcl::detail::ObjMaterial mat;
    mat.matName = "test_material";
    mat.map_Ke  = "emissive.png";
    std::ostringstream ss;
    ss << mat;
    std::string s = ss.str();
    REQUIRE(s.find("map_Ke emissive.png") != std::string::npos);

    vcl::detail::ObjMaterial matEmpty;
    matEmpty.matName = "test_empty";
    std::ostringstream ssEmpty;
    ssEmpty << matEmpty;
    std::string sEmpty = ssEmpty.str();
    REQUIRE(sEmpty.find("map_Ke") == std::string::npos);
}
