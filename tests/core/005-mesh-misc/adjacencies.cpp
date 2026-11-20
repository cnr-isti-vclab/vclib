// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

#include <iterator>
#include <ranges>
#include <sstream>
#include <vector>

TEST_CASE("Adjacencies")
{
    AdjMesh m;
    m.resizeVertices(4);
    m.resizeFaces(2);

    m.vertex(0).position() = Point3f(0.0f, 0.0f, 1.0f);
    m.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    m.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    m.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);

    m.face(0).setVertices(0, 1, 2);
    m.face(1).setVertices(1, 3, 2);

    // 1. Static face adjacencies (TriangleAdjacentFaces: size 3)
    auto f0 = m.face(0);
    auto f1 = m.face(1);

    REQUIRE(f0.adjFaceCount() == 3);
    REQUIRE(f1.adjFaceCount() == 3);

    // Initially NULL_INDEX
    REQUIRE(f0.adjFaceIndex(0) == vcl::NULL_INDEX<uint>);
    REQUIRE(f0.adjFaceIndex(1) == vcl::NULL_INDEX<uint>);
    REQUIRE(f0.adjFaceIndex(2) == vcl::NULL_INDEX<uint>);

    // Set face 0's edge 1 adjacent to face 1, face 1's edge 2 adjacent to face
    // 0
    f0.setAdjFace(1, 1);
    f1.setAdjFace(2, f0); // using proxy

    REQUIRE(f0.adjFaceIndex(1) == 1);
    REQUIRE(f0.adjFace(1).index() == 1);
    REQUIRE(f1.adjFaceIndex(2) == 0);
    REQUIRE(f1.adjFace(2).index() == 0);

    // Modular access
    REQUIRE(f0.adjFaceIndexMod(1) == 1);
    REQUIRE(f0.adjFaceIndexMod(4) == 1);  // 4 % 3 = 1
    REQUIRE(f0.adjFaceIndexMod(-2) == 1); // (-2 % 3 + 3) % 3 = 1
    REQUIRE(f0.adjFaceMod(4).index() == 1);
    REQUIRE(f0.adjFaceMod(-2).index() == 1);

    // setAdjFaceMod
    f0.setAdjFaceMod(-2, 1);
    REQUIRE(f0.adjFaceIndex(1) == 1);

    // Search: containsAdjFace & indexOfAdjFace
    REQUIRE(f0.containsAdjFace(1));
    REQUIRE(f0.containsAdjFace(f1));
    REQUIRE(!f0.containsAdjFace(99));
    REQUIRE(f0.indexOfAdjFace(1) == 1);
    REQUIRE(f0.indexOfAdjFace(f1) == 1);
    REQUIRE(f0.indexOfAdjFace(99) == UINT_NULL);

    // Range-based iteration
    uint count = 0;
    for (uint fi : f0.adjFaceIndices()) {
        if (count == 1) {
            REQUIRE(fi == 1);
        }
        else {
            REQUIRE(fi == vcl::NULL_INDEX<uint>);
        }
        ++count;
    }
    REQUIRE(count == 3);

    // Test setAdjFaces single-pass input_range (static)
    {
        std::istringstream iss("0 1 0");
        auto               streamRange = std::ranges::subrange(
            std::istream_iterator<vcl::uint>(iss),
            std::istream_iterator<vcl::uint>());
        f0.setAdjFaces(streamRange);
        REQUIRE(f0.adjFaceIndex(0) == 0);
        REQUIRE(f0.adjFaceIndex(1) == 1);
        REQUIRE(f0.adjFaceIndex(2) == 0);
        // restore original
        f0.setAdjFace(0, vcl::NULL_INDEX<uint>);
        f0.setAdjFace(2, vcl::NULL_INDEX<uint>);
    }

    // 2. Dynamic vertex adjacencies (AdjacentFaces and AdjacentVertices on
    // Vertex)
    auto v0 = m.vertex(0);
    auto v1 = m.vertex(1);
    auto v2 = m.vertex(2);
    auto v3 = m.vertex(3);

    REQUIRE(v0.adjFaceCount() == 0);
    REQUIRE(v0.adjVertexCount() == 0);

    // Dynamic operations on Vertex AdjacentFaces
    v0.pushAdjFace(0);
    REQUIRE(v0.adjFaceCount() == 1);
    REQUIRE(v0.adjFaceIndex(0) == 0);
    REQUIRE(v0.adjFace(0).index() == 0);

    v1.pushAdjFace(0);
    v1.pushAdjFace(f1);
    REQUIRE(v1.adjFaceCount() == 2);
    REQUIRE(v1.adjFaceIndex(0) == 0);
    REQUIRE(v1.adjFaceIndex(1) == 1);

    v2.resizeAdjFaces(2);
    v2.setAdjFace(0, 0);
    v2.setAdjFace(1, 1);
    REQUIRE(v2.adjFaceCount() == 2);

    v3.pushAdjFace(1);
    REQUIRE(v3.adjFaceCount() == 1);

    // Test insert, erase, clear on vertex adjacencies
    v0.insertAdjFace(0, 42);
    REQUIRE(v0.adjFaceCount() == 2);
    REQUIRE(v0.adjFaceIndex(0) == 42);
    REQUIRE(v0.adjFaceIndex(1) == 0);

    v0.eraseAdjFace(0);
    REQUIRE(v0.adjFaceCount() == 1);
    REQUIRE(v0.adjFaceIndex(0) == 0);

    // Dynamic operations on Vertex AdjacentVertices
    v0.pushAdjVertex(v1);
    v0.pushAdjVertex(2);
    REQUIRE(v0.adjVertexCount() == 2);
    REQUIRE(v0.adjVertexIndex(0) == 1);
    REQUIRE(v0.adjVertexIndex(1) == 2);
    REQUIRE(v0.adjVertex(0).index() == 1);
    REQUIRE(v0.adjVertex(1).index() == 2);

    std::vector<uint> v1_adj_verts = {0, 2, 3};
    v1.setAdjVertices(v1_adj_verts);
    REQUIRE(v1.adjVertexCount() == 3);
    REQUIRE(v1.adjVertexIndex(0) == 0);
    REQUIRE(v1.adjVertexIndex(1) == 2);
    REQUIRE(v1.adjVertexIndex(2) == 3);

    // Iterate through adjVertices()
    uint adjCount = 0;
    for (auto adjV : v1.adjVertices()) {
        REQUIRE(adjV.index() == v1_adj_verts[adjCount]);
        ++adjCount;
    }
    REQUIRE(adjCount == 3);

    // Test setAdjVertices single-pass input_range (dynamic)
    {
        std::istringstream iss("0 1 2");
        auto               streamRange = std::ranges::subrange(
            std::istream_iterator<vcl::uint>(iss),
            std::istream_iterator<vcl::uint>());
        v3.setAdjVertices(streamRange);
        REQUIRE(v3.adjVertexCount() == 3);
        REQUIRE(v3.adjVertexIndex(0) == 0);
        REQUIRE(v3.adjVertexIndex(1) == 1);
        REQUIRE(v3.adjVertexIndex(2) == 2);
        v3.clearAdjVertices();
    }

    // 3. Compaction test
    AdjMesh mComp = m;
    REQUIRE(mComp.vertex(1).adjFaceIndex(0) == 0);
    REQUIRE(mComp.vertex(1).adjFaceIndex(1) == 1);
    REQUIRE(mComp.face(0).adjFaceIndex(1) == 1);
    REQUIRE(mComp.face(1).adjFaceIndex(2) == 0);

    // Delete face 0 and compact
    mComp.deleteFace(0);
    mComp.compact();
    REQUIRE(mComp.faceCount() == 1);
    // Face 1 became Face 0.
    // Face 0's adjFace at index 2 pointed to old Face 0 (which was deleted).
    REQUIRE(mComp.face(0).adjFaceIndex(2) == vcl::NULL_INDEX<uint>);

    // 4. Append test
    AdjMesh mApp1      = m;
    AdjMesh mApp2      = m;
    uint    initVCount = mApp1.vertexCount();
    uint    initFCount = mApp1.faceCount();

    mApp1.append(mApp2);
    REQUIRE(mApp1.vertexCount() == initVCount * 2);
    REQUIRE(mApp1.faceCount() == initFCount * 2);

    // Face in second half (old face 0, now face initFCount) should have its
    // adjFace index shifted by initFCount!
    REQUIRE(mApp1.face(initFCount).adjFaceIndex(1) == 1 + initFCount);
    REQUIRE(mApp1.face(initFCount + 1).adjFaceIndex(2) == 0 + initFCount);

    // Vertex in second half had adjFace {0, 1} and adjVertex {0, 2, 3}:
    auto vApp2_1 = mApp1.vertex(initVCount + 1);
    REQUIRE(vApp2_1.adjFaceIndex(0) == 0 + initFCount);
    REQUIRE(vApp2_1.adjFaceIndex(1) == 1 + initFCount);
    REQUIRE(vApp2_1.adjVertexIndex(0) == 0 + initVCount);
    REQUIRE(vApp2_1.adjVertexIndex(1) == 2 + initVCount);
    REQUIRE(vApp2_1.adjVertexIndex(2) == 3 + initVCount);

    // 5. Optional component test (AdjacentEdges on Vertex and Face)
    REQUIRE(f0.isAdjacentFacesAvailable());
    REQUIRE(m.vertex(0).isAdjacentFacesAvailable());
    REQUIRE(m.vertex(0).isAdjacentVerticesAvailable());
    REQUIRE(!m.vertex(0).isAdjacentEdgesAvailable());
    REQUIRE(!m.face(0).isAdjacentEdgesAvailable());
    REQUIRE((!m.isElementComponentAvailable<
              ElemId::VERTEX,
              CompId::ADJACENT_EDGES>()));
    REQUIRE((!m.isElementComponentAvailable<
              ElemId::FACE,
              CompId::ADJACENT_EDGES>()));

    m.enableElementComponent<ElemId::VERTEX, CompId::ADJACENT_EDGES>();
    m.enableElementComponent<ElemId::FACE, CompId::ADJACENT_EDGES>();

    REQUIRE(m.vertex(0).isAdjacentEdgesAvailable());
    REQUIRE(m.face(0).isAdjacentEdgesAvailable());
    REQUIRE((m.isElementComponentAvailable<
             ElemId::VERTEX,
             CompId::ADJACENT_EDGES>()));
    REQUIRE((
        m.isElementComponentAvailable<ElemId::FACE, CompId::ADJACENT_EDGES>()));

    m.vertex(0).pushAdjEdge(10);
    REQUIRE(m.vertex(0).adjEdgeCount() == 1);
    REQUIRE(m.vertex(0).adjEdgeIndex(0) == 10);

    m.face(0).setAdjEdge(0, 100);
    REQUIRE(m.face(0).adjEdgeIndex(0) == 100);
}

TEST_CASE(
    "PolyMesh - TTVC Adjacencies pushVertex and insertVertex default to "
    "NULL_INDEX")
{
    vcl::PolyMesh pm;
    pm.enableFaceAdjacentFaces();

    pm.addVertices(5);
    for (uint i = 0; i < 5; ++i) {
        pm.vertex(i).position() = Point3d(i, 0.0, 0.0);
    }

    auto f = pm.face(pm.addFace());
    f.pushVertex(0);
    f.pushVertex(1);
    f.pushVertex(2);

    REQUIRE(f.vertexCount() == 3);
    REQUIRE(f.adjFaceCount() == 3);
    // Newly created adjacent faces must be NULL_INDEX, NOT 0
    REQUIRE(f.adjFaceIndex(0) == vcl::PolyMesh::NULL_INDEX);
    REQUIRE(f.adjFaceIndex(1) == vcl::PolyMesh::NULL_INDEX);
    REQUIRE(f.adjFaceIndex(2) == vcl::PolyMesh::NULL_INDEX);

    // Set specific values to existing adjacencies
    f.setAdjFace(0, 10);
    f.setAdjFace(1, 20);
    f.setAdjFace(2, 30);

    // pushVertex must append an entry initialized to NULL_INDEX (NOT 0)
    f.pushVertex(3);
    REQUIRE(f.vertexCount() == 4);
    REQUIRE(f.adjFaceCount() == 4);
    REQUIRE(f.adjFaceIndex(0) == 10);
    REQUIRE(f.adjFaceIndex(1) == 20);
    REQUIRE(f.adjFaceIndex(2) == 30);
    REQUIRE(f.adjFaceIndex(3) == vcl::PolyMesh::NULL_INDEX);

    // insertVertex at position 1 must insert an entry initialized to NULL_INDEX
    // (NOT 0)
    f.insertVertex(1, 4);
    REQUIRE(f.vertexCount() == 5);
    REQUIRE(f.adjFaceCount() == 5);
    REQUIRE(f.adjFaceIndex(0) == 10);
    REQUIRE(f.adjFaceIndex(1) == vcl::PolyMesh::NULL_INDEX);
    REQUIRE(f.adjFaceIndex(2) == 20);
    REQUIRE(f.adjFaceIndex(3) == 30);
    REQUIRE(f.adjFaceIndex(4) == vcl::PolyMesh::NULL_INDEX);
}
