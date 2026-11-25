// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/io/mesh/load_mesh.h>
#include <vclib/meshes/tri_mesh.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "TriMesh Topology",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMesh64,
    vcl::TriMesh64f)
{
    using TriMesh = TestType;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    THEN("The mesh has 8 vertices, 12 triangles")
    {
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);
    }

    THEN("Test Face Vertices")
    {
        REQUIRE(tm.face(0).vertexCount() == 3);
        REQUIRE(tm.face(0).vertex(0) == tm.vertex(2));
        REQUIRE(tm.face(0).vertex(1) == tm.vertex(1));
        REQUIRE(tm.face(0).vertex(2) == tm.vertex(0));

        REQUIRE(tm.face(1).vertexCount() == 3);
        REQUIRE(tm.face(1).vertex(0) == tm.vertex(1));
        REQUIRE(tm.face(1).vertex(1) == tm.vertex(2));
        REQUIRE(tm.face(1).vertex(2) == tm.vertex(3));

        REQUIRE(tm.face(2).vertexCount() == 3);
        REQUIRE(tm.face(2).vertex(0) == tm.vertex(4));
        REQUIRE(tm.face(2).vertex(1) == tm.vertex(2));
        REQUIRE(tm.face(2).vertex(2) == tm.vertex(0));

        REQUIRE(tm.face(3).vertexCount() == 3);
        REQUIRE(tm.face(3).vertex(0) == tm.vertex(2));
        REQUIRE(tm.face(3).vertex(1) == tm.vertex(4));
        REQUIRE(tm.face(3).vertex(2) == tm.vertex(6));

        REQUIRE(tm.face(4).vertexCount() == 3);
        REQUIRE(tm.face(4).vertex(0) == tm.vertex(1));
        REQUIRE(tm.face(4).vertex(1) == tm.vertex(4));
        REQUIRE(tm.face(4).vertex(2) == tm.vertex(0));

        REQUIRE(tm.face(5).vertexCount() == 3);
        REQUIRE(tm.face(5).vertex(0) == tm.vertex(4));
        REQUIRE(tm.face(5).vertex(1) == tm.vertex(1));
        REQUIRE(tm.face(5).vertex(2) == tm.vertex(5));

        REQUIRE(tm.face(6).vertexCount() == 3);
        REQUIRE(tm.face(6).vertex(0) == tm.vertex(6));
        REQUIRE(tm.face(6).vertex(1) == tm.vertex(5));
        REQUIRE(tm.face(6).vertex(2) == tm.vertex(7));

        REQUIRE(tm.face(7).vertexCount() == 3);
        REQUIRE(tm.face(7).vertex(0) == tm.vertex(5));
        REQUIRE(tm.face(7).vertex(1) == tm.vertex(6));
        REQUIRE(tm.face(7).vertex(2) == tm.vertex(4));

        REQUIRE(tm.face(8).vertexCount() == 3);
        REQUIRE(tm.face(8).vertex(0) == tm.vertex(3));
        REQUIRE(tm.face(8).vertex(1) == tm.vertex(6));
        REQUIRE(tm.face(8).vertex(2) == tm.vertex(7));

        REQUIRE(tm.face(9).vertexCount() == 3);
        REQUIRE(tm.face(9).vertex(0) == tm.vertex(6));
        REQUIRE(tm.face(9).vertex(1) == tm.vertex(3));
        REQUIRE(tm.face(9).vertex(2) == tm.vertex(2));

        REQUIRE(tm.face(10).vertexCount() == 3);
        REQUIRE(tm.face(10).vertex(0) == tm.vertex(5));
        REQUIRE(tm.face(10).vertex(1) == tm.vertex(3));
        REQUIRE(tm.face(10).vertex(2) == tm.vertex(7));

        REQUIRE(tm.face(11).vertexCount() == 3);
        REQUIRE(tm.face(11).vertex(0) == tm.vertex(3));
        REQUIRE(tm.face(11).vertex(1) == tm.vertex(5));
        REQUIRE(tm.face(11).vertex(2) == tm.vertex(1));
    }

    THEN("Test Per Vertex Adjacent Faces")
    {
        tm.enableVertexAdjacentFaces();
        vcl::updateVertexAdjacentFaces(tm);

        REQUIRE(tm.vertex(0).adjFaceCount() == 3);
        REQUIRE(tm.vertex(0).adjFace(0) == tm.face(0));
        REQUIRE(tm.vertex(0).adjFace(1) == tm.face(2));
        REQUIRE(tm.vertex(0).adjFace(2) == tm.face(4));

        REQUIRE(tm.vertex(1).adjFaceCount() == 5);
        REQUIRE(tm.vertex(1).adjFace(0) == tm.face(0));
        REQUIRE(tm.vertex(1).adjFace(1) == tm.face(1));
        REQUIRE(tm.vertex(1).adjFace(2) == tm.face(4));
        REQUIRE(tm.vertex(1).adjFace(3) == tm.face(5));
        REQUIRE(tm.vertex(1).adjFace(4) == tm.face(11));

        REQUIRE(tm.vertex(2).adjFaceCount() == 5);
        REQUIRE(tm.vertex(2).adjFace(0) == tm.face(0));
        REQUIRE(tm.vertex(2).adjFace(1) == tm.face(1));
        REQUIRE(tm.vertex(2).adjFace(2) == tm.face(2));
        REQUIRE(tm.vertex(2).adjFace(3) == tm.face(3));
        REQUIRE(tm.vertex(2).adjFace(4) == tm.face(9));

        REQUIRE(tm.vertex(3).adjFaceCount() == 5);
        REQUIRE(tm.vertex(3).adjFace(0) == tm.face(1));
        REQUIRE(tm.vertex(3).adjFace(1) == tm.face(8));
        REQUIRE(tm.vertex(3).adjFace(2) == tm.face(9));
        REQUIRE(tm.vertex(3).adjFace(3) == tm.face(10));
        REQUIRE(tm.vertex(3).adjFace(4) == tm.face(11));

        REQUIRE(tm.vertex(4).adjFaceCount() == 5);
        REQUIRE(tm.vertex(4).adjFace(0) == tm.face(2));
        REQUIRE(tm.vertex(4).adjFace(1) == tm.face(3));
        REQUIRE(tm.vertex(4).adjFace(2) == tm.face(4));
        REQUIRE(tm.vertex(4).adjFace(3) == tm.face(5));
        REQUIRE(tm.vertex(4).adjFace(4) == tm.face(7));

        REQUIRE(tm.vertex(5).adjFaceCount() == 5);
        REQUIRE(tm.vertex(5).adjFace(0) == tm.face(5));
        REQUIRE(tm.vertex(5).adjFace(1) == tm.face(6));
        REQUIRE(tm.vertex(5).adjFace(2) == tm.face(7));
        REQUIRE(tm.vertex(5).adjFace(3) == tm.face(10));
        REQUIRE(tm.vertex(5).adjFace(4) == tm.face(11));

        REQUIRE(tm.vertex(6).adjFaceCount() == 5);
        REQUIRE(tm.vertex(6).adjFace(0) == tm.face(3));
        REQUIRE(tm.vertex(6).adjFace(1) == tm.face(6));
        REQUIRE(tm.vertex(6).adjFace(2) == tm.face(7));
        REQUIRE(tm.vertex(6).adjFace(3) == tm.face(8));
        REQUIRE(tm.vertex(6).adjFace(4) == tm.face(9));

        REQUIRE(tm.vertex(7).adjFaceCount() == 3);
        REQUIRE(tm.vertex(7).adjFace(0) == tm.face(6));
        REQUIRE(tm.vertex(7).adjFace(1) == tm.face(8));
        REQUIRE(tm.vertex(7).adjFace(2) == tm.face(10));
    }

    THEN("Test Per Vertex Adjacent Vertices")
    {
        tm.enableVertexAdjacentVertices();
        vcl::updateVertexAdjacentVertices(tm);

        REQUIRE(tm.vertex(0).adjVertexCount() == 3);
        REQUIRE(tm.vertex(0).adjVertex(0) == tm.vertex(1));
        REQUIRE(tm.vertex(0).adjVertex(1) == tm.vertex(2));
        REQUIRE(tm.vertex(0).adjVertex(2) == tm.vertex(4));

        REQUIRE(tm.vertex(1).adjVertexCount() == 5);
        REQUIRE(tm.vertex(1).adjVertex(0) == tm.vertex(0));
        REQUIRE(tm.vertex(1).adjVertex(1) == tm.vertex(2));
        REQUIRE(tm.vertex(1).adjVertex(2) == tm.vertex(3));
        REQUIRE(tm.vertex(1).adjVertex(3) == tm.vertex(4));
        REQUIRE(tm.vertex(1).adjVertex(4) == tm.vertex(5));

        REQUIRE(tm.vertex(2).adjVertexCount() == 5);
        REQUIRE(tm.vertex(2).adjVertex(0) == tm.vertex(0));
        REQUIRE(tm.vertex(2).adjVertex(1) == tm.vertex(1));
        REQUIRE(tm.vertex(2).adjVertex(2) == tm.vertex(3));
        REQUIRE(tm.vertex(2).adjVertex(3) == tm.vertex(4));
        REQUIRE(tm.vertex(2).adjVertex(4) == tm.vertex(6));

        REQUIRE(tm.vertex(3).adjVertexCount() == 5);
        REQUIRE(tm.vertex(3).adjVertex(0) == tm.vertex(1));
        REQUIRE(tm.vertex(3).adjVertex(1) == tm.vertex(2));
        REQUIRE(tm.vertex(3).adjVertex(2) == tm.vertex(5));
        REQUIRE(tm.vertex(3).adjVertex(3) == tm.vertex(6));
        REQUIRE(tm.vertex(3).adjVertex(4) == tm.vertex(7));

        REQUIRE(tm.vertex(4).adjVertexCount() == 5);
        REQUIRE(tm.vertex(4).adjVertex(0) == tm.vertex(0));
        REQUIRE(tm.vertex(4).adjVertex(1) == tm.vertex(1));
        REQUIRE(tm.vertex(4).adjVertex(2) == tm.vertex(2));
        REQUIRE(tm.vertex(4).adjVertex(3) == tm.vertex(5));
        REQUIRE(tm.vertex(4).adjVertex(4) == tm.vertex(6));

        REQUIRE(tm.vertex(5).adjVertexCount() == 5);
        REQUIRE(tm.vertex(5).adjVertex(0) == tm.vertex(1));
        REQUIRE(tm.vertex(5).adjVertex(1) == tm.vertex(3));
        REQUIRE(tm.vertex(5).adjVertex(2) == tm.vertex(4));
        REQUIRE(tm.vertex(5).adjVertex(3) == tm.vertex(6));
        REQUIRE(tm.vertex(5).adjVertex(4) == tm.vertex(7));

        REQUIRE(tm.vertex(6).adjVertexCount() == 5);
        REQUIRE(tm.vertex(6).adjVertex(0) == tm.vertex(2));
        REQUIRE(tm.vertex(6).adjVertex(1) == tm.vertex(3));
        REQUIRE(tm.vertex(6).adjVertex(2) == tm.vertex(4));
        REQUIRE(tm.vertex(6).adjVertex(3) == tm.vertex(5));
        REQUIRE(tm.vertex(6).adjVertex(4) == tm.vertex(7));

        REQUIRE(tm.vertex(7).adjVertexCount() == 3);
        REQUIRE(tm.vertex(7).adjVertex(0) == tm.vertex(3));
        REQUIRE(tm.vertex(7).adjVertex(1) == tm.vertex(5));
        REQUIRE(tm.vertex(7).adjVertex(2) == tm.vertex(6));
    }

    THEN("Test Per Face Adjacent Faces")
    {
        tm.enableFaceAdjacentFaces();
        vcl::updateFaceAdjacentFaces(tm);

        REQUIRE(tm.face(0).adjFaceCount() == 3);
        REQUIRE(tm.face(0).adjFace(0) == tm.face(1));
        REQUIRE(tm.face(0).adjFace(1) == tm.face(4));
        REQUIRE(tm.face(0).adjFace(2) == tm.face(2));

        REQUIRE(tm.face(1).adjFaceCount() == 3);
        REQUIRE(tm.face(1).adjFace(0) == tm.face(0));
        REQUIRE(tm.face(1).adjFace(1) == tm.face(9));
        REQUIRE(tm.face(1).adjFace(2) == tm.face(11));

        REQUIRE(tm.face(2).adjFaceCount() == 3);
        REQUIRE(tm.face(2).adjFace(0) == tm.face(3));
        REQUIRE(tm.face(2).adjFace(1) == tm.face(0));
        REQUIRE(tm.face(2).adjFace(2) == tm.face(4));

        REQUIRE(tm.face(3).adjFaceCount() == 3);
        REQUIRE(tm.face(3).adjFace(0) == tm.face(2));
        REQUIRE(tm.face(3).adjFace(1) == tm.face(7));
        REQUIRE(tm.face(3).adjFace(2) == tm.face(9));

        REQUIRE(tm.face(4).adjFaceCount() == 3);
        REQUIRE(tm.face(4).adjFace(0) == tm.face(5));
        REQUIRE(tm.face(4).adjFace(1) == tm.face(2));
        REQUIRE(tm.face(4).adjFace(2) == tm.face(0));

        REQUIRE(tm.face(5).adjFaceCount() == 3);
        REQUIRE(tm.face(5).adjFace(0) == tm.face(4));
        REQUIRE(tm.face(5).adjFace(1) == tm.face(11));
        REQUIRE(tm.face(5).adjFace(2) == tm.face(7));

        REQUIRE(tm.face(6).adjFaceCount() == 3);
        REQUIRE(tm.face(6).adjFace(0) == tm.face(7));
        REQUIRE(tm.face(6).adjFace(1) == tm.face(10));
        REQUIRE(tm.face(6).adjFace(2) == tm.face(8));

        REQUIRE(tm.face(7).adjFaceCount() == 3);
        REQUIRE(tm.face(7).adjFace(0) == tm.face(6));
        REQUIRE(tm.face(7).adjFace(1) == tm.face(3));
        REQUIRE(tm.face(7).adjFace(2) == tm.face(5));

        REQUIRE(tm.face(8).adjFaceCount() == 3);
        REQUIRE(tm.face(8).adjFace(0) == tm.face(9));
        REQUIRE(tm.face(8).adjFace(1) == tm.face(6));
        REQUIRE(tm.face(8).adjFace(2) == tm.face(10));

        REQUIRE(tm.face(9).adjFaceCount() == 3);
        REQUIRE(tm.face(9).adjFace(0) == tm.face(8));
        REQUIRE(tm.face(9).adjFace(1) == tm.face(1));
        REQUIRE(tm.face(9).adjFace(2) == tm.face(3));

        REQUIRE(tm.face(10).adjFaceCount() == 3);
        REQUIRE(tm.face(10).adjFace(0) == tm.face(11));
        REQUIRE(tm.face(10).adjFace(1) == tm.face(8));
        REQUIRE(tm.face(10).adjFace(2) == tm.face(6));

        REQUIRE(tm.face(11).adjFaceCount() == 3);
        REQUIRE(tm.face(11).adjFace(0) == tm.face(10));
        REQUIRE(tm.face(11).adjFace(1) == tm.face(5));
        REQUIRE(tm.face(11).adjFace(2) == tm.face(1));
    }
}

SCENARIO("Test updateFaceAdjacentFaces on mesh with boundary edges")
{
    GIVEN("A single-triangle mesh with boundary edges")
    {
        vcl::TriMesh tm;
        tm.addVertices(3);
        tm.vertex(0).position() = vcl::Point3d(0, 0, 0);
        tm.vertex(1).position() = vcl::Point3d(1, 0, 0);
        tm.vertex(2).position() = vcl::Point3d(0, 1, 0);

        tm.addFace(0, 1, 2);

        tm.enableFaceAdjacentFaces();
        // Initialize with non-null values to verify they are all set to
        // NULL_INDEX
        tm.face(0).setAdjFace(0, 0);
        tm.face(0).setAdjFace(1, 0);
        tm.face(0).setAdjFace(2, 0);

        vcl::updateFaceAdjacentFaces(tm);

        REQUIRE(tm.face(0).adjFaceIndex(0) == vcl::TriMesh::NULL_INDEX);
        REQUIRE(tm.face(0).adjFaceIndex(1) == vcl::TriMesh::NULL_INDEX);
        REQUIRE(tm.face(0).adjFaceIndex(2) == vcl::TriMesh::NULL_INDEX);
    }

    GIVEN("A two-triangle mesh sharing one edge")
    {
        vcl::TriMesh tm;
        tm.addVertices(4);
        tm.vertex(0).position() = vcl::Point3d(0, 0, 0);
        tm.vertex(1).position() = vcl::Point3d(1, 0, 0);
        tm.vertex(2).position() = vcl::Point3d(1, 1, 0);
        tm.vertex(3).position() = vcl::Point3d(0, 1, 0);

        tm.addFace(0, 1, 2);
        tm.addFace(0, 2, 3);

        tm.enableFaceAdjacentFaces();
        vcl::updateFaceAdjacentFaces(tm);

        // Edge (0, 2) is shared between face 0 (edge index 2) and face 1 (edge
        // index 0)
        REQUIRE(tm.face(0).adjFaceIndex(2) == 1);
        REQUIRE(tm.face(1).adjFaceIndex(0) == 0);

        // All boundary edges (including the last in the sorted list) must be
        // NULL_INDEX
        REQUIRE(tm.face(0).adjFaceIndex(0) == vcl::TriMesh::NULL_INDEX);
        REQUIRE(tm.face(0).adjFaceIndex(1) == vcl::TriMesh::NULL_INDEX);
        REQUIRE(tm.face(1).adjFaceIndex(1) == vcl::TriMesh::NULL_INDEX);
        REQUIRE(tm.face(1).adjFaceIndex(2) == vcl::TriMesh::NULL_INDEX);
    }
}
