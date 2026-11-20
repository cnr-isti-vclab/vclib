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

using Meshes    = std::pair<vcl::TriMesh, vcl::PolyMesh>;
using Meshesf   = std::pair<vcl::TriMeshf, vcl::PolyMeshf>;
using Meshes64  = std::pair<vcl::TriMesh64, vcl::PolyMesh64>;
using Meshes64f = std::pair<vcl::TriMesh64f, vcl::PolyMesh64f>;

TEMPLATE_TEST_CASE(
    "Mesh Conversions -The TextureDouble mesh loaded on TriMesh",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh  = typename TestType::first_type;
    using PolyMesh = typename TestType::second_type;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply");

    tm.template addCustomComponent<int>("cust_comp", 4);
    tm.template addVertexCustomComponent<float>("v_comp");
    tm.deleteFace(3);

    for (auto&& v : tm.vertices()) {
        v.template customComponent<float>("v_comp") = 3.14f;
    }

    THEN("The mesh has 8 vertices, 4 triangles, per face wedge texcoords")
    {
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 3);
        REQUIRE(tm.isFaceWedgeTexCoordsAvailable());
        REQUIRE(tm.hasCustomComponent("cust_comp"));
        REQUIRE(tm.template isCustomComponentOfType<int>("cust_comp"));
        REQUIRE(
            !tm.template isCustomComponentOfType<unsigned int>("cust_comp"));
        REQUIRE(tm.template customComponent<int>("cust_comp") == 4);
        REQUIRE(tm.hasVertexCustomComponent("v_comp"));

        for (const auto& v : tm.vertices()) {
            REQUIRE(v.template customComponent<float>("v_comp") == 3.14f);
        }
    }

    PolyMesh pmNoOpt;
    REQUIRE(!pmNoOpt.isFaceWedgeTexCoordsAvailable());
    pmNoOpt.importFrom(tm);
    REQUIRE(!pmNoOpt.isFaceWedgeTexCoordsAvailable());

    PolyMesh pm;
    REQUIRE(!pm.isFaceWedgeTexCoordsAvailable());
    pm.enableSameOptionalComponentsOf(tm);
    REQUIRE(pm.isFaceWedgeTexCoordsAvailable());

    pm.importFrom(tm);

    THEN("The imported PolyMesh has same number of vertices and faces")
    {
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.faceCount() == 3);
    }

    THEN("The imported vetices have same positions")
    {
        for (const auto& tv : tm.vertices()) {
            REQUIRE(pm.vertex(tv.index()).position() == tv.position());
        }
    }

    THEN("The imported faces have same vertices")
    {
        for (const auto& pf : pm.faces()) {
            REQUIRE(pf.vertexCount() == 3);
            const auto& tf = tm.face(pf.index());

            unsigned int i = 0;
            for (auto pv : pf.vertices()) {
                REQUIRE(pv.index() == tf.vertexIndex(i));
                REQUIRE(pv.position() == tf.vertex(i).position());
                ++i;
            }

            i = 0;
            for (const auto& pwt : pf.wedges().texCoords()) {
                REQUIRE(pwt.u() == tf.wedge(i).texCoord().u());
                REQUIRE(pwt.v() == tf.wedge(i).texCoord().v());
                ++i;
            }
        }
    }

    THEN("The imported PolyMesh has same custom components")
    {
        REQUIRE(pm.hasCustomComponent("cust_comp"));
        REQUIRE(pm.template isCustomComponentOfType<int>("cust_comp"));
        REQUIRE(
            !pm.template isCustomComponentOfType<unsigned int>("cust_comp"));
        REQUIRE(pm.template customComponent<int>("cust_comp") == 4);
        REQUIRE(pm.hasVertexCustomComponent("v_comp"));
        REQUIRE(pm.template isVertexCustomComponentOfType<float>("v_comp"));

        for (const auto& v : pm.vertices()) {
            REQUIRE(v.template customComponent<float>("v_comp") == 3.14f);
        }
    }
}

TEMPLATE_TEST_CASE(
    "Mesh Conversions - The polygonal cube mesh loaded on TriMesh",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMesh64,
    vcl::TriMesh64f)
{
    using TriMesh = TestType;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    THEN("The loaded TriMesh has 8 vertices and 12 faces")
    {
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);
    }
}

TEMPLATE_TEST_CASE(
    "Mesh Conversions - The polygonal cube mesh loaded on PolyMesh",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh  = typename TestType::first_type;
    using PolyMesh = typename TestType::second_type;

    PolyMesh pm =
        vcl::loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    THEN("The loaded PolyMesh has 8 vertices and 6 faces")
    {
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.faceCount() == 6);
    }

    GIVEN("When importing into TriMesh")
    {
        TriMesh tm;

        tm.importFrom(pm);

        THEN("The imported trimesh has same vertices")
        {
            REQUIRE(tm.vertexCount() == pm.vertexCount());

            for (unsigned int i = 0; i < tm.vertexCount(); ++i) {
                REQUIRE(tm.vertex(i).position() == pm.vertex(i).position());
            }
        }

        THEN("The imported trimesh has 12 faces")
        {
            REQUIRE(tm.faceCount() == 12);
        }
    }

    GIVEN("When importing a PolyMesh with degenerate face into TriMesh")
    {
        PolyMesh pmDeg;
        pmDeg.addVertices(4);
        pmDeg.vertex(0).position() = {0.0, 0.0, 0.0};
        pmDeg.vertex(1).position() = {1.0, 0.0, 0.0};
        pmDeg.vertex(2).position() = {1.0, 1.0, 0.0};
        pmDeg.vertex(3).position() = {0.0, 1.0, 0.0};

        auto f0 = pmDeg.addFace();
        pmDeg.face(f0).pushVertex(0);
        pmDeg.face(f0).pushVertex(1);
        pmDeg.face(f0).pushVertex(2);
        pmDeg.face(f0).pushVertex(3);

        auto f1 = pmDeg.addFace();
        pmDeg.face(f1).pushVertex(0);
        pmDeg.face(f1).pushVertex(1);

        TriMesh tm;
        tm.importFrom(pmDeg);

        THEN(
            "The degenerate face is deleted and the quad is triangulated into "
            "2 faces")
        {
            REQUIRE(tm.faceCount() == 2);
            REQUIRE(tm.face(f1).deleted());
        }
    }
}
