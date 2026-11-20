// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/algorithms/mesh.h>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>
#include <vector>

TEST_CASE("Delete Algorithms - Delete Vertices If Function")
{
    // Quad mesh has 4 vertices:
    // v0: (0, 0, 1), v1: (1, 0, 0), v2: (0, 1, 0), v3: (1, 1, 0)
    // Face 0: (0, 1, 2), Face 1: (1, 3, 2)
    MyMesh mesh = createSampleQuadMesh();

    // Delete vertices with x > 0.5 (vertices 1 and 3)
    auto count = vcl::deleteVerticesIf(
        mesh, [](auto v) { return v.position().x() > 0.5f; });

    static_assert(std::is_same_v<decltype(count), MyMesh::IndexType>);
    REQUIRE(count == 2);
    REQUIRE(mesh.vertexCount() == 2);
    REQUIRE(mesh.deletedVertexCount() == 2);
    REQUIRE(mesh.vertex(1).deleted());
    REQUIRE(mesh.vertex(3).deleted());
    REQUIRE(!mesh.vertex(0).deleted());
    REQUIRE(!mesh.vertex(2).deleted());

    // Check that face vertex references to deleted vertices are invalidated
    // (NULL_INDEX)
    REQUIRE(mesh.face(0).vertexIndex(0) == 0);
    REQUIRE(mesh.face(0).vertexIndex(1) == MyMesh::NULL_INDEX);
    REQUIRE(mesh.face(0).vertexIndex(2) == 2);

    REQUIRE(mesh.face(1).vertexIndex(0) == MyMesh::NULL_INDEX);
    REQUIRE(mesh.face(1).vertexIndex(1) == MyMesh::NULL_INDEX);
    REQUIRE(mesh.face(1).vertexIndex(2) == 2);
}

TEST_CASE("Delete Algorithms - Delete Vertices If Range")
{
    MyMesh mesh = createSampleQuadMesh();

    // Range of booleans for valid vertices (one entry per valid vertex: 0, 1,
    // 2, 3)
    std::vector<bool> filter = {true, false, false, false}; // delete vertex 0

    auto count = vcl::deleteVerticesIf(mesh, filter);

    REQUIRE(count == 1);
    REQUIRE(mesh.vertexCount() == 3);
    REQUIRE(mesh.deletedVertexCount() == 1);
    REQUIRE(mesh.vertex(0).deleted());
    REQUIRE(!mesh.vertex(1).deleted());

    // Face 0 had vertex 0, so corner 0 becomes NULL_INDEX
    REQUIRE(mesh.face(0).vertexIndex(0) == MyMesh::NULL_INDEX);
    REQUIRE(mesh.face(0).vertexIndex(1) == 1);
    REQUIRE(mesh.face(0).vertexIndex(2) == 2);

    // Face 1 had vertices (1, 3, 2), unaffected
    REQUIRE(mesh.face(1).vertexIndex(0) == 1);
    REQUIRE(mesh.face(1).vertexIndex(1) == 3);
    REQUIRE(mesh.face(1).vertexIndex(2) == 2);
}

TEST_CASE("Delete Algorithms - Delete Faces If Function")
{
    MyMesh mesh = createSampleQuadMesh();

    // Delete face 0
    auto count =
        vcl::deleteFacesIf(mesh, [](auto f) { return f.index() == 0; });

    static_assert(std::is_same_v<decltype(count), MyMesh::IndexType>);
    REQUIRE(count == 1);
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(mesh.deletedFaceCount() == 1);
    REQUIRE(mesh.face(0).deleted());
    REQUIRE(!mesh.face(1).deleted());
}

TEST_CASE("Delete Algorithms - Delete Faces If Range")
{
    MyMesh mesh = createSampleQuadMesh();

    // Range for valid faces: delete face 1
    std::vector<bool> filter = {false, true};

    auto count = vcl::deleteFacesIf(mesh, filter);

    REQUIRE(count == 1);
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(mesh.deletedFaceCount() == 1);
    REQUIRE(!mesh.face(0).deleted());
    REQUIRE(mesh.face(1).deleted());
}

TEST_CASE("Delete Algorithms - Delete Edges If Function")
{
    // Sample edge mesh with 4 edges: (0, 1), (1, 3), (3, 2), (2, 0)
    MyEdgeMesh mesh = createSampleEdgeMesh();

    // Delete edges with odd indices (edges 1 and 3)
    auto count =
        vcl::deleteEdgesIf(mesh, [](auto e) { return e.index() % 2 == 1; });

    static_assert(std::is_same_v<decltype(count), MyEdgeMesh::IndexType>);
    REQUIRE(count == 2);
    REQUIRE(mesh.edgeCount() == 2);
    REQUIRE(mesh.deletedEdgeCount() == 2);
    REQUIRE(!mesh.edge(0).deleted());
    REQUIRE(mesh.edge(1).deleted());
    REQUIRE(!mesh.edge(2).deleted());
    REQUIRE(mesh.edge(3).deleted());
}

TEST_CASE("Delete Algorithms - Delete Edges If Range")
{
    MyEdgeMesh mesh = createSampleEdgeMesh();

    std::vector<bool> filter = {
        true, false, false, true}; // delete edge 0 and 3

    auto count = vcl::deleteEdgesIf(mesh, filter);

    REQUIRE(count == 2);
    REQUIRE(mesh.edgeCount() == 2);
    REQUIRE(mesh.deletedEdgeCount() == 2);
    REQUIRE(mesh.edge(0).deleted());
    REQUIRE(!mesh.edge(1).deleted());
    REQUIRE(!mesh.edge(2).deleted());
    REQUIRE(mesh.edge(3).deleted());
}

TEST_CASE("Delete Algorithms - Delete With Prior Deleted Elements")
{
    // Test on mesh that already has deleted elements before deleteVerticesIf
    MyMesh mesh = createSampleQuadMesh();

    // Pre-delete vertex 1
    mesh.deleteVertex(1);
    REQUIRE(mesh.vertexCount() == 3);

    // Filter over remaining valid vertices (0, 2, 3)
    // Vertex 2 has y = 1.0f, Vertex 3 has y = 1.0f
    auto count = vcl::deleteVerticesIf(
        mesh, [](auto v) { return v.position().y() > 0.5f; });

    REQUIRE(count == 2);
    REQUIRE(mesh.vertexCount() == 1); // Only vertex 0 remains
    REQUIRE(mesh.deletedVertexCount() == 3);
    REQUIRE(mesh.vertex(1).deleted());
    REQUIRE(mesh.vertex(2).deleted());
    REQUIRE(mesh.vertex(3).deleted());
    REQUIRE(!mesh.vertex(0).deleted());

    // Also test Range overload on a mesh with prior deleted elements:
    MyMesh mesh2 = createSampleQuadMesh();
    mesh2.deleteVertex(0); // Valid vertices are 1, 2, 3

    // Range has size 3 (one for each valid vertex: 1, 2, 3)
    std::vector<bool> filter = {false, true, false}; // delete vertex 2
    auto              count2 = vcl::deleteVerticesIf(mesh2, filter);

    REQUIRE(count2 == 1);
    REQUIRE(mesh2.vertexCount() == 2);
    REQUIRE(mesh2.vertex(0).deleted());
    REQUIRE(mesh2.vertex(2).deleted());
    REQUIRE(!mesh2.vertex(1).deleted());
    REQUIRE(!mesh2.vertex(3).deleted());
}

TEST_CASE("Delete Algorithms - Delete64 Bit Mesh")
{
    Mesh64 m;
    m.addVertices(4);
    m.vertex(0).position() = Point3d(0.0, 0.0, 0.0);
    m.vertex(1).position() = Point3d(1.0, 0.0, 0.0);
    m.vertex(2).position() = Point3d(0.0, 1.0, 0.0);
    m.vertex(3).position() = Point3d(1.0, 1.0, 0.0);

    m.addFace(0, 1, 2);
    m.addFace(1, 3, 2);

    auto count =
        vcl::deleteVerticesIf(m, [](auto v) { return v.index() == 1; });

    static_assert(std::is_same_v<decltype(count), uint64_t>);
    REQUIRE(count == 1);
    REQUIRE(m.vertexCount() == 3);
    REQUIRE(m.face(0).vertexIndex(1) == Mesh64::NULL_INDEX);
    REQUIRE(m.face(1).vertexIndex(0) == Mesh64::NULL_INDEX);
}

TEST_CASE("Delete Algorithms - Delete All Or None")
{
    MyMesh mesh = createSampleQuadMesh();

    // Delete none
    auto countNone = vcl::deleteVerticesIf(mesh, [](auto) { return false; });
    REQUIRE(countNone == 0);
    REQUIRE(mesh.vertexCount() == 4);

    // Delete all
    auto countAll = vcl::deleteVerticesIf(mesh, [](auto) { return true; });
    REQUIRE(countAll == 4);
    REQUIRE(mesh.vertexCount() == 0);
    REQUIRE(mesh.deletedVertexCount() == 4);

    // Face corners should all be NULL_INDEX
    REQUIRE(mesh.face(0).vertexIndex(0) == MyMesh::NULL_INDEX);
    REQUIRE(mesh.face(0).vertexIndex(1) == MyMesh::NULL_INDEX);
    REQUIRE(mesh.face(0).vertexIndex(2) == MyMesh::NULL_INDEX);
}
