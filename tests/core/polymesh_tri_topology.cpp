// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/io/mesh/load_mesh.h>
#include <vclib/meshes/poly_mesh.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "PolyMesh Triangle Mesh Topology",
    "",
    vcl::PolyMesh,
    vcl::PolyMeshf)
{
    using TriMesh = TestType;

    TriMesh pm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    THEN("The mesh has 8 vertices, 12 triangles")
    {
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.faceCount() == 12);
    }

    THEN("Test Face Vertices")
    {
        REQUIRE(pm.face(0).vertexCount() == 3);
        REQUIRE(pm.face(0).vertex(0) == pm.vertex(2));
        REQUIRE(pm.face(0).vertex(1) == pm.vertex(1));
        REQUIRE(pm.face(0).vertex(2) == pm.vertex(0));

        REQUIRE(pm.face(1).vertexCount() == 3);
        REQUIRE(pm.face(1).vertex(0) == pm.vertex(1));
        REQUIRE(pm.face(1).vertex(1) == pm.vertex(2));
        REQUIRE(pm.face(1).vertex(2) == pm.vertex(3));

        REQUIRE(pm.face(2).vertexCount() == 3);
        REQUIRE(pm.face(2).vertex(0) == pm.vertex(4));
        REQUIRE(pm.face(2).vertex(1) == pm.vertex(2));
        REQUIRE(pm.face(2).vertex(2) == pm.vertex(0));

        REQUIRE(pm.face(3).vertexCount() == 3);
        REQUIRE(pm.face(3).vertex(0) == pm.vertex(2));
        REQUIRE(pm.face(3).vertex(1) == pm.vertex(4));
        REQUIRE(pm.face(3).vertex(2) == pm.vertex(6));

        REQUIRE(pm.face(4).vertexCount() == 3);
        REQUIRE(pm.face(4).vertex(0) == pm.vertex(1));
        REQUIRE(pm.face(4).vertex(1) == pm.vertex(4));
        REQUIRE(pm.face(4).vertex(2) == pm.vertex(0));

        REQUIRE(pm.face(5).vertexCount() == 3);
        REQUIRE(pm.face(5).vertex(0) == pm.vertex(4));
        REQUIRE(pm.face(5).vertex(1) == pm.vertex(1));
        REQUIRE(pm.face(5).vertex(2) == pm.vertex(5));

        REQUIRE(pm.face(6).vertexCount() == 3);
        REQUIRE(pm.face(6).vertex(0) == pm.vertex(6));
        REQUIRE(pm.face(6).vertex(1) == pm.vertex(5));
        REQUIRE(pm.face(6).vertex(2) == pm.vertex(7));

        REQUIRE(pm.face(7).vertexCount() == 3);
        REQUIRE(pm.face(7).vertex(0) == pm.vertex(5));
        REQUIRE(pm.face(7).vertex(1) == pm.vertex(6));
        REQUIRE(pm.face(7).vertex(2) == pm.vertex(4));

        REQUIRE(pm.face(8).vertexCount() == 3);
        REQUIRE(pm.face(8).vertex(0) == pm.vertex(3));
        REQUIRE(pm.face(8).vertex(1) == pm.vertex(6));
        REQUIRE(pm.face(8).vertex(2) == pm.vertex(7));

        REQUIRE(pm.face(9).vertexCount() == 3);
        REQUIRE(pm.face(9).vertex(0) == pm.vertex(6));
        REQUIRE(pm.face(9).vertex(1) == pm.vertex(3));
        REQUIRE(pm.face(9).vertex(2) == pm.vertex(2));

        REQUIRE(pm.face(10).vertexCount() == 3);
        REQUIRE(pm.face(10).vertex(0) == pm.vertex(5));
        REQUIRE(pm.face(10).vertex(1) == pm.vertex(3));
        REQUIRE(pm.face(10).vertex(2) == pm.vertex(7));

        REQUIRE(pm.face(11).vertexCount() == 3);
        REQUIRE(pm.face(11).vertex(0) == pm.vertex(3));
        REQUIRE(pm.face(11).vertex(1) == pm.vertex(5));
        REQUIRE(pm.face(11).vertex(2) == pm.vertex(1));
    }

    THEN("Test Per Vertex Adjacent Faces")
    {
        pm.enableVertexAdjacentFaces();
        vcl::updateVertexAdjacentFaces(pm);

        REQUIRE(pm.vertex(0).adjFaceCount() == 3);
        REQUIRE(pm.vertex(0).adjFace(0) == pm.face(0));
        REQUIRE(pm.vertex(0).adjFace(1) == pm.face(2));
        REQUIRE(pm.vertex(0).adjFace(2) == pm.face(4));

        REQUIRE(pm.vertex(1).adjFaceCount() == 5);
        REQUIRE(pm.vertex(1).adjFace(0) == pm.face(0));
        REQUIRE(pm.vertex(1).adjFace(1) == pm.face(1));
        REQUIRE(pm.vertex(1).adjFace(2) == pm.face(4));
        REQUIRE(pm.vertex(1).adjFace(3) == pm.face(5));
        REQUIRE(pm.vertex(1).adjFace(4) == pm.face(11));

        REQUIRE(pm.vertex(2).adjFaceCount() == 5);
        REQUIRE(pm.vertex(2).adjFace(0) == pm.face(0));
        REQUIRE(pm.vertex(2).adjFace(1) == pm.face(1));
        REQUIRE(pm.vertex(2).adjFace(2) == pm.face(2));
        REQUIRE(pm.vertex(2).adjFace(3) == pm.face(3));
        REQUIRE(pm.vertex(2).adjFace(4) == pm.face(9));

        REQUIRE(pm.vertex(3).adjFaceCount() == 5);
        REQUIRE(pm.vertex(3).adjFace(0) == pm.face(1));
        REQUIRE(pm.vertex(3).adjFace(1) == pm.face(8));
        REQUIRE(pm.vertex(3).adjFace(2) == pm.face(9));
        REQUIRE(pm.vertex(3).adjFace(3) == pm.face(10));
        REQUIRE(pm.vertex(3).adjFace(4) == pm.face(11));

        REQUIRE(pm.vertex(4).adjFaceCount() == 5);
        REQUIRE(pm.vertex(4).adjFace(0) == pm.face(2));
        REQUIRE(pm.vertex(4).adjFace(1) == pm.face(3));
        REQUIRE(pm.vertex(4).adjFace(2) == pm.face(4));
        REQUIRE(pm.vertex(4).adjFace(3) == pm.face(5));
        REQUIRE(pm.vertex(4).adjFace(4) == pm.face(7));

        REQUIRE(pm.vertex(5).adjFaceCount() == 5);
        REQUIRE(pm.vertex(5).adjFace(0) == pm.face(5));
        REQUIRE(pm.vertex(5).adjFace(1) == pm.face(6));
        REQUIRE(pm.vertex(5).adjFace(2) == pm.face(7));
        REQUIRE(pm.vertex(5).adjFace(3) == pm.face(10));
        REQUIRE(pm.vertex(5).adjFace(4) == pm.face(11));

        REQUIRE(pm.vertex(6).adjFaceCount() == 5);
        REQUIRE(pm.vertex(6).adjFace(0) == pm.face(3));
        REQUIRE(pm.vertex(6).adjFace(1) == pm.face(6));
        REQUIRE(pm.vertex(6).adjFace(2) == pm.face(7));
        REQUIRE(pm.vertex(6).adjFace(3) == pm.face(8));
        REQUIRE(pm.vertex(6).adjFace(4) == pm.face(9));

        REQUIRE(pm.vertex(7).adjFaceCount() == 3);
        REQUIRE(pm.vertex(7).adjFace(0) == pm.face(6));
        REQUIRE(pm.vertex(7).adjFace(1) == pm.face(8));
        REQUIRE(pm.vertex(7).adjFace(2) == pm.face(10));
    }

    THEN("Test Per Vertex Adjacent Vertices")
    {
        pm.enableVertexAdjacentVertices();
        vcl::updateVertexAdjacentVertices(pm);

        REQUIRE(pm.vertex(0).adjVertexCount() == 3);
        REQUIRE(pm.vertex(0).adjVertex(0) == pm.vertex(1));
        REQUIRE(pm.vertex(0).adjVertex(1) == pm.vertex(2));
        REQUIRE(pm.vertex(0).adjVertex(2) == pm.vertex(4));

        REQUIRE(pm.vertex(1).adjVertexCount() == 5);
        REQUIRE(pm.vertex(1).adjVertex(0) == pm.vertex(0));
        REQUIRE(pm.vertex(1).adjVertex(1) == pm.vertex(2));
        REQUIRE(pm.vertex(1).adjVertex(2) == pm.vertex(3));
        REQUIRE(pm.vertex(1).adjVertex(3) == pm.vertex(4));
        REQUIRE(pm.vertex(1).adjVertex(4) == pm.vertex(5));

        REQUIRE(pm.vertex(2).adjVertexCount() == 5);
        REQUIRE(pm.vertex(2).adjVertex(0) == pm.vertex(0));
        REQUIRE(pm.vertex(2).adjVertex(1) == pm.vertex(1));
        REQUIRE(pm.vertex(2).adjVertex(2) == pm.vertex(3));
        REQUIRE(pm.vertex(2).adjVertex(3) == pm.vertex(4));
        REQUIRE(pm.vertex(2).adjVertex(4) == pm.vertex(6));

        REQUIRE(pm.vertex(3).adjVertexCount() == 5);
        REQUIRE(pm.vertex(3).adjVertex(0) == pm.vertex(1));
        REQUIRE(pm.vertex(3).adjVertex(1) == pm.vertex(2));
        REQUIRE(pm.vertex(3).adjVertex(2) == pm.vertex(5));
        REQUIRE(pm.vertex(3).adjVertex(3) == pm.vertex(6));
        REQUIRE(pm.vertex(3).adjVertex(4) == pm.vertex(7));

        REQUIRE(pm.vertex(4).adjVertexCount() == 5);
        REQUIRE(pm.vertex(4).adjVertex(0) == pm.vertex(0));
        REQUIRE(pm.vertex(4).adjVertex(1) == pm.vertex(1));
        REQUIRE(pm.vertex(4).adjVertex(2) == pm.vertex(2));
        REQUIRE(pm.vertex(4).adjVertex(3) == pm.vertex(5));
        REQUIRE(pm.vertex(4).adjVertex(4) == pm.vertex(6));

        REQUIRE(pm.vertex(5).adjVertexCount() == 5);
        REQUIRE(pm.vertex(5).adjVertex(0) == pm.vertex(1));
        REQUIRE(pm.vertex(5).adjVertex(1) == pm.vertex(3));
        REQUIRE(pm.vertex(5).adjVertex(2) == pm.vertex(4));
        REQUIRE(pm.vertex(5).adjVertex(3) == pm.vertex(6));
        REQUIRE(pm.vertex(5).adjVertex(4) == pm.vertex(7));

        REQUIRE(pm.vertex(6).adjVertexCount() == 5);
        REQUIRE(pm.vertex(6).adjVertex(0) == pm.vertex(2));
        REQUIRE(pm.vertex(6).adjVertex(1) == pm.vertex(3));
        REQUIRE(pm.vertex(6).adjVertex(2) == pm.vertex(4));
        REQUIRE(pm.vertex(6).adjVertex(3) == pm.vertex(5));
        REQUIRE(pm.vertex(6).adjVertex(4) == pm.vertex(7));

        REQUIRE(pm.vertex(7).adjVertexCount() == 3);
        REQUIRE(pm.vertex(7).adjVertex(0) == pm.vertex(3));
        REQUIRE(pm.vertex(7).adjVertex(1) == pm.vertex(5));
        REQUIRE(pm.vertex(7).adjVertex(2) == pm.vertex(6));
    }

    THEN("Test Per Face Adjacent Faces")
    {
        pm.enableFaceAdjacentFaces();
        vcl::updateFaceAdjacentFaces(pm);

        REQUIRE(pm.face(0).adjFaceCount() == 3);
        REQUIRE(pm.face(0).adjFace(0) == pm.face(1));
        REQUIRE(pm.face(0).adjFace(1) == pm.face(4));
        REQUIRE(pm.face(0).adjFace(2) == pm.face(2));

        REQUIRE(pm.face(1).adjFaceCount() == 3);
        REQUIRE(pm.face(1).adjFace(0) == pm.face(0));
        REQUIRE(pm.face(1).adjFace(1) == pm.face(9));
        REQUIRE(pm.face(1).adjFace(2) == pm.face(11));

        REQUIRE(pm.face(2).adjFaceCount() == 3);
        REQUIRE(pm.face(2).adjFace(0) == pm.face(3));
        REQUIRE(pm.face(2).adjFace(1) == pm.face(0));
        REQUIRE(pm.face(2).adjFace(2) == pm.face(4));

        REQUIRE(pm.face(3).adjFaceCount() == 3);
        REQUIRE(pm.face(3).adjFace(0) == pm.face(2));
        REQUIRE(pm.face(3).adjFace(1) == pm.face(7));
        REQUIRE(pm.face(3).adjFace(2) == pm.face(9));

        REQUIRE(pm.face(4).adjFaceCount() == 3);
        REQUIRE(pm.face(4).adjFace(0) == pm.face(5));
        REQUIRE(pm.face(4).adjFace(1) == pm.face(2));
        REQUIRE(pm.face(4).adjFace(2) == pm.face(0));

        REQUIRE(pm.face(5).adjFaceCount() == 3);
        REQUIRE(pm.face(5).adjFace(0) == pm.face(4));
        REQUIRE(pm.face(5).adjFace(1) == pm.face(11));
        REQUIRE(pm.face(5).adjFace(2) == pm.face(7));

        REQUIRE(pm.face(6).adjFaceCount() == 3);
        REQUIRE(pm.face(6).adjFace(0) == pm.face(7));
        REQUIRE(pm.face(6).adjFace(1) == pm.face(10));
        REQUIRE(pm.face(6).adjFace(2) == pm.face(8));

        REQUIRE(pm.face(7).adjFaceCount() == 3);
        REQUIRE(pm.face(7).adjFace(0) == pm.face(6));
        REQUIRE(pm.face(7).adjFace(1) == pm.face(3));
        REQUIRE(pm.face(7).adjFace(2) == pm.face(5));

        REQUIRE(pm.face(8).adjFaceCount() == 3);
        REQUIRE(pm.face(8).adjFace(0) == pm.face(9));
        REQUIRE(pm.face(8).adjFace(1) == pm.face(6));
        REQUIRE(pm.face(8).adjFace(2) == pm.face(10));

        REQUIRE(pm.face(9).adjFaceCount() == 3);
        REQUIRE(pm.face(9).adjFace(0) == pm.face(8));
        REQUIRE(pm.face(9).adjFace(1) == pm.face(1));
        REQUIRE(pm.face(9).adjFace(2) == pm.face(3));

        REQUIRE(pm.face(10).adjFaceCount() == 3);
        REQUIRE(pm.face(10).adjFace(0) == pm.face(11));
        REQUIRE(pm.face(10).adjFace(1) == pm.face(8));
        REQUIRE(pm.face(10).adjFace(2) == pm.face(6));

        REQUIRE(pm.face(11).adjFaceCount() == 3);
        REQUIRE(pm.face(11).adjFace(0) == pm.face(10));
        REQUIRE(pm.face(11).adjFace(1) == pm.face(5));
        REQUIRE(pm.face(11).adjFace(2) == pm.face(1));
    }
}
