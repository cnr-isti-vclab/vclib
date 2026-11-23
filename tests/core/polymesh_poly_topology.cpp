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
    "PolyMesh Polygon Mesh Topology",
    "",
    vcl::PolyMesh,
    vcl::PolyMeshf,
    vcl::PolyMesh64,
    vcl::PolyMesh64f)
{
    using PolyMesh = TestType;

    PolyMesh pm =
        vcl::loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    THEN("The mesh has 8 vertices, 6 faces")
    {
        REQUIRE(pm.vertexCount() == 8);
        REQUIRE(pm.faceCount() == 6);
    }

    THEN("Test Face Vertices")
    {
        REQUIRE(pm.face(0).vertexCount() == 4);
        REQUIRE(pm.face(0).vertex(0) == pm.vertex(2));
        REQUIRE(pm.face(0).vertex(1) == pm.vertex(3));
        REQUIRE(pm.face(0).vertex(2) == pm.vertex(1));
        REQUIRE(pm.face(0).vertex(3) == pm.vertex(0));

        REQUIRE(pm.face(1).vertexCount() == 4);
        REQUIRE(pm.face(1).vertex(0) == pm.vertex(4));
        REQUIRE(pm.face(1).vertex(1) == pm.vertex(6));
        REQUIRE(pm.face(1).vertex(2) == pm.vertex(2));
        REQUIRE(pm.face(1).vertex(3) == pm.vertex(0));

        REQUIRE(pm.face(2).vertexCount() == 4);
        REQUIRE(pm.face(2).vertex(0) == pm.vertex(1));
        REQUIRE(pm.face(2).vertex(1) == pm.vertex(5));
        REQUIRE(pm.face(2).vertex(2) == pm.vertex(4));
        REQUIRE(pm.face(2).vertex(3) == pm.vertex(0));

        REQUIRE(pm.face(3).vertexCount() == 4);
        REQUIRE(pm.face(3).vertex(0) == pm.vertex(6));
        REQUIRE(pm.face(3).vertex(1) == pm.vertex(4));
        REQUIRE(pm.face(3).vertex(2) == pm.vertex(5));
        REQUIRE(pm.face(3).vertex(3) == pm.vertex(7));

        REQUIRE(pm.face(4).vertexCount() == 4);
        REQUIRE(pm.face(4).vertex(0) == pm.vertex(3));
        REQUIRE(pm.face(4).vertex(1) == pm.vertex(2));
        REQUIRE(pm.face(4).vertex(2) == pm.vertex(6));
        REQUIRE(pm.face(4).vertex(3) == pm.vertex(7));

        REQUIRE(pm.face(5).vertexCount() == 4);
        REQUIRE(pm.face(5).vertex(0) == pm.vertex(5));
        REQUIRE(pm.face(5).vertex(1) == pm.vertex(1));
        REQUIRE(pm.face(5).vertex(2) == pm.vertex(3));
        REQUIRE(pm.face(5).vertex(3) == pm.vertex(7));
    }

    THEN("Test Per Vertex Adjacent Faces")
    {
        pm.enableVertexAdjacentFaces();
        vcl::updateVertexAdjacentFaces(pm);

        REQUIRE(pm.vertex(0).adjFaceCount() == 3);
        REQUIRE(pm.vertex(0).adjFace(0) == pm.face(0));
        REQUIRE(pm.vertex(0).adjFace(1) == pm.face(1));
        REQUIRE(pm.vertex(0).adjFace(2) == pm.face(2));

        REQUIRE(pm.vertex(1).adjFaceCount() == 3);
        REQUIRE(pm.vertex(1).adjFace(0) == pm.face(0));
        REQUIRE(pm.vertex(1).adjFace(1) == pm.face(2));
        REQUIRE(pm.vertex(1).adjFace(2) == pm.face(5));

        REQUIRE(pm.vertex(2).adjFaceCount() == 3);
        REQUIRE(pm.vertex(2).adjFace(0) == pm.face(0));
        REQUIRE(pm.vertex(2).adjFace(1) == pm.face(1));
        REQUIRE(pm.vertex(2).adjFace(2) == pm.face(4));

        REQUIRE(pm.vertex(3).adjFaceCount() == 3);
        REQUIRE(pm.vertex(3).adjFace(0) == pm.face(0));
        REQUIRE(pm.vertex(3).adjFace(1) == pm.face(4));
        REQUIRE(pm.vertex(3).adjFace(2) == pm.face(5));

        REQUIRE(pm.vertex(4).adjFaceCount() == 3);
        REQUIRE(pm.vertex(4).adjFace(0) == pm.face(1));
        REQUIRE(pm.vertex(4).adjFace(1) == pm.face(2));
        REQUIRE(pm.vertex(4).adjFace(2) == pm.face(3));

        REQUIRE(pm.vertex(5).adjFaceCount() == 3);
        REQUIRE(pm.vertex(5).adjFace(0) == pm.face(2));
        REQUIRE(pm.vertex(5).adjFace(1) == pm.face(3));
        REQUIRE(pm.vertex(5).adjFace(2) == pm.face(5));

        REQUIRE(pm.vertex(6).adjFaceCount() == 3);
        REQUIRE(pm.vertex(6).adjFace(0) == pm.face(1));
        REQUIRE(pm.vertex(6).adjFace(1) == pm.face(3));
        REQUIRE(pm.vertex(6).adjFace(2) == pm.face(4));

        REQUIRE(pm.vertex(7).adjFaceCount() == 3);
        REQUIRE(pm.vertex(7).adjFace(0) == pm.face(3));
        REQUIRE(pm.vertex(7).adjFace(1) == pm.face(4));
        REQUIRE(pm.vertex(7).adjFace(2) == pm.face(5));
    }

    THEN("Test Per Vertex Adjacent Vertices")
    {
        pm.enableVertexAdjacentVertices();
        vcl::updateVertexAdjacentVertices(pm);

        REQUIRE(pm.vertex(0).adjVertexCount() == 3);
        REQUIRE(pm.vertex(0).adjVertex(0) == pm.vertex(1));
        REQUIRE(pm.vertex(0).adjVertex(1) == pm.vertex(2));
        REQUIRE(pm.vertex(0).adjVertex(2) == pm.vertex(4));

        REQUIRE(pm.vertex(1).adjVertexCount() == 3);
        REQUIRE(pm.vertex(1).adjVertex(0) == pm.vertex(0));
        REQUIRE(pm.vertex(1).adjVertex(1) == pm.vertex(3));
        REQUIRE(pm.vertex(1).adjVertex(2) == pm.vertex(5));

        REQUIRE(pm.vertex(2).adjVertexCount() == 3);
        REQUIRE(pm.vertex(2).adjVertex(0) == pm.vertex(0));
        REQUIRE(pm.vertex(2).adjVertex(1) == pm.vertex(3));
        REQUIRE(pm.vertex(2).adjVertex(2) == pm.vertex(6));

        REQUIRE(pm.vertex(3).adjVertexCount() == 3);
        REQUIRE(pm.vertex(3).adjVertex(0) == pm.vertex(1));
        REQUIRE(pm.vertex(3).adjVertex(1) == pm.vertex(2));
        REQUIRE(pm.vertex(3).adjVertex(2) == pm.vertex(7));

        REQUIRE(pm.vertex(4).adjVertexCount() == 3);
        REQUIRE(pm.vertex(4).adjVertex(0) == pm.vertex(0));
        REQUIRE(pm.vertex(4).adjVertex(1) == pm.vertex(5));
        REQUIRE(pm.vertex(4).adjVertex(2) == pm.vertex(6));

        REQUIRE(pm.vertex(5).adjVertexCount() == 3);
        REQUIRE(pm.vertex(5).adjVertex(0) == pm.vertex(1));
        REQUIRE(pm.vertex(5).adjVertex(1) == pm.vertex(4));
        REQUIRE(pm.vertex(5).adjVertex(2) == pm.vertex(7));

        REQUIRE(pm.vertex(6).adjVertexCount() == 3);
        REQUIRE(pm.vertex(6).adjVertex(0) == pm.vertex(2));
        REQUIRE(pm.vertex(6).adjVertex(1) == pm.vertex(4));
        REQUIRE(pm.vertex(6).adjVertex(2) == pm.vertex(7));

        REQUIRE(pm.vertex(7).adjVertexCount() == 3);
        REQUIRE(pm.vertex(7).adjVertex(0) == pm.vertex(3));
        REQUIRE(pm.vertex(7).adjVertex(1) == pm.vertex(5));
        REQUIRE(pm.vertex(7).adjVertex(2) == pm.vertex(6));
    }

    THEN("Test Per Face Adjacent Faces")
    {
        pm.enableFaceAdjacentFaces();
        vcl::updateFaceAdjacentFaces(pm);

        REQUIRE(pm.face(0).adjFaceCount() == 4);
        REQUIRE(pm.face(0).adjFace(0) == pm.face(4));
        REQUIRE(pm.face(0).adjFace(1) == pm.face(5));
        REQUIRE(pm.face(0).adjFace(2) == pm.face(2));
        REQUIRE(pm.face(0).adjFace(3) == pm.face(1));

        REQUIRE(pm.face(1).adjFaceCount() == 4);
        REQUIRE(pm.face(1).adjFace(0) == pm.face(3));
        REQUIRE(pm.face(1).adjFace(1) == pm.face(4));
        REQUIRE(pm.face(1).adjFace(2) == pm.face(0));
        REQUIRE(pm.face(1).adjFace(3) == pm.face(2));

        REQUIRE(pm.face(2).adjFaceCount() == 4);
        REQUIRE(pm.face(2).adjFace(0) == pm.face(5));
        REQUIRE(pm.face(2).adjFace(1) == pm.face(3));
        REQUIRE(pm.face(2).adjFace(2) == pm.face(1));
        REQUIRE(pm.face(2).adjFace(3) == pm.face(0));

        REQUIRE(pm.face(3).adjFaceCount() == 4);
        REQUIRE(pm.face(3).adjFace(0) == pm.face(1));
        REQUIRE(pm.face(3).adjFace(1) == pm.face(2));
        REQUIRE(pm.face(3).adjFace(2) == pm.face(5));
        REQUIRE(pm.face(3).adjFace(3) == pm.face(4));

        REQUIRE(pm.face(4).adjFaceCount() == 4);
        REQUIRE(pm.face(4).adjFace(0) == pm.face(0));
        REQUIRE(pm.face(4).adjFace(1) == pm.face(1));
        REQUIRE(pm.face(4).adjFace(2) == pm.face(3));
        REQUIRE(pm.face(4).adjFace(3) == pm.face(5));

        REQUIRE(pm.face(5).adjFaceCount() == 4);
        REQUIRE(pm.face(5).adjFace(0) == pm.face(2));
        REQUIRE(pm.face(5).adjFace(1) == pm.face(0));
        REQUIRE(pm.face(5).adjFace(2) == pm.face(4));
        REQUIRE(pm.face(5).adjFace(3) == pm.face(3));
    }
}
