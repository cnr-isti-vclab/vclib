// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/meshes.h>

#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <limits>

TEST_CASE("Clean Algorithms - Remove Unreferenced Vertices")
{
    MyMesh mesh = createSampleQuadMesh();
    REQUIRE(mesh.vertexCount() == 4);

    // Add 2 unreferenced vertices
    mesh.addVertex(Point3f(5.0f, 5.0f, 5.0f));
    mesh.addVertex(Point3f(6.0f, 6.0f, 6.0f));
    REQUIRE(mesh.vertexCount() == 6);

    auto removed = vcl::removeUnreferencedVertices(mesh);

    REQUIRE(removed == 2);
    REQUIRE(mesh.vertexCount() == 4);
    REQUIRE(mesh.deletedVertexCount() == 2);
    REQUIRE(mesh.vertex(4).deleted());
    REQUIRE(mesh.vertex(5).deleted());

    // Calling again should remove 0
    auto removedAgain = vcl::removeUnreferencedVertices(mesh);
    REQUIRE(removedAgain == 0);

    // Test with 64-bit mesh
    vcl::TriMesh64 m64;
    m64.addVertices(4);
    m64.vertex(0).position() = Point3d(0.0, 0.0, 0.0);
    m64.vertex(1).position() = Point3d(1.0, 0.0, 0.0);
    m64.vertex(2).position() = Point3d(0.0, 1.0, 0.0);
    m64.vertex(3).position() = Point3d(2.0, 2.0, 2.0); // unreferenced
    m64.addFace(0, 1, 2);

    REQUIRE(vcl::unreferencedVertexCount(m64) == 1);
    auto rem64 = vcl::removeUnreferencedVertices(m64);
    static_assert(std::is_same_v<decltype(rem64), uint64_t>);
    REQUIRE(rem64 == 1);
    REQUIRE(m64.vertexCount() == 3);
    REQUIRE(m64.deletedVertexCount() == 1);
    REQUIRE(m64.vertex(3).deleted());
}

TEST_CASE("Clean Algorithms - Remove Duplicate Vertices")
{
    MyMesh mesh;
    mesh.addVertices(4);
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(3).position() =
        Point3f(1.0f, 0.0f, 0.0f); // duplicate of vertex 1!

    mesh.addFace(0, 1, 2);
    mesh.addFace(3, 2, 0);

    auto removed = vcl::removeDuplicateVertices(mesh);

    REQUIRE(removed == 1);
    REQUIRE(mesh.vertexCount() == 3);
    REQUIRE(mesh.deletedVertexCount() == 1);
    REQUIRE(mesh.vertex(3).deleted());

    // Face 1 was (3, 2, 0), vertex 3 was merged to vertex 1
    REQUIRE(mesh.face(1).vertexIndex(0) == 1);
    REQUIRE(mesh.face(1).vertexIndex(1) == 2);
    REQUIRE(mesh.face(1).vertexIndex(2) == 0);
}

TEST_CASE("Clean Algorithms - Remove Duplicate Faces")
{
    MyMesh mesh;
    mesh.addVertices(3);
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);

    mesh.addFace(0, 1, 2);
    mesh.addFace(1, 2, 0); // same vertices cycle
    mesh.addFace(0, 2, 1); // same vertices inverted

    REQUIRE(mesh.faceCount() == 3);

    auto removed = vcl::removeDuplicateFaces(mesh);

    REQUIRE(removed == 2);
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(mesh.deletedFaceCount() == 2);
}

TEST_CASE("Clean Algorithms - Remove Degenerate Vertices")
{
    MyMesh mesh;
    mesh.addVertices(4);
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(3).position() =
        Point3f(std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f);

    mesh.addFace(0, 1, 2);
    mesh.addFace(0, 1, 3); // incident to NaN vertex

    auto removed = vcl::removeDegenerateVertices(mesh, true);

    REQUIRE(removed == 1);
    REQUIRE(mesh.vertexCount() == 3);
    REQUIRE(mesh.vertex(3).deleted());

    // Face 1 was incident to vertex 3, so deleteAlsoIncidentElements deletes it
    REQUIRE(!mesh.face(0).deleted());
    REQUIRE(mesh.face(1).deleted());
    REQUIRE(mesh.faceCount() == 1);
}

TEST_CASE("Clean Algorithms - Remove Degenerate Faces")
{
    MyMesh mesh;
    mesh.addVertices(3);
    mesh.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    mesh.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    mesh.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);

    mesh.addFace(0, 1, 2); // valid
    mesh.addFace(0, 0, 1); // degenerate: corners 0 and 1 are vertex 0
    mesh.addFace(1, 2, 2); // degenerate: corners 1 and 2 are vertex 2

    REQUIRE(mesh.faceCount() == 3);

    auto removed = vcl::removeDegenerateFaces(mesh);

    REQUIRE(removed == 2);
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(!mesh.face(0).deleted());
    REQUIRE(mesh.face(1).deleted());
    REQUIRE(mesh.face(2).deleted());
}
