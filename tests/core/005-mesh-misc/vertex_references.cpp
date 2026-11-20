// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

#include <iterator>
#include <ranges>
#include <sstream>

inline void testTriangleReferences(MyMesh& mesh)
{
    auto testF = mesh.face(0);
    // testF indices are (0, 1, 2)
    REQUIRE(testF.vertexIndexMod(0) == 0);
    REQUIRE(testF.vertexIndexMod(3) == 0);
    REQUIRE(testF.vertexIndexMod(-1) == 2);
    REQUIRE(testF.vertexIndexMod(-3) == 0);

    REQUIRE(testF.containsVertex(0));
    REQUIRE(testF.containsVertex(1));
    REQUIRE(!testF.containsVertex(3));
    REQUIRE(testF.containsVertex(mesh.vertex(2)));

    REQUIRE(testF.indexOfVertex(0) == 0);
    REQUIRE(testF.indexOfVertex(1) == 1);
    REQUIRE(testF.indexOfVertex(2) == 2);
    REQUIRE(testF.indexOfVertex(3) == vcl::UINT_NULL);

    // Edge (0, 1) -> edge 0
    // Edge (1, 2) -> edge 1
    // Edge (2, 0) -> edge 2
    REQUIRE(testF.indexOfEdge(0, 1) == 0);
    REQUIRE(testF.indexOfEdge(1, 0) == 0);
    REQUIRE(testF.indexOfEdge(1, 2) == 1);
    REQUIRE(testF.indexOfEdge(2, 1) == 1);
    REQUIRE(testF.indexOfEdge(2, 0) == 2);
    REQUIRE(testF.indexOfEdge(0, 2) == 2);
    REQUIRE(testF.indexOfEdge(0, 3) == vcl::UINT_NULL);

    // Test setVertexMod
    testF.setVertexMod(-1, 2);

    // Test setVertices range
    std::array<vcl::uint, 3> newIdxs = {0, 1, 2};
    testF.setVertices(newIdxs);
    REQUIRE(
        (testF.vertexIndex(0) == 0 && testF.vertexIndex(1) == 1 &&
         testF.vertexIndex(2) == 2));

    // Test setVertices variadic
    testF.setVertices(0, 1, 2);
    REQUIRE(
        (testF.vertexIndex(0) == 0 && testF.vertexIndex(1) == 1 &&
         testF.vertexIndex(2) == 2));

    // Test setVertices single-pass input_range (std::istream_iterator)
    {
        std::istringstream iss("2 0 1");
        auto               streamRange = std::ranges::subrange(
            std::istream_iterator<vcl::uint>(iss),
            std::istream_iterator<vcl::uint>());
        testF.setVertices(streamRange);
        REQUIRE(testF.vertexIndex(0) == 2);
        REQUIRE(testF.vertexIndex(1) == 0);
        REQUIRE(testF.vertexIndex(2) == 1);
    }
}

TEST_CASE("Vertex References - Polygon References")
{
    SimplePolyMesh polyMesh;
    polyMesh.addVertices(5);

    vcl::uint fi = polyMesh.addFace(0, 1, 2, 3);
    auto      f  = polyMesh.face(fi);
    REQUIRE(f.vertexCount() == 4);

    // containsVertex & indexOfVertex
    REQUIRE(f.containsVertex(0));
    REQUIRE(f.containsVertex(3));
    REQUIRE(!f.containsVertex(4));
    REQUIRE(f.indexOfVertex(2) == 2);
    REQUIRE(f.indexOfVertex(4) == vcl::UINT_NULL);

    // indexOfEdge
    REQUIRE(f.indexOfEdge(0, 1) == 0);
    REQUIRE(f.indexOfEdge(1, 0) == 0);
    REQUIRE(f.indexOfEdge(1, 2) == 1);
    REQUIRE(f.indexOfEdge(2, 3) == 2);
    REQUIRE(f.indexOfEdge(3, 0) == 3);
    REQUIRE(f.indexOfEdge(0, 3) == 3);
    REQUIRE(f.indexOfEdge(0, 2) == vcl::UINT_NULL);

    // Dynamic operations on polygon
    f.pushVertex(4);
    REQUIRE(f.vertexCount() == 5);
    REQUIRE(f.vertexIndex(4) == 4);
    REQUIRE(f.containsVertex(4));

    f.insertVertex(1, 99);
    REQUIRE(f.vertexCount() == 6);
    REQUIRE(f.vertexIndex(1) == 99);
    REQUIRE(f.vertexIndex(2) == 1);

    f.eraseVertex(1);
    REQUIRE(f.vertexCount() == 5);
    REQUIRE(f.vertexIndex(1) == 1);

    f.clearVertices();
    REQUIRE(f.vertexCount() == 0);

    // Test setVertices single-pass input_range on dynamic Polygon
    {
        std::istringstream iss("1 3 4");
        auto               streamRange = std::ranges::subrange(
            std::istream_iterator<vcl::uint>(iss),
            std::istream_iterator<vcl::uint>());
        f.setVertices(streamRange);
        REQUIRE(f.vertexCount() == 3);
        REQUIRE(f.vertexIndex(0) == 1);
        REQUIRE(f.vertexIndex(1) == 3);
        REQUIRE(f.vertexIndex(2) == 4);
    }
}

TEST_CASE("Vertex References")
{
    MyMesh mesh = createSampleQuadMesh();

    testTriangleReferences(mesh);
}
