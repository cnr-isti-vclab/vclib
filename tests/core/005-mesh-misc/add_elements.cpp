// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

inline void testAddVertex(MyMesh& mesh)
{
    // addVertex() and addVertex(pos)
    vcl::uint v0               = mesh.addVertex();
    mesh.vertex(v0).position() = Point3f(0.0f, 0.0f, 0.0f);
    REQUIRE(mesh.vertexCount() == 1);
    REQUIRE(mesh.vertex(v0).position() == Point3f(0.0f, 0.0f, 0.0f));

    vcl::uint v1 = mesh.addVertex(Point3f(1.0f, 0.0f, 0.0f));
    REQUIRE(mesh.vertexCount() == 2);
    REQUIRE(mesh.vertex(v1).position() == Point3f(1.0f, 0.0f, 0.0f));
}

inline void testAddVertices(MyMesh& mesh)
{
    // 1. addVertices(count)
    vcl::uint baseV = mesh.addVertices(2);
    REQUIRE(baseV == 2);
    REQUIRE(mesh.vertexCount() == 4);
    mesh.vertex(baseV).position()     = Point3f(0.0f, 1.0f, 0.0f);
    mesh.vertex(baseV + 1).position() = Point3f(1.0f, 1.0f, 0.0f);

    // 2. addVertices(variadic positions)
    vcl::uint baseVVar =
        mesh.addVertices(Point3f(2.0f, 0.0f, 0.0f), Point3f(2.0f, 1.0f, 0.0f));
    REQUIRE(baseVVar == 4);
    REQUIRE(mesh.vertexCount() == 6);
    REQUIRE(mesh.vertex(4).position() == Point3f(2.0f, 0.0f, 0.0f));
    REQUIRE(mesh.vertex(5).position() == Point3f(2.0f, 1.0f, 0.0f));

    // 3. addVertices(range of positions)
    std::vector<Point3f> newPos = {
        Point3f(3.0f, 0.0f, 0.0f), Point3f(3.0f, 1.0f, 0.0f)};
    vcl::uint baseVRng = mesh.addVertices(newPos);
    REQUIRE(baseVRng == 6);
    REQUIRE(mesh.vertexCount() == 8);
    REQUIRE(mesh.vertex(6).position() == Point3f(3.0f, 0.0f, 0.0f));
    REQUIRE(mesh.vertex(7).position() == Point3f(3.0f, 1.0f, 0.0f));
}

inline void testAddFace(MyMesh& mesh)
{
    // 1. addFace() and addFace(indices)
    vcl::uint addF0 = mesh.addFace();
    mesh.face(addF0).setVertices(0, 1, 2);
    REQUIRE(mesh.faceCount() == 1);
    REQUIRE(
        (mesh.face(addF0).vertexIndex(0) == 0 &&
         mesh.face(addF0).vertexIndex(1) == 1 &&
         mesh.face(addF0).vertexIndex(2) == 2));

    vcl::uint addF1 = mesh.addFace(1, 3, 2);
    REQUIRE(mesh.faceCount() == 2);
    REQUIRE(
        (mesh.face(addF1).vertexIndex(0) == 1 &&
         mesh.face(addF1).vertexIndex(1) == 3 &&
         mesh.face(addF1).vertexIndex(2) == 2));

    // 2. addFace(vertex proxies)
    vcl::uint addF2 =
        mesh.addFace(mesh.vertex(0), mesh.vertex(1), mesh.vertex(3));
    REQUIRE(mesh.faceCount() == 3);
    REQUIRE(
        (mesh.face(addF2).vertexIndex(0) == 0 &&
         mesh.face(addF2).vertexIndex(1) == 1 &&
         mesh.face(addF2).vertexIndex(2) == 3));

    // 3. addFace(range of indices)
    std::vector<vcl::uint> faceIndices = {4, 5, 2};
    vcl::uint              addF3       = mesh.addFace(faceIndices);
    REQUIRE(mesh.faceCount() == 4);
    REQUIRE(
        (mesh.face(addF3).vertexIndex(0) == 4 &&
         mesh.face(addF3).vertexIndex(1) == 5 &&
         mesh.face(addF3).vertexIndex(2) == 2));
}

inline void testAddFaces(MyMesh& mesh)
{
    // 1. addFaces(count)
    vcl::uint baseF = mesh.addFaces(2);
    REQUIRE(baseF == 4);
    REQUIRE(mesh.faceCount() == 6);

    // 2. addFaces(range of ranges)
    std::vector<std::vector<vcl::uint>> multiFaces = {
        {6, 7, 4},
        {7, 5, 4}
    };
    vcl::uint baseFMulti = mesh.addFaces(multiFaces);
    REQUIRE(baseFMulti == 6);
    REQUIRE(mesh.faceCount() == 8);
    REQUIRE(
        (mesh.face(6).vertexIndex(0) == 6 && mesh.face(6).vertexIndex(1) == 7 &&
         mesh.face(6).vertexIndex(2) == 4));
    REQUIRE(
        (mesh.face(7).vertexIndex(0) == 7 && mesh.face(7).vertexIndex(1) == 5 &&
         mesh.face(7).vertexIndex(2) == 4));
}

TEST_CASE("Add Elements - Add Polygon Faces")
{
    SimplePolyMesh polyMesh;
    polyMesh.addVertices(
        Point3f(0.0f, 0.0f, 0.0f),
        Point3f(1.0f, 0.0f, 0.0f),
        Point3f(1.0f, 1.0f, 0.0f),
        Point3f(0.0f, 1.0f, 0.0f),
        Point3f(0.5f, 1.5f, 0.0f));
    REQUIRE(polyMesh.vertexCount() == 5);

    // Quad face
    vcl::uint polyF0 = polyMesh.addFace(0, 1, 2, 3);
    REQUIRE(polyMesh.face(polyF0).vertexCount() == 4);
    REQUIRE(polyMesh.face(polyF0).vertexIndex(0) == 0);
    REQUIRE(polyMesh.face(polyF0).vertexIndex(3) == 3);

    // Pentagon face from range
    std::vector<vcl::uint> pentagon = {0, 1, 2, 4, 3};
    vcl::uint              polyF1   = polyMesh.addFace(pentagon);
    REQUIRE(polyMesh.face(polyF1).vertexCount() == 5);
    REQUIRE(polyMesh.face(polyF1).vertexIndex(3) == 4);
}

TEST_CASE("Add Elements - Default Component Initialization")
{
    // 1. Non-optional MaterialIndex component (ushort)
    MaterialMesh mesh;
    vcl::uint    f0 = mesh.addFace();
    REQUIRE(mesh.face(f0).materialIndex() == vcl::USHORT_NULL);

    vcl::uint fBase = mesh.addFaces(3);
    for (vcl::uint i = 0; i < 3; ++i) {
        REQUIRE(mesh.face(fBase + i).materialIndex() == vcl::USHORT_NULL);
    }

    mesh.resizeFaces(8);
    for (vcl::uint i = 4; i < 8; ++i) {
        REQUIRE(mesh.face(i).materialIndex() == vcl::USHORT_NULL);
    }

    // Mutate and check
    mesh.face(0).materialIndex() = 2;
    REQUIRE(mesh.face(0).materialIndex() == 2);
    REQUIRE(mesh.face(1).materialIndex() == vcl::USHORT_NULL);

    // 2. Optional MaterialIndex component
    OptionalMaterialMesh optMesh;
    REQUIRE((!optMesh.isElementComponentAvailable<
              ElemId::FACE,
              CompId::MATERIAL_INDEX>()));

    optMesh.resizeFaces(4);
    optMesh.enableAllOptionalComponents();
    REQUIRE((optMesh.isElementComponentAvailable<
             ElemId::FACE,
             CompId::MATERIAL_INDEX>()));

    for (vcl::uint i = 0; i < 4; ++i) {
        REQUIRE(optMesh.face(i).materialIndex() == vcl::USHORT_NULL);
    }

    vcl::uint optFBase = optMesh.addFaces(2);
    REQUIRE(optMesh.face(optFBase).materialIndex() == vcl::USHORT_NULL);
    REQUIRE(optMesh.face(optFBase + 1).materialIndex() == vcl::USHORT_NULL);
}

TEST_CASE("Add Elements")
{
    MyMesh mesh;

    testAddVertex(mesh);
    testAddVertices(mesh);
    testAddFace(mesh);
    testAddFaces(mesh);
}
