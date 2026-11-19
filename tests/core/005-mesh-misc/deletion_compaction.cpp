// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

#include <atomic>
#include <ranges>
#include <vector>

inline void testVertexDeletionAndIteration(MyMesh& mesh)
{
    mesh.deleteVertex(1);

    REQUIRE(mesh.vertexContainerSize() == 4);
    REQUIRE(mesh.vertexCount() == 3);
    REQUIRE(mesh.deletedVertexCount() == 1);
    REQUIRE(!mesh.isCompact());

    // Iterating over active vertices (vertex 1 automatically skipped)
    std::size_t activeCount = 0;
    for (auto v : mesh.vertices()) {
        REQUIRE(v.index() != 1);
        REQUIRE(!v.deleted());
        ++activeCount;
    }
    REQUIRE(activeCount == 3);

    // Iterating over ALL vertices (including deleted, jumpDeleted = false)
    std::size_t allCount = 0;
    for (auto v : mesh.vertices(false)) {
        if (v.index() == 1) {
            REQUIRE(v.deleted());
        }
        ++allCount;
    }
    REQUIRE(allCount == 4);
}

inline void testComponentViewWithDeleted(MyMesh& mesh)
{
    // Test ComponentView jumping deleted elements
    REQUIRE(mesh.vertices().positions().size() == 3);
    REQUIRE(mesh.vertices(false).positions().size() == 4);

    std::size_t activePosCount = 0;
    for (auto& p : mesh.vertices().positions()) {
        (void) p;
        ++activePosCount;
    }
    REQUIRE(activePosCount == 3);

    std::size_t allPosCount = 0;
    for (auto& p : mesh.vertices(false).positions()) {
        (void) p;
        ++allPosCount;
    }
    REQUIRE(allPosCount == 4);
}

inline void testMeshCompaction(MyMesh& mesh)
{
    REQUIRE(!mesh.isCompact());
    mesh.compact();

    REQUIRE(mesh.isCompact());
    REQUIRE(mesh.vertexContainerSize() == 3);
    REQUIRE(mesh.vertexCount() == 3);
    REQUIRE(mesh.deletedVertexCount() == 0);

    using VertexIndex = MyMesh::Vertex::IndexType;
    auto formatVIdx   = [](VertexIndex idx) {
        return idx == MyMesh::NULL_INDEX ? "NULL" : std::to_string(idx);
    };

    // Vertex 1 was deleted, so Face 0 corner 1 is now NULL
    REQUIRE(mesh.face(0).vertexIndex(1) == MyMesh::NULL_INDEX);
    REQUIRE(!mesh.face(0).vertex(1).isValid());
    // Corner 0 is vertex 0
    REQUIRE(mesh.face(0).vertexIndex(0) == 0);
    REQUIRE(mesh.face(0).vertex(0).isValid());
}

inline void testFaceDeletionAndCompaction(MyMesh& mesh)
{
    // Test face deletion and compactFaces
    mesh.deleteFace(0);
    REQUIRE(!mesh.isCompact());
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(mesh.faceContainerSize() == 2);

    mesh.compactFaces();
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(mesh.faceContainerSize() == 1);
}

TEST_CASE("Deletion Compaction - 64 Bit Null Index And Compaction")
{
    Mesh64 m;
    m.addVertices(4);
    m.addFace(0, 1, 2);
    m.addFace(1, 3, 2);

    REQUIRE(m.vertex(0).index() != Mesh64::NULL_INDEX);
    REQUIRE(m.face(0).vertexIndex(0) == 0);

    // Delete vertex 1
    m.deleteVertex(1);
    REQUIRE(m.vertex(1).deleted());

    // Compact vertices
    m.compactVertices();
    REQUIRE(m.vertexCount() == 3);

    // Vertex 1 was deleted, so Face 0 corner 1 must be Mesh64::NULL_INDEX, NOT
    // 32-bit UINT_NULL
    REQUIRE(m.face(0).vertexIndex(1) == Mesh64::NULL_INDEX);
    REQUIRE(m.face(0).vertexIndex(1) != vcl::UINT_NULL);
    REQUIRE(!m.face(0).vertex(1).isValid());
}

inline void testIteratorDistanceWithDeleted(MyMesh& mesh)
{
    // delete a middle and the last vertex: the tail of the container is deleted
    mesh.deleteVertex(1);
    mesh.deleteVertex(mesh.vertexContainerSize() - 1);
    const std::size_t active = mesh.vertexCount();

    auto vertices = mesh.vertices();
    REQUIRE(
        static_cast<std::size_t>(vertices.end() - vertices.begin()) == active);
    REQUIRE(
        static_cast<std::size_t>(vertices.begin() - vertices.end()) ==
        0 - active);
    REQUIRE(std::ranges::distance(vertices) == static_cast<long>(active));

    // backward iteration must skip the deleted tail
    REQUIRE(vertices.back().index() == mesh.vertexContainerSize() - 2);
    REQUIRE((--vertices.end())->index() == mesh.vertexContainerSize() - 2);

    // parallelFor must visit exactly the active elements
    std::atomic<std::size_t> visited = 0;
    std::atomic<bool>        visitedDeleted = false;
    vcl::parallelFor(mesh.vertices(), [&](auto v) {
        if (v.deleted()) {
            visitedDeleted = true;
        }
        ++visited;
    });
    REQUIRE(visited == active);
    REQUIRE(!visitedDeleted);

    // component views
    auto positions = mesh.vertices().positions();
    REQUIRE(
        static_cast<std::size_t>(positions.end() - positions.begin()) ==
        active);

    // jumpDeleted = false covers the whole container
    auto all = mesh.vertices(false);
    REQUIRE(
        static_cast<std::size_t>(all.end() - all.begin()) ==
        mesh.vertexContainerSize());

    // default constructed view must not dereference a null container
    decltype(mesh.vertices().positions()) emptyView;
    REQUIRE(emptyView.begin() == emptyView.end());
}

TEST_CASE("Deletion Compaction - parallelFor on non compact mesh")
{
    constexpr std::size_t N = 100000;

    MyMesh mesh;
    mesh.addVertices(N);
    for (std::size_t i = 0; i < N; ++i) {
        mesh.vertex(i).position() = Point3f(i, 0, 0);
    }
    for (std::size_t i = 0; i < N; i += 3) {
        mesh.deleteVertex(i);
    }
    // deleted tail
    mesh.deleteVertex(N - 1);
    mesh.deleteVertex(N - 2);
    REQUIRE(!mesh.isCompact());

    std::vector<int> hits(N, 0);
    vcl::parallelFor(mesh.vertices(), [&](auto v) {
        ++hits[v.index()];
    });
    for (std::size_t i = 0; i < N; ++i) {
        REQUIRE(hits[i] == (mesh.vertex(i).deleted() ? 0 : 1));
    }

    // component view
    std::atomic<std::size_t> count = 0;
    vcl::parallelFor(mesh.vertices().positions(), [&](Point3f& p) {
        p.x() += 1;
        ++count;
    });
    REQUIRE(count == mesh.vertexCount());
    REQUIRE(mesh.vertex(1).position().x() == 2.0f);

    // iterators that do not jump deleted elements visit everything
    count = 0;
    vcl::parallelFor(mesh.vertices(false), [&](auto) { ++count; });
    REQUIRE(count == mesh.vertexContainerSize());

    // empty views
    vcl::parallelFor(
        decltype(mesh.vertices().positions()) {}, [&](Point3f&) { ++count; });
    REQUIRE(count == mesh.vertexContainerSize());
}

TEST_CASE("Deletion Compaction")
{
    MyMesh mesh = createSampleQuadMesh();

    testVertexDeletionAndIteration(mesh);
    testComponentViewWithDeleted(mesh);
    testMeshCompaction(mesh);
    testFaceDeletionAndCompaction(mesh);

    MyMesh mesh2 = createSampleQuadMesh();
    testIteratorDistanceWithDeleted(mesh2);
}
