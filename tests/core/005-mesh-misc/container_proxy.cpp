// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

inline void testProxyIndexing(MyMesh& mesh)
{
    // Direct indexing on ElementContainerProxy
    REQUIRE(mesh.vertices().size() == 4);
    REQUIRE(mesh.vertices()[0].position() == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(mesh.faces().size() == 2);
    REQUIRE(mesh.faces()[0].vertexIndex(0) == 0);
}

inline void testPositionsComponentView(MyMesh& mesh)
{
    auto posView = mesh.vertices().positions();
    REQUIRE(posView.size() == 4);
    REQUIRE(posView.isAvailable());
    REQUIRE(posView[0] == Point3f(0.0f, 0.0f, 1.0f));
    REQUIRE(posView.data() != nullptr);
    REQUIRE(posView.data()[0] == Point3f(0.0f, 0.0f, 1.0f));

    // Test mutation via positions operator[]
    posView[0] = Point3f(0.1f, 0.2f, 1.3f);
    REQUIRE(mesh.vertex(0).position() == Point3f(0.1f, 0.2f, 1.3f));
    posView[0] = Point3f(0.0f, 0.0f, 1.0f); // restore

    // Iterate directly over positions
    std::size_t posCount = 0;
    for (Point3f& p : mesh.vertices().positions()) {
        static_assert(!std::is_const_v<std::remove_reference_t<decltype(p)>>);
        ++posCount;
    }
    REQUIRE(posCount == 4);
}

inline void testNormalsAndColorsComponentViews(MyMesh& mesh)
{
    auto normView = mesh.vertices().normals();
    REQUIRE(normView.size() == 4);
    REQUIRE(normView[0] == Point3f(0.0f, 0.0f, 1.0f));

    auto colView = mesh.vertices().colors();
    REQUIRE(colView.size() == 4);
    REQUIRE(colView[0] == Color::Blue);
    REQUIRE(colView[1] == Color::Green);

    // Face colors
    auto faceColView = mesh.faces().colors();
    REQUIRE(faceColView.size() == 2);
    REQUIRE(faceColView[0] == Color::DarkRed);
}

inline void testFaceIndicesComponentView(MyMesh& mesh)
{
    auto faceIndicesView = mesh.faces().vertexIndices();
    REQUIRE(faceIndicesView.size() == 2);
    REQUIRE(
        (faceIndicesView[0][0] == 0 && faceIndicesView[0][1] == 1 &&
         faceIndicesView[0][2] == 2));
    REQUIRE(faceIndicesView.data() != nullptr);
}

TEST_CASE("Container Proxy - Proxy Concepts")
{
    MyMesh mesh;
    static_assert(!CanCallBitFlags<decltype(mesh.vertices())>);
    static_assert(!CanCallPositions<decltype(mesh.faces())>);
    static_assert(!CanCallNormals<decltype(SimpleMesh {}.vertices())>);
    static_assert(CanCallPositions<decltype(mesh.vertices())>);
    static_assert(CanCallNormals<decltype(mesh.vertices())>);
    static_assert(CanCallVertexIndices<decltype(mesh.faces())>);
    static_assert(!CanCallVertexIndices<decltype(SimplePolyMesh {}.faces())>);
}

TEST_CASE("Container Proxy")
{
    MyMesh mesh = createSampleQuadMesh();

    testProxyIndexing(mesh);
    testPositionsComponentView(mesh);
    testNormalsAndColorsComponentViews(mesh);
    testFaceIndicesComponentView(mesh);
}
