/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

TEMPLATE_TEST_CASE(
    "TriMesh Filter Vertices and Faces",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh tm = vcl::loadPly<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    THEN("The mesh has 8 vertices, 12 triangles")
    {
        REQUIRE(tm.vertexNumber() == 8);
        REQUIRE(tm.faceNumber() == 12);
    }

    THEN("Filter vertices")
    {
        std::vector<bool> filter = {
            true, false, false, true, false, false, true, true};

        TriMesh anotherMesh = vcl::perVertexMeshFilter(tm, filter);

        THEN("The mesh has been filtered")
        {
            REQUIRE(anotherMesh.vertexNumber() == 4);
            REQUIRE(anotherMesh.faceNumber() == 0);

            REQUIRE(anotherMesh.hasPerVertexCustomComponent("birthVertex"));
            REQUIRE(
                anotherMesh.vertex(0).template customComponent<unsigned int>(
                    "birthVertex") == 0);
            REQUIRE(
                anotherMesh.vertex(1).template customComponent<unsigned int>(
                    "birthVertex") == 3);
            REQUIRE(
                anotherMesh.vertex(2).template customComponent<unsigned int>(
                    "birthVertex") == 6);
            REQUIRE(
                anotherMesh.vertex(3).template customComponent<unsigned int>(
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

        TriMesh anotherMesh = vcl::perFaceMeshFilter(tm, filter);

        // order of vertices:
        // pos:   0 1 2 3 4 5 6 7
        // tm vi: 2 1 0 4 6 5 7 3

        THEN("The mesh has been filtered")
        {
            REQUIRE(anotherMesh.vertexNumber() == 8);
            REQUIRE(anotherMesh.faceNumber() == 5);

            REQUIRE(anotherMesh.hasPerVertexCustomComponent("birthVertex"));
            REQUIRE(
                anotherMesh.vertex(0).template customComponent<unsigned int>(
                    "birthVertex") == 2);
            REQUIRE(
                anotherMesh.vertex(1).template customComponent<unsigned int>(
                    "birthVertex") == 1);
            REQUIRE(
                anotherMesh.vertex(2).template customComponent<unsigned int>(
                    "birthVertex") == 0);
            REQUIRE(
                anotherMesh.vertex(3).template customComponent<unsigned int>(
                    "birthVertex") == 4);
            REQUIRE(
                anotherMesh.vertex(4).template customComponent<unsigned int>(
                    "birthVertex") == 6);
            REQUIRE(
                anotherMesh.vertex(5).template customComponent<unsigned int>(
                    "birthVertex") == 5);
            REQUIRE(
                anotherMesh.vertex(6).template customComponent<unsigned int>(
                    "birthVertex") == 7);
            REQUIRE(
                anotherMesh.vertex(7).template customComponent<unsigned int>(
                    "birthVertex") == 3);

            REQUIRE(anotherMesh.hasPerFaceCustomComponent("birthFace"));
            REQUIRE(
                anotherMesh.face(0).template customComponent<unsigned int>(
                    "birthFace") == 0);
            REQUIRE(
                anotherMesh.face(1).template customComponent<unsigned int>(
                    "birthFace") == 3);
            REQUIRE(
                anotherMesh.face(2).template customComponent<unsigned int>(
                    "birthFace") == 6);
            REQUIRE(
                anotherMesh.face(3).template customComponent<unsigned int>(
                    "birthFace") == 7);
            REQUIRE(
                anotherMesh.face(4).template customComponent<unsigned int>(
                    "birthFace") == 10);
        }
    }
}

using Meshes         = std::pair<vcl::TriMesh, vcl::EdgeMesh>;
using Meshesf        = std::pair<vcl::TriMeshf, vcl::EdgeMeshf>;
using MeshesIndexed  = std::pair<vcl::TriMeshIndexed, vcl::EdgeMeshIndexed>;
using MeshesIndexedf = std::pair<vcl::TriMeshIndexedf, vcl::EdgeMeshIndexedf>;

TEMPLATE_TEST_CASE(
    "TriMesh Filter Face Edges",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = typename TestType::first_type;
    using EdgeMesh = typename TestType::second_type;

    TriMesh cylinder = vcl::createCylinder<TriMesh>(1, 1, 36);
    cylinder.enablePerFaceAdjacentFaces();

    vcl::updatePerFaceAdjacentFaces(cylinder);

    // select crease edges
    vcl::selectCreaseFaceEdges(cylinder, -vcl::toRad(30.f), vcl::toRad(30.f));

    EdgeMesh creaseEdges =
        vcl::perFaceEdgeSelectionMeshFilter<EdgeMesh>(cylinder);

    THEN("The mesh has 36 * 2 crease edges")
    {
        REQUIRE(creaseEdges.edgeNumber() == 36 * 2);
    }
}
