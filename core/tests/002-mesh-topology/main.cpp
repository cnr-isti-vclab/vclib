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
    "TriMesh Topology",
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

    THEN("Test Face Vertices")
    {
        REQUIRE(tm.face(0).vertexNumber() == 3);
        REQUIRE(tm.face(0).vertex(0) == &tm.vertex(2));
        REQUIRE(tm.face(0).vertex(1) == &tm.vertex(1));
        REQUIRE(tm.face(0).vertex(2) == &tm.vertex(0));

        REQUIRE(tm.face(1).vertexNumber() == 3);
        REQUIRE(tm.face(1).vertex(0) == &tm.vertex(1));
        REQUIRE(tm.face(1).vertex(1) == &tm.vertex(2));
        REQUIRE(tm.face(1).vertex(2) == &tm.vertex(3));

        REQUIRE(tm.face(2).vertexNumber() == 3);
        REQUIRE(tm.face(2).vertex(0) == &tm.vertex(4));
        REQUIRE(tm.face(2).vertex(1) == &tm.vertex(2));
        REQUIRE(tm.face(2).vertex(2) == &tm.vertex(0));

        REQUIRE(tm.face(3).vertexNumber() == 3);
        REQUIRE(tm.face(3).vertex(0) == &tm.vertex(2));
        REQUIRE(tm.face(3).vertex(1) == &tm.vertex(4));
        REQUIRE(tm.face(3).vertex(2) == &tm.vertex(6));

        REQUIRE(tm.face(4).vertexNumber() == 3);
        REQUIRE(tm.face(4).vertex(0) == &tm.vertex(1));
        REQUIRE(tm.face(4).vertex(1) == &tm.vertex(4));
        REQUIRE(tm.face(4).vertex(2) == &tm.vertex(0));

        REQUIRE(tm.face(5).vertexNumber() == 3);
        REQUIRE(tm.face(5).vertex(0) == &tm.vertex(4));
        REQUIRE(tm.face(5).vertex(1) == &tm.vertex(1));
        REQUIRE(tm.face(5).vertex(2) == &tm.vertex(5));

        REQUIRE(tm.face(6).vertexNumber() == 3);
        REQUIRE(tm.face(6).vertex(0) == &tm.vertex(6));
        REQUIRE(tm.face(6).vertex(1) == &tm.vertex(5));
        REQUIRE(tm.face(6).vertex(2) == &tm.vertex(7));

        REQUIRE(tm.face(7).vertexNumber() == 3);
        REQUIRE(tm.face(7).vertex(0) == &tm.vertex(5));
        REQUIRE(tm.face(7).vertex(1) == &tm.vertex(6));
        REQUIRE(tm.face(7).vertex(2) == &tm.vertex(4));

        REQUIRE(tm.face(8).vertexNumber() == 3);
        REQUIRE(tm.face(8).vertex(0) == &tm.vertex(3));
        REQUIRE(tm.face(8).vertex(1) == &tm.vertex(6));
        REQUIRE(tm.face(8).vertex(2) == &tm.vertex(7));

        REQUIRE(tm.face(9).vertexNumber() == 3);
        REQUIRE(tm.face(9).vertex(0) == &tm.vertex(6));
        REQUIRE(tm.face(9).vertex(1) == &tm.vertex(3));
        REQUIRE(tm.face(9).vertex(2) == &tm.vertex(2));

        REQUIRE(tm.face(10).vertexNumber() == 3);
        REQUIRE(tm.face(10).vertex(0) == &tm.vertex(5));
        REQUIRE(tm.face(10).vertex(1) == &tm.vertex(3));
        REQUIRE(tm.face(10).vertex(2) == &tm.vertex(7));

        REQUIRE(tm.face(11).vertexNumber() == 3);
        REQUIRE(tm.face(11).vertex(0) == &tm.vertex(3));
        REQUIRE(tm.face(11).vertex(1) == &tm.vertex(5));
        REQUIRE(tm.face(11).vertex(2) == &tm.vertex(1));
    }

    THEN("Test Per Vertex Adjacent Faces")
    {
        tm.enablePerVertexAdjacentFaces();
        vcl::updatePerVertexAdjacentFaces(tm);

        REQUIRE(tm.vertex(0).adjFacesNumber() == 3);
        REQUIRE(tm.vertex(0).adjFace(0) == &tm.face(0));
        REQUIRE(tm.vertex(0).adjFace(1) == &tm.face(2));
        REQUIRE(tm.vertex(0).adjFace(2) == &tm.face(4));

        REQUIRE(tm.vertex(1).adjFacesNumber() == 5);
        REQUIRE(tm.vertex(1).adjFace(0) == &tm.face(0));
        REQUIRE(tm.vertex(1).adjFace(1) == &tm.face(1));
        REQUIRE(tm.vertex(1).adjFace(2) == &tm.face(4));
        REQUIRE(tm.vertex(1).adjFace(3) == &tm.face(5));
        REQUIRE(tm.vertex(1).adjFace(4) == &tm.face(11));

        REQUIRE(tm.vertex(2).adjFacesNumber() == 5);
        REQUIRE(tm.vertex(2).adjFace(0) == &tm.face(0));
        REQUIRE(tm.vertex(2).adjFace(1) == &tm.face(1));
        REQUIRE(tm.vertex(2).adjFace(2) == &tm.face(2));
        REQUIRE(tm.vertex(2).adjFace(3) == &tm.face(3));
        REQUIRE(tm.vertex(2).adjFace(4) == &tm.face(9));

        REQUIRE(tm.vertex(3).adjFacesNumber() == 5);
        REQUIRE(tm.vertex(3).adjFace(0) == &tm.face(1));
        REQUIRE(tm.vertex(3).adjFace(1) == &tm.face(8));
        REQUIRE(tm.vertex(3).adjFace(2) == &tm.face(9));
        REQUIRE(tm.vertex(3).adjFace(3) == &tm.face(10));
        REQUIRE(tm.vertex(3).adjFace(4) == &tm.face(11));

        REQUIRE(tm.vertex(4).adjFacesNumber() == 5);
        REQUIRE(tm.vertex(4).adjFace(0) == &tm.face(2));
        REQUIRE(tm.vertex(4).adjFace(1) == &tm.face(3));
        REQUIRE(tm.vertex(4).adjFace(2) == &tm.face(4));
        REQUIRE(tm.vertex(4).adjFace(3) == &tm.face(5));
        REQUIRE(tm.vertex(4).adjFace(4) == &tm.face(7));

        REQUIRE(tm.vertex(5).adjFacesNumber() == 5);
        REQUIRE(tm.vertex(5).adjFace(0) == &tm.face(5));
        REQUIRE(tm.vertex(5).adjFace(1) == &tm.face(6));
        REQUIRE(tm.vertex(5).adjFace(2) == &tm.face(7));
        REQUIRE(tm.vertex(5).adjFace(3) == &tm.face(10));
        REQUIRE(tm.vertex(5).adjFace(4) == &tm.face(11));

        REQUIRE(tm.vertex(6).adjFacesNumber() == 5);
        REQUIRE(tm.vertex(6).adjFace(0) == &tm.face(3));
        REQUIRE(tm.vertex(6).adjFace(1) == &tm.face(6));
        REQUIRE(tm.vertex(6).adjFace(2) == &tm.face(7));
        REQUIRE(tm.vertex(6).adjFace(3) == &tm.face(8));
        REQUIRE(tm.vertex(6).adjFace(4) == &tm.face(9));

        REQUIRE(tm.vertex(7).adjFacesNumber() == 3);
        REQUIRE(tm.vertex(7).adjFace(0) == &tm.face(6));
        REQUIRE(tm.vertex(7).adjFace(1) == &tm.face(8));
        REQUIRE(tm.vertex(7).adjFace(2) == &tm.face(10));
    }

    THEN("Test Per Vertex Adjacent Vertices")
    {
        tm.enablePerVertexAdjacentVertices();
        vcl::updatePerVertexAdjacentVertices(tm);

        REQUIRE(tm.vertex(0).adjVerticesNumber() == 3);
        REQUIRE(tm.vertex(0).adjVertex(0) == &tm.vertex(1));
        REQUIRE(tm.vertex(0).adjVertex(1) == &tm.vertex(2));
        REQUIRE(tm.vertex(0).adjVertex(2) == &tm.vertex(4));

        REQUIRE(tm.vertex(1).adjVerticesNumber() == 5);
        REQUIRE(tm.vertex(1).adjVertex(0) == &tm.vertex(0));
        REQUIRE(tm.vertex(1).adjVertex(1) == &tm.vertex(2));
        REQUIRE(tm.vertex(1).adjVertex(2) == &tm.vertex(3));
        REQUIRE(tm.vertex(1).adjVertex(3) == &tm.vertex(4));
        REQUIRE(tm.vertex(1).adjVertex(4) == &tm.vertex(5));

        REQUIRE(tm.vertex(2).adjVerticesNumber() == 5);
        REQUIRE(tm.vertex(2).adjVertex(0) == &tm.vertex(0));
        REQUIRE(tm.vertex(2).adjVertex(1) == &tm.vertex(1));
        REQUIRE(tm.vertex(2).adjVertex(2) == &tm.vertex(3));
        REQUIRE(tm.vertex(2).adjVertex(3) == &tm.vertex(4));
        REQUIRE(tm.vertex(2).adjVertex(4) == &tm.vertex(6));

        REQUIRE(tm.vertex(3).adjVerticesNumber() == 5);
        REQUIRE(tm.vertex(3).adjVertex(0) == &tm.vertex(1));
        REQUIRE(tm.vertex(3).adjVertex(1) == &tm.vertex(2));
        REQUIRE(tm.vertex(3).adjVertex(2) == &tm.vertex(5));
        REQUIRE(tm.vertex(3).adjVertex(3) == &tm.vertex(6));
        REQUIRE(tm.vertex(3).adjVertex(4) == &tm.vertex(7));

        REQUIRE(tm.vertex(4).adjVerticesNumber() == 5);
        REQUIRE(tm.vertex(4).adjVertex(0) == &tm.vertex(0));
        REQUIRE(tm.vertex(4).adjVertex(1) == &tm.vertex(1));
        REQUIRE(tm.vertex(4).adjVertex(2) == &tm.vertex(2));
        REQUIRE(tm.vertex(4).adjVertex(3) == &tm.vertex(5));
        REQUIRE(tm.vertex(4).adjVertex(4) == &tm.vertex(6));

        REQUIRE(tm.vertex(5).adjVerticesNumber() == 5);
        REQUIRE(tm.vertex(5).adjVertex(0) == &tm.vertex(1));
        REQUIRE(tm.vertex(5).adjVertex(1) == &tm.vertex(3));
        REQUIRE(tm.vertex(5).adjVertex(2) == &tm.vertex(4));
        REQUIRE(tm.vertex(5).adjVertex(3) == &tm.vertex(6));
        REQUIRE(tm.vertex(5).adjVertex(4) == &tm.vertex(7));

        REQUIRE(tm.vertex(6).adjVerticesNumber() == 5);
        REQUIRE(tm.vertex(6).adjVertex(0) == &tm.vertex(2));
        REQUIRE(tm.vertex(6).adjVertex(1) == &tm.vertex(3));
        REQUIRE(tm.vertex(6).adjVertex(2) == &tm.vertex(4));
        REQUIRE(tm.vertex(6).adjVertex(3) == &tm.vertex(5));
        REQUIRE(tm.vertex(6).adjVertex(4) == &tm.vertex(7));

        REQUIRE(tm.vertex(7).adjVerticesNumber() == 3);
        REQUIRE(tm.vertex(7).adjVertex(0) == &tm.vertex(3));
        REQUIRE(tm.vertex(7).adjVertex(1) == &tm.vertex(5));
        REQUIRE(tm.vertex(7).adjVertex(2) == &tm.vertex(6));
    }

    THEN("Test Per Face Adjacent Faces")
    {
        tm.enablePerFaceAdjacentFaces();
        vcl::updatePerFaceAdjacentFaces(tm);

        REQUIRE(tm.face(0).adjFacesNumber() == 3);
        REQUIRE(tm.face(0).adjFace(0) == &tm.face(1));
        REQUIRE(tm.face(0).adjFace(1) == &tm.face(4));
        REQUIRE(tm.face(0).adjFace(2) == &tm.face(2));

        REQUIRE(tm.face(1).adjFacesNumber() == 3);
        REQUIRE(tm.face(1).adjFace(0) == &tm.face(0));
        REQUIRE(tm.face(1).adjFace(1) == &tm.face(9));
        REQUIRE(tm.face(1).adjFace(2) == &tm.face(11));

        REQUIRE(tm.face(2).adjFacesNumber() == 3);
        REQUIRE(tm.face(2).adjFace(0) == &tm.face(3));
        REQUIRE(tm.face(2).adjFace(1) == &tm.face(0));
        REQUIRE(tm.face(2).adjFace(2) == &tm.face(4));

        REQUIRE(tm.face(3).adjFacesNumber() == 3);
        REQUIRE(tm.face(3).adjFace(0) == &tm.face(2));
        REQUIRE(tm.face(3).adjFace(1) == &tm.face(7));
        REQUIRE(tm.face(3).adjFace(2) == &tm.face(9));

        REQUIRE(tm.face(4).adjFacesNumber() == 3);
        REQUIRE(tm.face(4).adjFace(0) == &tm.face(5));
        REQUIRE(tm.face(4).adjFace(1) == &tm.face(2));
        REQUIRE(tm.face(4).adjFace(2) == &tm.face(0));

        REQUIRE(tm.face(5).adjFacesNumber() == 3);
        REQUIRE(tm.face(5).adjFace(0) == &tm.face(4));
        REQUIRE(tm.face(5).adjFace(1) == &tm.face(11));
        REQUIRE(tm.face(5).adjFace(2) == &tm.face(7));

        REQUIRE(tm.face(6).adjFacesNumber() == 3);
        REQUIRE(tm.face(6).adjFace(0) == &tm.face(7));
        REQUIRE(tm.face(6).adjFace(1) == &tm.face(10));
        REQUIRE(tm.face(6).adjFace(2) == &tm.face(8));

        REQUIRE(tm.face(7).adjFacesNumber() == 3);
        REQUIRE(tm.face(7).adjFace(0) == &tm.face(6));
        REQUIRE(tm.face(7).adjFace(1) == &tm.face(3));
        REQUIRE(tm.face(7).adjFace(2) == &tm.face(5));

        REQUIRE(tm.face(8).adjFacesNumber() == 3);
        REQUIRE(tm.face(8).adjFace(0) == &tm.face(9));
        REQUIRE(tm.face(8).adjFace(1) == &tm.face(6));
        REQUIRE(tm.face(8).adjFace(2) == &tm.face(10));

        REQUIRE(tm.face(9).adjFacesNumber() == 3);
        REQUIRE(tm.face(9).adjFace(0) == &tm.face(8));
        REQUIRE(tm.face(9).adjFace(1) == &tm.face(1));
        REQUIRE(tm.face(9).adjFace(2) == &tm.face(3));

        REQUIRE(tm.face(10).adjFacesNumber() == 3);
        REQUIRE(tm.face(10).adjFace(0) == &tm.face(11));
        REQUIRE(tm.face(10).adjFace(1) == &tm.face(8));
        REQUIRE(tm.face(10).adjFace(2) == &tm.face(6));

        REQUIRE(tm.face(11).adjFacesNumber() == 3);
        REQUIRE(tm.face(11).adjFace(0) == &tm.face(10));
        REQUIRE(tm.face(11).adjFace(1) == &tm.face(5));
        REQUIRE(tm.face(11).adjFace(2) == &tm.face(1));
    }
}

TEMPLATE_TEST_CASE(
    "PolyMesh Triangle Mesh Topology",
    "",
    vcl::PolyMesh,
    vcl::PolyMeshf)
{
    using TriMesh = TestType;

    TriMesh pm = vcl::loadPly<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    THEN("The mesh has 8 vertices, 12 triangles")
    {
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 12);
    }

    THEN("Test Face Vertices")
    {
        REQUIRE(pm.face(0).vertexNumber() == 3);
        REQUIRE(pm.face(0).vertex(0) == &pm.vertex(2));
        REQUIRE(pm.face(0).vertex(1) == &pm.vertex(1));
        REQUIRE(pm.face(0).vertex(2) == &pm.vertex(0));

        REQUIRE(pm.face(1).vertexNumber() == 3);
        REQUIRE(pm.face(1).vertex(0) == &pm.vertex(1));
        REQUIRE(pm.face(1).vertex(1) == &pm.vertex(2));
        REQUIRE(pm.face(1).vertex(2) == &pm.vertex(3));

        REQUIRE(pm.face(2).vertexNumber() == 3);
        REQUIRE(pm.face(2).vertex(0) == &pm.vertex(4));
        REQUIRE(pm.face(2).vertex(1) == &pm.vertex(2));
        REQUIRE(pm.face(2).vertex(2) == &pm.vertex(0));

        REQUIRE(pm.face(3).vertexNumber() == 3);
        REQUIRE(pm.face(3).vertex(0) == &pm.vertex(2));
        REQUIRE(pm.face(3).vertex(1) == &pm.vertex(4));
        REQUIRE(pm.face(3).vertex(2) == &pm.vertex(6));

        REQUIRE(pm.face(4).vertexNumber() == 3);
        REQUIRE(pm.face(4).vertex(0) == &pm.vertex(1));
        REQUIRE(pm.face(4).vertex(1) == &pm.vertex(4));
        REQUIRE(pm.face(4).vertex(2) == &pm.vertex(0));

        REQUIRE(pm.face(5).vertexNumber() == 3);
        REQUIRE(pm.face(5).vertex(0) == &pm.vertex(4));
        REQUIRE(pm.face(5).vertex(1) == &pm.vertex(1));
        REQUIRE(pm.face(5).vertex(2) == &pm.vertex(5));

        REQUIRE(pm.face(6).vertexNumber() == 3);
        REQUIRE(pm.face(6).vertex(0) == &pm.vertex(6));
        REQUIRE(pm.face(6).vertex(1) == &pm.vertex(5));
        REQUIRE(pm.face(6).vertex(2) == &pm.vertex(7));

        REQUIRE(pm.face(7).vertexNumber() == 3);
        REQUIRE(pm.face(7).vertex(0) == &pm.vertex(5));
        REQUIRE(pm.face(7).vertex(1) == &pm.vertex(6));
        REQUIRE(pm.face(7).vertex(2) == &pm.vertex(4));

        REQUIRE(pm.face(8).vertexNumber() == 3);
        REQUIRE(pm.face(8).vertex(0) == &pm.vertex(3));
        REQUIRE(pm.face(8).vertex(1) == &pm.vertex(6));
        REQUIRE(pm.face(8).vertex(2) == &pm.vertex(7));

        REQUIRE(pm.face(9).vertexNumber() == 3);
        REQUIRE(pm.face(9).vertex(0) == &pm.vertex(6));
        REQUIRE(pm.face(9).vertex(1) == &pm.vertex(3));
        REQUIRE(pm.face(9).vertex(2) == &pm.vertex(2));

        REQUIRE(pm.face(10).vertexNumber() == 3);
        REQUIRE(pm.face(10).vertex(0) == &pm.vertex(5));
        REQUIRE(pm.face(10).vertex(1) == &pm.vertex(3));
        REQUIRE(pm.face(10).vertex(2) == &pm.vertex(7));

        REQUIRE(pm.face(11).vertexNumber() == 3);
        REQUIRE(pm.face(11).vertex(0) == &pm.vertex(3));
        REQUIRE(pm.face(11).vertex(1) == &pm.vertex(5));
        REQUIRE(pm.face(11).vertex(2) == &pm.vertex(1));
    }

    THEN("Test Per Vertex Adjacent Faces")
    {
        pm.enablePerVertexAdjacentFaces();
        vcl::updatePerVertexAdjacentFaces(pm);

        REQUIRE(pm.vertex(0).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(0).adjFace(0) == &pm.face(0));
        REQUIRE(pm.vertex(0).adjFace(1) == &pm.face(2));
        REQUIRE(pm.vertex(0).adjFace(2) == &pm.face(4));

        REQUIRE(pm.vertex(1).adjFacesNumber() == 5);
        REQUIRE(pm.vertex(1).adjFace(0) == &pm.face(0));
        REQUIRE(pm.vertex(1).adjFace(1) == &pm.face(1));
        REQUIRE(pm.vertex(1).adjFace(2) == &pm.face(4));
        REQUIRE(pm.vertex(1).adjFace(3) == &pm.face(5));
        REQUIRE(pm.vertex(1).adjFace(4) == &pm.face(11));

        REQUIRE(pm.vertex(2).adjFacesNumber() == 5);
        REQUIRE(pm.vertex(2).adjFace(0) == &pm.face(0));
        REQUIRE(pm.vertex(2).adjFace(1) == &pm.face(1));
        REQUIRE(pm.vertex(2).adjFace(2) == &pm.face(2));
        REQUIRE(pm.vertex(2).adjFace(3) == &pm.face(3));
        REQUIRE(pm.vertex(2).adjFace(4) == &pm.face(9));

        REQUIRE(pm.vertex(3).adjFacesNumber() == 5);
        REQUIRE(pm.vertex(3).adjFace(0) == &pm.face(1));
        REQUIRE(pm.vertex(3).adjFace(1) == &pm.face(8));
        REQUIRE(pm.vertex(3).adjFace(2) == &pm.face(9));
        REQUIRE(pm.vertex(3).adjFace(3) == &pm.face(10));
        REQUIRE(pm.vertex(3).adjFace(4) == &pm.face(11));

        REQUIRE(pm.vertex(4).adjFacesNumber() == 5);
        REQUIRE(pm.vertex(4).adjFace(0) == &pm.face(2));
        REQUIRE(pm.vertex(4).adjFace(1) == &pm.face(3));
        REQUIRE(pm.vertex(4).adjFace(2) == &pm.face(4));
        REQUIRE(pm.vertex(4).adjFace(3) == &pm.face(5));
        REQUIRE(pm.vertex(4).adjFace(4) == &pm.face(7));

        REQUIRE(pm.vertex(5).adjFacesNumber() == 5);
        REQUIRE(pm.vertex(5).adjFace(0) == &pm.face(5));
        REQUIRE(pm.vertex(5).adjFace(1) == &pm.face(6));
        REQUIRE(pm.vertex(5).adjFace(2) == &pm.face(7));
        REQUIRE(pm.vertex(5).adjFace(3) == &pm.face(10));
        REQUIRE(pm.vertex(5).adjFace(4) == &pm.face(11));

        REQUIRE(pm.vertex(6).adjFacesNumber() == 5);
        REQUIRE(pm.vertex(6).adjFace(0) == &pm.face(3));
        REQUIRE(pm.vertex(6).adjFace(1) == &pm.face(6));
        REQUIRE(pm.vertex(6).adjFace(2) == &pm.face(7));
        REQUIRE(pm.vertex(6).adjFace(3) == &pm.face(8));
        REQUIRE(pm.vertex(6).adjFace(4) == &pm.face(9));

        REQUIRE(pm.vertex(7).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(7).adjFace(0) == &pm.face(6));
        REQUIRE(pm.vertex(7).adjFace(1) == &pm.face(8));
        REQUIRE(pm.vertex(7).adjFace(2) == &pm.face(10));
    }

    THEN("Test Per Vertex Adjacent Vertices")
    {
        pm.enablePerVertexAdjacentVertices();
        vcl::updatePerVertexAdjacentVertices(pm);

        REQUIRE(pm.vertex(0).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(0).adjVertex(0) == &pm.vertex(1));
        REQUIRE(pm.vertex(0).adjVertex(1) == &pm.vertex(2));
        REQUIRE(pm.vertex(0).adjVertex(2) == &pm.vertex(4));

        REQUIRE(pm.vertex(1).adjVerticesNumber() == 5);
        REQUIRE(pm.vertex(1).adjVertex(0) == &pm.vertex(0));
        REQUIRE(pm.vertex(1).adjVertex(1) == &pm.vertex(2));
        REQUIRE(pm.vertex(1).adjVertex(2) == &pm.vertex(3));
        REQUIRE(pm.vertex(1).adjVertex(3) == &pm.vertex(4));
        REQUIRE(pm.vertex(1).adjVertex(4) == &pm.vertex(5));

        REQUIRE(pm.vertex(2).adjVerticesNumber() == 5);
        REQUIRE(pm.vertex(2).adjVertex(0) == &pm.vertex(0));
        REQUIRE(pm.vertex(2).adjVertex(1) == &pm.vertex(1));
        REQUIRE(pm.vertex(2).adjVertex(2) == &pm.vertex(3));
        REQUIRE(pm.vertex(2).adjVertex(3) == &pm.vertex(4));
        REQUIRE(pm.vertex(2).adjVertex(4) == &pm.vertex(6));

        REQUIRE(pm.vertex(3).adjVerticesNumber() == 5);
        REQUIRE(pm.vertex(3).adjVertex(0) == &pm.vertex(1));
        REQUIRE(pm.vertex(3).adjVertex(1) == &pm.vertex(2));
        REQUIRE(pm.vertex(3).adjVertex(2) == &pm.vertex(5));
        REQUIRE(pm.vertex(3).adjVertex(3) == &pm.vertex(6));
        REQUIRE(pm.vertex(3).adjVertex(4) == &pm.vertex(7));

        REQUIRE(pm.vertex(4).adjVerticesNumber() == 5);
        REQUIRE(pm.vertex(4).adjVertex(0) == &pm.vertex(0));
        REQUIRE(pm.vertex(4).adjVertex(1) == &pm.vertex(1));
        REQUIRE(pm.vertex(4).adjVertex(2) == &pm.vertex(2));
        REQUIRE(pm.vertex(4).adjVertex(3) == &pm.vertex(5));
        REQUIRE(pm.vertex(4).adjVertex(4) == &pm.vertex(6));

        REQUIRE(pm.vertex(5).adjVerticesNumber() == 5);
        REQUIRE(pm.vertex(5).adjVertex(0) == &pm.vertex(1));
        REQUIRE(pm.vertex(5).adjVertex(1) == &pm.vertex(3));
        REQUIRE(pm.vertex(5).adjVertex(2) == &pm.vertex(4));
        REQUIRE(pm.vertex(5).adjVertex(3) == &pm.vertex(6));
        REQUIRE(pm.vertex(5).adjVertex(4) == &pm.vertex(7));

        REQUIRE(pm.vertex(6).adjVerticesNumber() == 5);
        REQUIRE(pm.vertex(6).adjVertex(0) == &pm.vertex(2));
        REQUIRE(pm.vertex(6).adjVertex(1) == &pm.vertex(3));
        REQUIRE(pm.vertex(6).adjVertex(2) == &pm.vertex(4));
        REQUIRE(pm.vertex(6).adjVertex(3) == &pm.vertex(5));
        REQUIRE(pm.vertex(6).adjVertex(4) == &pm.vertex(7));

        REQUIRE(pm.vertex(7).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(7).adjVertex(0) == &pm.vertex(3));
        REQUIRE(pm.vertex(7).adjVertex(1) == &pm.vertex(5));
        REQUIRE(pm.vertex(7).adjVertex(2) == &pm.vertex(6));
    }

    THEN("Test Per Face Adjacent Faces")
    {
        pm.enablePerFaceAdjacentFaces();
        vcl::updatePerFaceAdjacentFaces(pm);

        REQUIRE(pm.face(0).adjFacesNumber() == 3);
        REQUIRE(pm.face(0).adjFace(0) == &pm.face(1));
        REQUIRE(pm.face(0).adjFace(1) == &pm.face(4));
        REQUIRE(pm.face(0).adjFace(2) == &pm.face(2));

        REQUIRE(pm.face(1).adjFacesNumber() == 3);
        REQUIRE(pm.face(1).adjFace(0) == &pm.face(0));
        REQUIRE(pm.face(1).adjFace(1) == &pm.face(9));
        REQUIRE(pm.face(1).adjFace(2) == &pm.face(11));

        REQUIRE(pm.face(2).adjFacesNumber() == 3);
        REQUIRE(pm.face(2).adjFace(0) == &pm.face(3));
        REQUIRE(pm.face(2).adjFace(1) == &pm.face(0));
        REQUIRE(pm.face(2).adjFace(2) == &pm.face(4));

        REQUIRE(pm.face(3).adjFacesNumber() == 3);
        REQUIRE(pm.face(3).adjFace(0) == &pm.face(2));
        REQUIRE(pm.face(3).adjFace(1) == &pm.face(7));
        REQUIRE(pm.face(3).adjFace(2) == &pm.face(9));

        REQUIRE(pm.face(4).adjFacesNumber() == 3);
        REQUIRE(pm.face(4).adjFace(0) == &pm.face(5));
        REQUIRE(pm.face(4).adjFace(1) == &pm.face(2));
        REQUIRE(pm.face(4).adjFace(2) == &pm.face(0));

        REQUIRE(pm.face(5).adjFacesNumber() == 3);
        REQUIRE(pm.face(5).adjFace(0) == &pm.face(4));
        REQUIRE(pm.face(5).adjFace(1) == &pm.face(11));
        REQUIRE(pm.face(5).adjFace(2) == &pm.face(7));

        REQUIRE(pm.face(6).adjFacesNumber() == 3);
        REQUIRE(pm.face(6).adjFace(0) == &pm.face(7));
        REQUIRE(pm.face(6).adjFace(1) == &pm.face(10));
        REQUIRE(pm.face(6).adjFace(2) == &pm.face(8));

        REQUIRE(pm.face(7).adjFacesNumber() == 3);
        REQUIRE(pm.face(7).adjFace(0) == &pm.face(6));
        REQUIRE(pm.face(7).adjFace(1) == &pm.face(3));
        REQUIRE(pm.face(7).adjFace(2) == &pm.face(5));

        REQUIRE(pm.face(8).adjFacesNumber() == 3);
        REQUIRE(pm.face(8).adjFace(0) == &pm.face(9));
        REQUIRE(pm.face(8).adjFace(1) == &pm.face(6));
        REQUIRE(pm.face(8).adjFace(2) == &pm.face(10));

        REQUIRE(pm.face(9).adjFacesNumber() == 3);
        REQUIRE(pm.face(9).adjFace(0) == &pm.face(8));
        REQUIRE(pm.face(9).adjFace(1) == &pm.face(1));
        REQUIRE(pm.face(9).adjFace(2) == &pm.face(3));

        REQUIRE(pm.face(10).adjFacesNumber() == 3);
        REQUIRE(pm.face(10).adjFace(0) == &pm.face(11));
        REQUIRE(pm.face(10).adjFace(1) == &pm.face(8));
        REQUIRE(pm.face(10).adjFace(2) == &pm.face(6));

        REQUIRE(pm.face(11).adjFacesNumber() == 3);
        REQUIRE(pm.face(11).adjFace(0) == &pm.face(10));
        REQUIRE(pm.face(11).adjFace(1) == &pm.face(5));
        REQUIRE(pm.face(11).adjFace(2) == &pm.face(1));
    }
}

TEMPLATE_TEST_CASE(
    "PolyMesh Polygon Mesh Topology",
    "",
    vcl::PolyMesh,
    vcl::PolyMeshf,
    vcl::PolyMeshIndexed,
    vcl::PolyMeshIndexedf)
{
    using PolyMesh = TestType;

    PolyMesh pm = vcl::loadPly<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    THEN("The mesh has 8 vertices, 6 faces")
    {
        REQUIRE(pm.vertexNumber() == 8);
        REQUIRE(pm.faceNumber() == 6);
    }

    THEN("Test Face Vertices")
    {
        REQUIRE(pm.face(0).vertexNumber() == 4);
        REQUIRE(pm.face(0).vertex(0) == &pm.vertex(2));
        REQUIRE(pm.face(0).vertex(1) == &pm.vertex(3));
        REQUIRE(pm.face(0).vertex(2) == &pm.vertex(1));
        REQUIRE(pm.face(0).vertex(3) == &pm.vertex(0));

        REQUIRE(pm.face(1).vertexNumber() == 4);
        REQUIRE(pm.face(1).vertex(0) == &pm.vertex(4));
        REQUIRE(pm.face(1).vertex(1) == &pm.vertex(6));
        REQUIRE(pm.face(1).vertex(2) == &pm.vertex(2));
        REQUIRE(pm.face(1).vertex(3) == &pm.vertex(0));

        REQUIRE(pm.face(2).vertexNumber() == 4);
        REQUIRE(pm.face(2).vertex(0) == &pm.vertex(1));
        REQUIRE(pm.face(2).vertex(1) == &pm.vertex(5));
        REQUIRE(pm.face(2).vertex(2) == &pm.vertex(4));
        REQUIRE(pm.face(2).vertex(3) == &pm.vertex(0));

        REQUIRE(pm.face(3).vertexNumber() == 4);
        REQUIRE(pm.face(3).vertex(0) == &pm.vertex(6));
        REQUIRE(pm.face(3).vertex(1) == &pm.vertex(4));
        REQUIRE(pm.face(3).vertex(2) == &pm.vertex(5));
        REQUIRE(pm.face(3).vertex(3) == &pm.vertex(7));

        REQUIRE(pm.face(4).vertexNumber() == 4);
        REQUIRE(pm.face(4).vertex(0) == &pm.vertex(3));
        REQUIRE(pm.face(4).vertex(1) == &pm.vertex(2));
        REQUIRE(pm.face(4).vertex(2) == &pm.vertex(6));
        REQUIRE(pm.face(4).vertex(3) == &pm.vertex(7));

        REQUIRE(pm.face(5).vertexNumber() == 4);
        REQUIRE(pm.face(5).vertex(0) == &pm.vertex(5));
        REQUIRE(pm.face(5).vertex(1) == &pm.vertex(1));
        REQUIRE(pm.face(5).vertex(2) == &pm.vertex(3));
        REQUIRE(pm.face(5).vertex(3) == &pm.vertex(7));
    }

    THEN("Test Per Vertex Adjacent Faces")
    {
        pm.enablePerVertexAdjacentFaces();
        vcl::updatePerVertexAdjacentFaces(pm);

        REQUIRE(pm.vertex(0).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(0).adjFace(0) == &pm.face(0));
        REQUIRE(pm.vertex(0).adjFace(1) == &pm.face(1));
        REQUIRE(pm.vertex(0).adjFace(2) == &pm.face(2));

        REQUIRE(pm.vertex(1).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(1).adjFace(0) == &pm.face(0));
        REQUIRE(pm.vertex(1).adjFace(1) == &pm.face(2));
        REQUIRE(pm.vertex(1).adjFace(2) == &pm.face(5));

        REQUIRE(pm.vertex(2).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(2).adjFace(0) == &pm.face(0));
        REQUIRE(pm.vertex(2).adjFace(1) == &pm.face(1));
        REQUIRE(pm.vertex(2).adjFace(2) == &pm.face(4));

        REQUIRE(pm.vertex(3).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(3).adjFace(0) == &pm.face(0));
        REQUIRE(pm.vertex(3).adjFace(1) == &pm.face(4));
        REQUIRE(pm.vertex(3).adjFace(2) == &pm.face(5));

        REQUIRE(pm.vertex(4).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(4).adjFace(0) == &pm.face(1));
        REQUIRE(pm.vertex(4).adjFace(1) == &pm.face(2));
        REQUIRE(pm.vertex(4).adjFace(2) == &pm.face(3));

        REQUIRE(pm.vertex(5).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(5).adjFace(0) == &pm.face(2));
        REQUIRE(pm.vertex(5).adjFace(1) == &pm.face(3));
        REQUIRE(pm.vertex(5).adjFace(2) == &pm.face(5));

        REQUIRE(pm.vertex(6).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(6).adjFace(0) == &pm.face(1));
        REQUIRE(pm.vertex(6).adjFace(1) == &pm.face(3));
        REQUIRE(pm.vertex(6).adjFace(2) == &pm.face(4));

        REQUIRE(pm.vertex(7).adjFacesNumber() == 3);
        REQUIRE(pm.vertex(7).adjFace(0) == &pm.face(3));
        REQUIRE(pm.vertex(7).adjFace(1) == &pm.face(4));
        REQUIRE(pm.vertex(7).adjFace(2) == &pm.face(5));
    }

    THEN("Test Per Vertex Adjacent Vertices")
    {
        pm.enablePerVertexAdjacentVertices();
        vcl::updatePerVertexAdjacentVertices(pm);

        REQUIRE(pm.vertex(0).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(0).adjVertex(0) == &pm.vertex(1));
        REQUIRE(pm.vertex(0).adjVertex(1) == &pm.vertex(2));
        REQUIRE(pm.vertex(0).adjVertex(2) == &pm.vertex(4));

        REQUIRE(pm.vertex(1).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(1).adjVertex(0) == &pm.vertex(0));
        REQUIRE(pm.vertex(1).adjVertex(1) == &pm.vertex(3));
        REQUIRE(pm.vertex(1).adjVertex(2) == &pm.vertex(5));

        REQUIRE(pm.vertex(2).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(2).adjVertex(0) == &pm.vertex(0));
        REQUIRE(pm.vertex(2).adjVertex(1) == &pm.vertex(3));
        REQUIRE(pm.vertex(2).adjVertex(2) == &pm.vertex(6));

        REQUIRE(pm.vertex(3).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(3).adjVertex(0) == &pm.vertex(1));
        REQUIRE(pm.vertex(3).adjVertex(1) == &pm.vertex(2));
        REQUIRE(pm.vertex(3).adjVertex(2) == &pm.vertex(7));

        REQUIRE(pm.vertex(4).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(4).adjVertex(0) == &pm.vertex(0));
        REQUIRE(pm.vertex(4).adjVertex(1) == &pm.vertex(5));
        REQUIRE(pm.vertex(4).adjVertex(2) == &pm.vertex(6));

        REQUIRE(pm.vertex(5).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(5).adjVertex(0) == &pm.vertex(1));
        REQUIRE(pm.vertex(5).adjVertex(1) == &pm.vertex(4));
        REQUIRE(pm.vertex(5).adjVertex(2) == &pm.vertex(7));

        REQUIRE(pm.vertex(6).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(6).adjVertex(0) == &pm.vertex(2));
        REQUIRE(pm.vertex(6).adjVertex(1) == &pm.vertex(4));
        REQUIRE(pm.vertex(6).adjVertex(2) == &pm.vertex(7));

        REQUIRE(pm.vertex(7).adjVerticesNumber() == 3);
        REQUIRE(pm.vertex(7).adjVertex(0) == &pm.vertex(3));
        REQUIRE(pm.vertex(7).adjVertex(1) == &pm.vertex(5));
        REQUIRE(pm.vertex(7).adjVertex(2) == &pm.vertex(6));
    }

    THEN("Test Per Face Adjacent Faces")
    {
        pm.enablePerFaceAdjacentFaces();
        vcl::updatePerFaceAdjacentFaces(pm);

        REQUIRE(pm.face(0).adjFacesNumber() == 4);
        REQUIRE(pm.face(0).adjFace(0) == &pm.face(4));
        REQUIRE(pm.face(0).adjFace(1) == &pm.face(5));
        REQUIRE(pm.face(0).adjFace(2) == &pm.face(2));
        REQUIRE(pm.face(0).adjFace(3) == &pm.face(1));

        REQUIRE(pm.face(1).adjFacesNumber() == 4);
        REQUIRE(pm.face(1).adjFace(0) == &pm.face(3));
        REQUIRE(pm.face(1).adjFace(1) == &pm.face(4));
        REQUIRE(pm.face(1).adjFace(2) == &pm.face(0));
        REQUIRE(pm.face(1).adjFace(3) == &pm.face(2));

        REQUIRE(pm.face(2).adjFacesNumber() == 4);
        REQUIRE(pm.face(2).adjFace(0) == &pm.face(5));
        REQUIRE(pm.face(2).adjFace(1) == &pm.face(3));
        REQUIRE(pm.face(2).adjFace(2) == &pm.face(1));
        REQUIRE(pm.face(2).adjFace(3) == &pm.face(0));

        REQUIRE(pm.face(3).adjFacesNumber() == 4);
        REQUIRE(pm.face(3).adjFace(0) == &pm.face(1));
        REQUIRE(pm.face(3).adjFace(1) == &pm.face(2));
        REQUIRE(pm.face(3).adjFace(2) == &pm.face(5));
        REQUIRE(pm.face(3).adjFace(3) == &pm.face(4));

        REQUIRE(pm.face(4).adjFacesNumber() == 4);
        REQUIRE(pm.face(4).adjFace(0) == &pm.face(0));
        REQUIRE(pm.face(4).adjFace(1) == &pm.face(1));
        REQUIRE(pm.face(4).adjFace(2) == &pm.face(3));
        REQUIRE(pm.face(4).adjFace(3) == &pm.face(5));

        REQUIRE(pm.face(5).adjFacesNumber() == 4);
        REQUIRE(pm.face(5).adjFace(0) == &pm.face(2));
        REQUIRE(pm.face(5).adjFace(1) == &pm.face(0));
        REQUIRE(pm.face(5).adjFace(2) == &pm.face(4));
        REQUIRE(pm.face(5).adjFace(3) == &pm.face(3));
    }
}
