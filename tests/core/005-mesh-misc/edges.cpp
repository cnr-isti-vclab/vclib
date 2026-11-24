// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

#include <ranges>
#include <vector>

TEST_CASE("Edges - Edge Concepts")
{
    // Element concepts
    static_assert(ElementConcept<MyEdgeMesh::Edge>);
    static_assert(ElementConcept<MyEdgeMesh::ConstEdge>);
    static_assert(ElementConcept<GraphMesh::Edge>);
    static_assert(ElementConcept<GraphMesh::ConstEdge>);

    static_assert(EdgeConcept<MyEdgeMesh::Edge>);
    static_assert(EdgeConcept<MyEdgeMesh::ConstEdge>);
    static_assert(EdgeConcept<GraphMesh::Edge>);
    static_assert(EdgeConcept<GraphMesh::ConstEdge>);

    // Strong typing: Edge is not Vertex nor Face
    static_assert(!VertexConcept<MyEdgeMesh::Edge>);
    static_assert(!FaceConcept<MyEdgeMesh::Edge>);
    static_assert(!EdgeConcept<MyEdgeMesh::Vertex>);
    static_assert(!EdgeConcept<GraphMesh::Face>);

    // Component concepts
    static_assert(HasRequiredEdgeComponents<MyEdgeComponents>);
    static_assert(HasRequiredEdgeComponents<CCEdgeComponents>);
    static_assert(edge::HasVertexReferences<MyEdgeComponents>);
    static_assert(edge::HasVertexReferences<MyEdgeMesh::Edge>);
    static_assert(edge::HasColor<MyEdgeComponents>);
    static_assert(edge::HasColor<MyEdgeMesh::Edge>);
    static_assert(edge::HasOptionalQuality<MyEdgeComponents>);
    static_assert(edge::HasOptionalQuality<MyEdgeMesh::Edge>);

    // Mesh concepts
    static_assert(HasEdges<MyEdgeMesh>);
    static_assert(!HasFaces<MyEdgeMesh>);

    static_assert(HasFaces<GraphMesh>);
    static_assert(HasEdges<GraphMesh>);

    static_assert(!HasEdges<MyMesh>);

    static_assert(HasEdgeCustomComponents<EdgeMeshWithCC>);
    static_assert(!HasEdgeCustomComponents<MyEdgeMesh>);

    // Sentinels
    static_assert(MyEdgeMesh::NULL_INDEX == MyEdgeMesh::Edge::NULL_INDEX);
    static_assert(
        MyEdgeMesh::NULL_INDEX == MyEdgeMesh::EdgeContainer::NULL_INDEX);
    static_assert(GraphMesh::NULL_INDEX == GraphMesh::Edge::NULL_INDEX);

    // Iterators and Ranges
    static_assert(
        std::random_access_iterator<MyEdgeMesh::EdgeContainer::Iterator>);
    static_assert(
        std::random_access_iterator<MyEdgeMesh::EdgeContainer::ConstIterator>);
    static_assert(
        EdgeRangeConcept<decltype(std::declval<MyEdgeMesh>().edges())>);
    static_assert(
        EdgeRangeConcept<decltype(std::declval<const MyEdgeMesh>().edges())>);
}

TEST_CASE("Edges - Edge Creation And Properties")
{
    MyEdgeMesh m = createSampleEdgeMesh();

    REQUIRE(m.vertexCount() == 4);
    REQUIRE(m.edgeCount() == 4);
    REQUIRE(m.edgeContainerSize() == 4);
    REQUIRE(m.deletedEdgeCount() == 0);

    // Check vertex indices for each edge
    REQUIRE((m.edge(0).vertexIndex(0) == 0 && m.edge(0).vertexIndex(1) == 1));
    REQUIRE((m.edge(1).vertexIndex(0) == 1 && m.edge(1).vertexIndex(1) == 3));
    REQUIRE((m.edge(2).vertexIndex(0) == 3 && m.edge(2).vertexIndex(1) == 2));
    REQUIRE((m.edge(3).vertexIndex(0) == 2 && m.edge(3).vertexIndex(1) == 0));

    // Check navigating to Vertex proxies
    REQUIRE(m.edge(0).vertex(0).index() == 0);
    REQUIRE(m.edge(0).vertex(1).index() == 1);
    REQUIRE(m.edge(0).vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(m.edge(0).vertex(1).position() == Point3f(1.0f, 0.0f, 0.0f));

    // Test modifying vertices via setVertex and setVertices
    m.edge(0).setVertex(1, 2);
    REQUIRE(m.edge(0).vertexIndex(1) == 2);

    m.edge(0).setVertices(m.vertex(0), m.vertex(1));
    REQUIRE((m.edge(0).vertexIndex(0) == 0 && m.edge(0).vertexIndex(1) == 1));

    // Test color component
    m.edge(0).color() = Color::Red;
    REQUIRE(m.edge(0).color() == Color::Red);

    // Test optional quality
    REQUIRE((!m.isElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>()));
    m.enableElementComponent<ElemId::EDGE, CompId::QUALITY>();
    REQUIRE((m.isElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>()));
    m.edge(0).quality() = 42.0f;
    REQUIRE(m.edge(0).quality() == 42.0f);
    m.disableElementComponent<ElemId::EDGE, CompId::QUALITY>();
    REQUIRE((!m.isElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>()));
}

TEST_CASE("Edges - Edge Container Methods")
{
    MyEdgeMesh m;
    m.addVertices(4);

    // addEdge()
    uint e0 = m.addEdge();
    REQUIRE(e0 == 0);
    REQUIRE(m.edgeCount() == 1);
    m.edge(e0).setVertices(0, 1);

    // addEdge(v0, v1)
    uint e1 = m.addEdge(1, 2);
    REQUIRE(e1 == 1);
    REQUIRE((m.edge(e1).vertexIndex(0) == 1 && m.edge(e1).vertexIndex(1) == 2));

    // addEdge(rng)
    std::array<uint, 2> edgeVerts = {2, 3};
    uint                e2        = m.addEdge(edgeVerts);
    REQUIRE(e2 == 2);
    REQUIRE((m.edge(e2).vertexIndex(0) == 2 && m.edge(e2).vertexIndex(1) == 3));

    // addEdges(n)
    uint e3 = m.addEdges(2);
    REQUIRE(e3 == 3);
    REQUIRE(m.edgeCount() == 5);
    REQUIRE(m.edgeContainerSize() == 5);

    // addEdges(rng of ranges)
    std::vector<std::array<uint, 2>> batch = {
        {0, 2},
        {1, 3}
    };
    uint e5 = m.addEdges(batch);
    REQUIRE(e5 == 5);
    REQUIRE(m.edgeCount() == 7);
    REQUIRE((m.edge(5).vertexIndex(0) == 0 && m.edge(5).vertexIndex(1) == 2));
    REQUIRE((m.edge(6).vertexIndex(0) == 1 && m.edge(6).vertexIndex(1) == 3));

    // reserve & resize
    m.reserveEdges(20);
    m.resizeEdges(4);
    REQUIRE(m.edgeCount() == 4);
    REQUIRE(m.edgeContainerSize() == 4);

    // clearEdges()
    m.clearEdges();
    REQUIRE(m.edgeCount() == 0);
    REQUIRE(m.edgeContainerSize() == 0);
}

TEST_CASE("Edges - Edge Views And Ranges")
{
    MyEdgeMesh m = createSampleEdgeMesh();

    // Loop with m.edges()
    uint count = 0;
    for (auto e : m.edges()) {
        REQUIRE(e.index() == count);
        count++;
    }
    REQUIRE(count == 4);

    // Views pipe syntax
    count = 0;
    for (auto e : m | views::edges) {
        REQUIRE(e.index() == count);
        count++;
    }
    REQUIRE(count == 4);

    // Views function syntax
    count = 0;
    for (auto e : views::edges(m)) {
        REQUIRE(e.index() == count);
        count++;
    }
    REQUIRE(count == 4);

    // Generic elements<ElemId::EDGE>()
    count = 0;
    for (auto e : m.elements<ElemId::EDGE>()) {
        REQUIRE(e.index() == count);
        count++;
    }
    REQUIRE(count == 4);

    // Slice range
    count = 0;
    for (auto e : m.edges(1, 3)) {
        REQUIRE(e.index() == count + 1);
        count++;
    }
    REQUIRE(count == 2);

    // Column access
    auto colors = m.edges().colors();
    REQUIRE(colors.size() == 4);
    colors[0] = Color::Red;
    REQUIRE(m.edge(0).color() == Color::Red);

    auto viCol = m.edges().vertexIndices();
    REQUIRE(viCol.size() == 4);
    REQUIRE((viCol[0][0] == 0 && viCol[0][1] == 1));
}

TEST_CASE("Edges - Edge Deletion And Compaction")
{
    MyEdgeMesh m = createSampleEdgeMesh();

    REQUIRE(m.edgeCount() == 4);
    REQUIRE(m.isCompact());

    // Delete edge 1
    m.deleteEdge(1);
    REQUIRE(!m.isCompact());
    REQUIRE(m.edgeCount() == 3);
    REQUIRE(m.edgeContainerSize() == 4);
    REQUIRE(m.deletedEdgeCount() == 1);

    // Iteration skips deleted element
    std::vector<uint> visited;
    for (auto e : m.edges()) {
        visited.push_back(e.index());
    }
    REQUIRE((visited == std::vector<uint> {0, 2, 3}));

    // Iteration without jumpDeleted
    visited.clear();
    for (auto e : m.edges(false)) {
        visited.push_back(e.index());
    }
    REQUIRE((visited == std::vector<uint> {0, 1, 2, 3}));

    // edgeIndexIfCompact
    REQUIRE(m.edgeIndexIfCompact(0) == 0);
    REQUIRE(m.edgeIndexIfCompact(2) == 1);
    REQUIRE(m.edgeIndexIfCompact(3) == 2);

    // Compact
    auto newIndices = m.compactEdges();
    REQUIRE(m.isCompact());
    REQUIRE(m.edgeCount() == 3);
    REQUIRE(m.edgeContainerSize() == 3);
    REQUIRE(m.deletedEdgeCount() == 0);

    REQUIRE(newIndices[0] == 0);
    REQUIRE(newIndices[1] == MyEdgeMesh::NULL_INDEX);
    REQUIRE(newIndices[2] == 1);
    REQUIRE(newIndices[3] == 2);

    REQUIRE((m.edge(0).vertexIndex(0) == 0 && m.edge(0).vertexIndex(1) == 1));
    REQUIRE((m.edge(1).vertexIndex(0) == 3 && m.edge(1).vertexIndex(1) == 2));
    REQUIRE((m.edge(2).vertexIndex(0) == 2 && m.edge(2).vertexIndex(1) == 0));
}

TEST_CASE("Edges - Edge Custom Components")
{
    EdgeMeshWithCC m;
    m.addVertices(4);
    m.addEdge(0, 1);
    m.addEdge(1, 2);

    REQUIRE(!m.hasEdgeCustomComponent("weight"));
    m.addEdgeCustomComponent<float>("weight");
    REQUIRE(m.hasEdgeCustomComponent("weight"));
    REQUIRE(m.isEdgeCustomComponentOfType<float>("weight"));

    auto names = m.edgeCustomComponentNames();
    REQUIRE((names.size() == 1 && names[0] == "weight"));

    auto handle = m.edgeCustomComponentVectorHandle<float>("weight");
    handle[0]   = 1.5f;
    handle[1]   = 2.5f;

    REQUIRE(m.edge(0).customComponent<float>("weight") == 1.5f);
    REQUIRE(m.edge(1).customComponent<float>("weight") == 2.5f);

    m.deleteEdgeCustomComponent("weight");
    REQUIRE(!m.hasEdgeCustomComponent("weight"));
}

TEST_CASE("Edges - Edge Append")
{
    MyEdgeMesh m1;
    m1.addVertices(2);
    m1.addEdge(0, 1);

    MyEdgeMesh m2;
    m2.addVertices(3);
    m2.addEdge(0, 1);
    m2.addEdge(1, 2);

    m1.append(m2);

    REQUIRE(m1.vertexCount() == 5);
    REQUIRE(m1.edgeCount() == 3);

    // Initial edge from m1
    REQUIRE((m1.edge(0).vertexIndex(0) == 0 && m1.edge(0).vertexIndex(1) == 1));

    // Appended edges from m2: vertex references must have been shifted by 2!
    REQUIRE((m1.edge(1).vertexIndex(0) == 2 && m1.edge(1).vertexIndex(1) == 3));
    REQUIRE((m1.edge(2).vertexIndex(0) == 3 && m1.edge(2).vertexIndex(1) == 4));
}
