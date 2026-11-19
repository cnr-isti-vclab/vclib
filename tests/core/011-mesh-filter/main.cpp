// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "TriMesh Filter Vertices and Faces",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMesh64,
    vcl::TriMesh64f)
{
    using TriMesh = TestType;
    using IndexType = TriMesh::IndexType;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    THEN("The mesh has 8 vertices, 12 triangles")
    {
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);
    }

    THEN("Filter vertices")
    {
        std::vector<bool> filter = {
            true, false, false, true, false, false, true, true};

        TriMesh anotherMesh = vcl::vertexMeshFilter(tm, filter);

        THEN("The mesh has been filtered")
        {
            REQUIRE(anotherMesh.vertexCount() == 4);
            REQUIRE(anotherMesh.faceCount() == 0);

            REQUIRE(anotherMesh.hasVertexCustomComponent("birthVertex"));
            REQUIRE(
                anotherMesh.vertex(0).template customComponent<IndexType>(
                    "birthVertex") == 0);
            REQUIRE(
                anotherMesh.vertex(1).template customComponent<IndexType>(
                    "birthVertex") == 3);
            REQUIRE(
                anotherMesh.vertex(2).template customComponent<IndexType>(
                    "birthVertex") == 6);
            REQUIRE(
                anotherMesh.vertex(3).template customComponent<IndexType>(
                    "birthVertex") == 7);
        }
    }

    THEN("Filter faces")
    {
        std::vector<bool> filter = {
            true,
            false,
            false,
            true,
            false,
            false,
            true,
            true,
            false,
            false,
            true,
            false};

        TriMesh anotherMesh = vcl::faceMeshFilter(tm, filter);

        // order of vertices:
        // pos:   0 1 2 3 4 5 6 7
        // tm vi: 2 1 0 4 6 5 7 3

        THEN("The mesh has been filtered")
        {
            REQUIRE(anotherMesh.vertexCount() == 8);
            REQUIRE(anotherMesh.faceCount() == 5);

            REQUIRE(anotherMesh.hasVertexCustomComponent("birthVertex"));
            REQUIRE(
                anotherMesh.vertex(0).template customComponent<IndexType>(
                    "birthVertex") == 2);
            REQUIRE(
                anotherMesh.vertex(1).template customComponent<IndexType>(
                    "birthVertex") == 1);
            REQUIRE(
                anotherMesh.vertex(2).template customComponent<IndexType>(
                    "birthVertex") == 0);
            REQUIRE(
                anotherMesh.vertex(3).template customComponent<IndexType>(
                    "birthVertex") == 4);
            REQUIRE(
                anotherMesh.vertex(4).template customComponent<IndexType>(
                    "birthVertex") == 6);
            REQUIRE(
                anotherMesh.vertex(5).template customComponent<IndexType>(
                    "birthVertex") == 5);
            REQUIRE(
                anotherMesh.vertex(6).template customComponent<IndexType>(
                    "birthVertex") == 7);
            REQUIRE(
                anotherMesh.vertex(7).template customComponent<IndexType>(
                    "birthVertex") == 3);

            REQUIRE(anotherMesh.hasFaceCustomComponent("birthFace"));
            REQUIRE(
                anotherMesh.face(0).template customComponent<IndexType>(
                    "birthFace") == 0);
            REQUIRE(
                anotherMesh.face(1).template customComponent<IndexType>(
                    "birthFace") == 3);
            REQUIRE(
                anotherMesh.face(2).template customComponent<IndexType>(
                    "birthFace") == 6);
            REQUIRE(
                anotherMesh.face(3).template customComponent<IndexType>(
                    "birthFace") == 7);
            REQUIRE(
                anotherMesh.face(4).template customComponent<IndexType>(
                    "birthFace") == 10);
        }
    }
}

using Meshes         = std::pair<vcl::TriMesh, vcl::EdgeMesh>;
using Meshesf        = std::pair<vcl::TriMeshf, vcl::EdgeMeshf>;
using Meshes64  = std::pair<vcl::TriMesh64, vcl::EdgeMesh64>;
using Meshes64f = std::pair<vcl::TriMesh64f, vcl::EdgeMesh64f>;
using MeshesMixed1   = std::pair<vcl::TriMesh, vcl::EdgeMesh64>;
using MeshesMixed2   = std::pair<vcl::TriMesh64, vcl::EdgeMesh>;

TEMPLATE_TEST_CASE(
    "TriMesh Filter Face Edges",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f,
    MeshesMixed1,
    MeshesMixed2)
{
    using TriMesh  = typename TestType::first_type;
    using EdgeMesh = typename TestType::second_type;

    TriMesh cylinder = vcl::createCylinder<TriMesh>(1, 1, 36);
    cylinder.enableFaceAdjacentFaces();

    vcl::updateFaceAdjacentFaces(cylinder);

    // select crease edges
    vcl::selectCreaseFaceEdges(cylinder, -vcl::toRad(30.f), vcl::toRad(30.f));

    EdgeMesh creaseEdges = vcl::faceEdgeSelectionMeshFilter<EdgeMesh>(cylinder);

    THEN("The mesh has 36 * 2 crease edges")
    {
        REQUIRE(creaseEdges.edgeCount() == 36 * 2);
        REQUIRE(creaseEdges.hasVertexCustomComponent("birthVertex"));
        for (std::size_t i = 0; i < creaseEdges.vertexCount(); ++i) {
            auto birthV =
                creaseEdges.vertex(i)
                    .template customComponent<typename TriMesh::IndexType>(
                        "birthVertex");
            REQUIRE(birthV < cylinder.vertexCount());
        }
    }
}
