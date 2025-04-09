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

using Meshes         = std::pair<vcl::TriMesh, vcl::PolyMesh>;
using Meshesf        = std::pair<vcl::TriMeshf, vcl::PolyMeshf>;
using MeshesIndexed  = std::pair<vcl::TriMeshIndexed, vcl::PolyMeshIndexed>;
using MeshesIndexedf = std::pair<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf>;

TEMPLATE_TEST_CASE(
    "Mesh Conversions -The TextureDouble mesh loaded on TriMesh",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = typename TestType::first_type;
    using PolyMesh = typename TestType::second_type;

    TriMesh tm =
        vcl::loadPly<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply");

    tm.template addCustomComponent<int>("cust_comp", 4);
    tm.template addPerVertexCustomComponent<float>("v_comp");
    tm.deleteFace(3);

    for (auto& v : tm.vertices()) {
        v.template customComponent<float>("v_comp") = 3.14f;
    }

    THEN("The mesh has 8 vertices, 4 triangles, per face wedge texcoords")
    {
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 3);
        REQUIRE(tm.isPerFaceWedgeTexCoordsEnabled());
        REQUIRE(tm.hasCustomComponent("cust_comp"));
        REQUIRE(tm.template isCustomComponentOfType<int>("cust_comp"));
        REQUIRE(
            !tm.template isCustomComponentOfType<unsigned int>("cust_comp"));
        REQUIRE(tm.template customComponent<int>("cust_comp") == 4);
        REQUIRE(tm.hasPerVertexCustomComponent("v_comp"));

        for (const auto& v : tm.vertices()) {
            REQUIRE(v.template customComponent<float>("v_comp") == 3.14f);
        }
    }

    PolyMesh pm;
    REQUIRE(!pm.isPerFaceWedgeTexCoordsEnabled());
    pm.enableSameOptionalComponentsOf(tm);
    REQUIRE(pm.isPerFaceWedgeTexCoordsEnabled());

    pm.importFrom(tm);

    THEN("The imported PolyMesh has same vertex and face number")
    {
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 3);
    }

    THEN("The imported vetices have same coordinates")
    {
        for (const auto& tv : tm.vertices()) {
            REQUIRE(pm.vertex(tv.index()).coord() == tv.coord());
        }
    }

    THEN("The imported faces have same vertices")
    {
        for (const auto& pf : pm.faces()) {
            REQUIRE(pf.vertexNumber() == 3);
            const auto& tf = tm.face(pf.index());

            unsigned int i = 0;
            for (const auto* pv : pf.vertices()) {
                REQUIRE(pv->index() == tf.vertexIndex(i));
                REQUIRE(pv->coord() == tf.vertex(i)->coord());
                ++i;
            }

            i = 0;
            for (const auto& pwt : pf.wedgeTexCoords()) {
                REQUIRE(pwt.u() == tf.wedgeTexCoord(i).u());
                REQUIRE(pwt.v() == tf.wedgeTexCoord(i).v());
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
        REQUIRE(pm.hasPerVertexCustomComponent("v_comp"));
        REQUIRE(pm.template isPerVertexCustomComponentOfType<float>("v_comp"));

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
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh tm =
        vcl::loadPly<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    THEN("The loaded TriMesh has 8 vertices and 12 faces")
    {
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }
}

TEMPLATE_TEST_CASE(
    "Mesh Conversions - The polygonal cube mesh loaded on PolyMesh",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = typename TestType::first_type;
    using PolyMesh = typename TestType::second_type;

    PolyMesh pm =
        vcl::loadPly<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    THEN("The loaded PolyMesh has 8 vertices and 6 faces")
    {
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 6);
    }

    GIVEN("When importing into TriMesh")
    {
        TriMesh tm;

        tm.importFrom(pm);

        THEN("The imported trimesh has same vertices")
        {
            REQUIRE(tm.vertexNumber() == pm.vertexNumber());

            for (unsigned int i = 0; i < tm.vertexNumber(); ++i) {
                REQUIRE(tm.vertex(i).coord() == pm.vertex(i).coord());
            }
        }

        THEN("The imported trimesh has 12 faces")
        {
            REQUIRE(tm.faceNumber() == 12);
        }
    }
}
