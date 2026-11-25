// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

inline void testMeshCreationAndColors(const MyMesh& mesh)
{
    using enum ElemId;
    using enum CompId;

    REQUIRE(mesh.vertexCount() == 4);
    REQUIRE(mesh.faceCount() == 2);

    // Mesh color
    REQUIRE(mesh.color() == Color::Red);
    static_assert(mesh::HasColor<decltype(mesh)>);

    // Vertex colors
    REQUIRE((mesh.isElementComponentAvailable<VERTEX, COLOR>()));
    REQUIRE(mesh.vertex(0).isComponentAvailable<COLOR>());
    REQUIRE(mesh.vertex(0).color() == Color::Blue);
    REQUIRE(mesh.vertex(1).color() == Color::Green);
    REQUIRE(mesh.vertex(2).color() == Color::Yellow);
    REQUIRE(mesh.vertex(3).color() == Color::Cyan);

    // Face colors
    REQUIRE((mesh.isElementComponentAvailable<FACE, COLOR>()));
    REQUIRE(mesh.face(0).isComponentAvailable<COLOR>());
    REQUIRE(mesh.face(0).color() == Color::DarkRed);
    REQUIRE(mesh.face(1).color() == Color::DarkBlue);
}

inline void testFaceToVertexNavigation(MyMesh& mesh)
{
    REQUIRE(mesh.face(0).vertexCount() == 3);
    for (vcl::uint i = 0; i < mesh.face(0).vertexCount(); ++i) {
        REQUIRE(mesh.face(0).vertex(i).index() == mesh.face(0).vertexIndex(i));
        REQUIRE(
            mesh.face(0).vertex(i).position() ==
            mesh.vertex(mesh.face(0).vertexIndex(i)).position());
    }

    vcl::uint cornerIdx = 0;
    for (MyMesh::Vertex v : mesh.face(1).vertices()) {
        REQUIRE(v.index() == mesh.face(1).vertexIndex(cornerIdx));
        ++cornerIdx;
    }
    REQUIRE(cornerIdx == 3);
}

inline void testMeshElementIteration(MyMesh& mesh)
{
    std::size_t vCount = 0;
    for (auto v : mesh.vertices()) {
        REQUIRE(v.index() == vCount);
        ++vCount;
    }
    REQUIRE(vCount == 4);

    std::size_t fCount = 0;
    for (auto f : mesh.faces()) {
        REQUIRE(f.index() == fCount);
        REQUIRE(f.vertexIndex(0) == mesh.face(fCount).vertexIndex(0));
        ++fCount;
    }
    REQUIRE(fCount == 2);
}

TEST_CASE("Navigation")
{
    MyMesh mesh = createSampleQuadMesh();

    testMeshCreationAndColors(mesh);
    testFaceToVertexNavigation(mesh);
    testMeshElementIteration(mesh);
}
