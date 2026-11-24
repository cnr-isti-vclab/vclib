// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/io/mesh/load_mesh.h>
#include <vclib/meshes/tri_edge_mesh.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEMPLATE_TEST_CASE(
    "TriEdgeMesh Topology",
    "",
    vcl::TriEdgeMesh,
    vcl::TriEdgeMeshf,
    vcl::TriEdgeMesh64,
    vcl::TriEdgeMesh64f)
{
    using TriEdgeMesh = TestType;

    TriEdgeMesh tm =
        vcl::loadMesh<TriEdgeMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    tm.addEdge(6u, 7u);
    tm.addEdge(5u, 7u);
    tm.addEdge(5u, 4u);
    tm.addEdge(4u, 6u);

    THEN("The mesh has 8 vertices, 12 triangles and 4 edges")
    {
        REQUIRE(tm.vertexCount() == 8);
        REQUIRE(tm.faceCount() == 12);
        REQUIRE(tm.edgeCount() == 4);
    }

    THEN("Test Per Vertex Adjacent Edges")
    {
        tm.enableVertexAdjacentEdges();
        vcl::updateVertexAdjacentEdges(tm);

        REQUIRE(tm.vertex(0).adjEdgeCount() == 0);
        REQUIRE(tm.vertex(1).adjEdgeCount() == 0);
        REQUIRE(tm.vertex(2).adjEdgeCount() == 0);
        REQUIRE(tm.vertex(3).adjEdgeCount() == 0);
        REQUIRE(tm.vertex(4).adjEdgeCount() == 2);
        REQUIRE(tm.vertex(4).adjEdge(0) == tm.edge(2));
        REQUIRE(tm.vertex(4).adjEdge(1) == tm.edge(3));
        REQUIRE(tm.vertex(5).adjEdgeCount() == 2);
        REQUIRE(tm.vertex(5).adjEdge(0) == tm.edge(1));
        REQUIRE(tm.vertex(5).adjEdge(1) == tm.edge(2));
        REQUIRE(tm.vertex(6).adjEdgeCount() == 2);
        REQUIRE(tm.vertex(6).adjEdge(0) == tm.edge(0));
        REQUIRE(tm.vertex(6).adjEdge(1) == tm.edge(3));
        REQUIRE(tm.vertex(7).adjEdgeCount() == 2);
        REQUIRE(tm.vertex(7).adjEdge(0) == tm.edge(0));
        REQUIRE(tm.vertex(7).adjEdge(1) == tm.edge(1));
    }

    THEN("Test Per Face Adjacent Edges")
    {
        tm.enableFaceAdjacentEdges();
        vcl::updateFaceAdjacentEdges(tm);

        REQUIRE(tm.face(0).adjEdgeCount() == 0);
        REQUIRE(tm.face(1).adjEdgeCount() == 0);
        REQUIRE(tm.face(2).adjEdgeCount() == 0);
        REQUIRE(tm.face(3).adjEdgeCount() == 1);
        REQUIRE(tm.face(3).adjEdge(0) == tm.edge(3));
        REQUIRE(tm.face(4).adjEdgeCount() == 0);
        REQUIRE(tm.face(5).adjEdgeCount() == 1);
        REQUIRE(tm.face(5).adjEdge(0) == tm.edge(2));
        REQUIRE(tm.face(6).adjEdgeCount() == 2);
        REQUIRE(tm.face(6).adjEdge(0) == tm.edge(0));
        REQUIRE(tm.face(6).adjEdge(1) == tm.edge(1));
        REQUIRE(tm.face(7).adjEdgeCount() == 2);
        REQUIRE(tm.face(7).adjEdge(0) == tm.edge(2));
        REQUIRE(tm.face(7).adjEdge(1) == tm.edge(3));
        REQUIRE(tm.face(8).adjEdgeCount() == 1);
        REQUIRE(tm.face(8).adjEdge(0) == tm.edge(0));
        REQUIRE(tm.face(9).adjEdgeCount() == 0);
        REQUIRE(tm.face(10).adjEdgeCount() == 1);
        REQUIRE(tm.face(10).adjEdge(0) == tm.edge(1));
        REQUIRE(tm.face(11).adjEdgeCount() == 0);
    }

    THEN("Test Per Edge Adjacent Faces")
    {
        tm.enableEdgeAdjacentFaces();
        vcl::updateEdgeAdjacentFaces(tm);

        REQUIRE(tm.edge(0).adjFaceCount() == 2);
        REQUIRE(tm.edge(0).containsAdjFace(tm.face(6)));
        REQUIRE(tm.edge(0).containsAdjFace(tm.face(8)));
        REQUIRE(tm.edge(1).adjFaceCount() == 2);
        REQUIRE(tm.edge(1).containsAdjFace(tm.face(6)));
        REQUIRE(tm.edge(1).containsAdjFace(tm.face(10)));
        REQUIRE(tm.edge(2).adjFaceCount() == 2);
        REQUIRE(tm.edge(2).containsAdjFace(tm.face(5)));
        REQUIRE(tm.edge(2).containsAdjFace(tm.face(7)));
        REQUIRE(tm.edge(3).adjFaceCount() == 2);
        REQUIRE(tm.edge(3).containsAdjFace(tm.face(3)));
        REQUIRE(tm.edge(3).containsAdjFace(tm.face(7)));
    }

    THEN("Test Per Edge Adjacent Edges")
    {
        tm.enableEdgeAdjacentEdges();
        vcl::updateEdgeAdjacentEdges(tm);

        REQUIRE(tm.edge(0).adjEdgeCount() == 2);
        REQUIRE(tm.edge(0).containsAdjEdge(tm.edge(3)));
        REQUIRE(tm.edge(0).containsAdjEdge(tm.edge(1)));
        REQUIRE(tm.edge(1).adjEdgeCount() == 2);
        REQUIRE(tm.edge(1).containsAdjEdge(tm.edge(2)));
        REQUIRE(tm.edge(1).containsAdjEdge(tm.edge(0)));
        REQUIRE(tm.edge(2).adjEdgeCount() == 2);
        REQUIRE(tm.edge(2).containsAdjEdge(tm.edge(1)));
        REQUIRE(tm.edge(2).containsAdjEdge(tm.edge(3)));
        REQUIRE(tm.edge(3).adjEdgeCount() == 2);
        REQUIRE(tm.edge(3).containsAdjEdge(tm.edge(0)));
        REQUIRE(tm.edge(3).containsAdjEdge(tm.edge(2)));
    }
}
