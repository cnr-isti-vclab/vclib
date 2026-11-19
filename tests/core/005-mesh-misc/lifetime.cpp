// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/space/core.h>

#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <type_traits>

static_assert(std::is_nothrow_move_constructible_v<MyMesh>);
static_assert(std::is_nothrow_move_assignable_v<MyMesh>);
static_assert(std::is_nothrow_swappable_v<MyMesh>);
static_assert(std::movable<MyMesh>);
static_assert(
    std::is_nothrow_move_constructible_v<MyMesh::ContainerType<ElemId::VERTEX>>);
static_assert(
    std::is_nothrow_move_assignable_v<MyMesh::ContainerType<ElemId::VERTEX>>);
static_assert(std::is_nothrow_swappable_v<MyMesh::ContainerType<ElemId::VERTEX>>);
static_assert(std::movable<MyMesh::ContainerType<ElemId::VERTEX>>);

inline void testElementValidity(MyMesh& mesh)
{
    MyMesh::Vertex validV = mesh.vertex(0);
    MyMesh::Vertex invalidV;

    REQUIRE((bool) validV);
    REQUIRE(validV.isValid());
    REQUIRE(!(bool) invalidV);
    REQUIRE(!invalidV.isValid());

    MyMesh::Face validF = mesh.face(0);
    MyMesh::Face invalidF;
    REQUIRE((bool) validF);
    REQUIRE(validF.isValid());
    REQUIRE(!(bool) invalidF);
    REQUIRE(!invalidF.isValid());
}

inline void testMeshCopy(const MyMesh& mesh)
{
    MyMesh meshCopy = mesh;
    REQUIRE(meshCopy.color() == Color::Red);
    REQUIRE(meshCopy.vertexCount() == mesh.vertexCount());
    REQUIRE(meshCopy.faceCount() == mesh.faceCount());
    REQUIRE(
        meshCopy.face(0).vertex(0).position() ==
        mesh.face(0).vertex(0).position());

    // Deep copy check: modifying copy must not affect original
    meshCopy.vertex(0).position() = Point3f(99.0f, 99.0f, 99.0f);
    REQUIRE(mesh.vertex(0).position() != Point3f(99.0f, 99.0f, 99.0f));
}

inline void testMeshMove(MyMesh meshToMove)
{
    std::size_t origVCount = meshToMove.vertexCount();
    std::size_t origFCount = meshToMove.faceCount();
    MyMesh      meshMoved  = std::move(meshToMove);

    // Verify moved-to mesh
    REQUIRE(meshMoved.color() == Color::Red);
    REQUIRE(meshMoved.vertexCount() == origVCount);
    REQUIRE(meshMoved.faceCount() == origFCount);
    REQUIRE(!meshMoved.empty());
    REQUIRE(meshMoved.isCompact());
    REQUIRE(
        meshMoved.face(0).vertex(0).position() == Point3f(0.0f, 0.0f, 1.0f));

    // Verify moved-from mesh is in a valid empty state
    REQUIRE(meshToMove.vertexCount() == 0);
    REQUIRE(meshToMove.faceCount() == 0);
    REQUIRE(meshToMove.vertexContainerSize() == 0);
    REQUIRE(meshToMove.faceContainerSize() == 0);
    REQUIRE(meshToMove.deletedVertexCount() == 0);
    REQUIRE(meshToMove.deletedFaceCount() == 0);
    REQUIRE(meshToMove.empty());
    REQUIRE(meshToMove.isCompact());

    // Verify moved-from mesh is fully functional and can receive new elements
    uint v0 = meshToMove.addVertex();
    REQUIRE(v0 == 0);
    REQUIRE(meshToMove.vertexCount() == 1);
    REQUIRE(meshToMove.vertexContainerSize() == 1);
    meshToMove.vertex(0).position() = Point3f(1.0f, 2.0f, 3.0f);
    REQUIRE(meshToMove.vertex(0).position() == Point3f(1.0f, 2.0f, 3.0f));

    uint v1 = meshToMove.addVertex();
    uint v2 = meshToMove.addVertex();
    uint f0 = meshToMove.addFace();
    REQUIRE(f0 == 0);
    meshToMove.face(0).setVertices(v0, v1, v2);
    REQUIRE(
        meshToMove.face(0).vertex(0).position() == Point3f(1.0f, 2.0f, 3.0f));
}

inline void testMeshSwap(MyMesh& mesh)
{
    std::size_t origVCount = mesh.vertexCount();
    MyMesh      meshSwapped;
    swap(mesh, meshSwapped);
    REQUIRE(meshSwapped.color() == Color::Red);
    REQUIRE(meshSwapped.vertexCount() == origVCount);
    REQUIRE(mesh.vertexCount() == 0);

    // Swap back
    swap(mesh, meshSwapped);
    REQUIRE(mesh.vertexCount() == origVCount);
}

inline void testMeshAssignment(const MyMesh& mesh)
{
    MyMesh meshAssigned;
    meshAssigned = mesh; // Copy assignment
    REQUIRE(meshAssigned.color() == Color::Red);
    REQUIRE(meshAssigned.vertexCount() == mesh.vertexCount());

    MyMesh meshMoveAssigned;
    meshMoveAssigned = std::move(meshAssigned); // Move assignment
    REQUIRE(meshMoveAssigned.color() == Color::Red);
    REQUIRE(meshMoveAssigned.vertexCount() == mesh.vertexCount());
    REQUIRE(meshMoveAssigned.faceCount() == mesh.faceCount());
    REQUIRE(!meshMoveAssigned.empty());

    // Verify moved-from mesh is in a valid empty state
    REQUIRE(meshAssigned.vertexCount() == 0);
    REQUIRE(meshAssigned.faceCount() == 0);
    REQUIRE(meshAssigned.vertexContainerSize() == 0);
    REQUIRE(meshAssigned.faceContainerSize() == 0);
    REQUIRE(meshAssigned.deletedVertexCount() == 0);
    REQUIRE(meshAssigned.deletedFaceCount() == 0);
    REQUIRE(meshAssigned.empty());
    REQUIRE(meshAssigned.isCompact());

    // Verify moved-from mesh is fully functional
    uint v0 = meshAssigned.addVertex();
    REQUIRE(v0 == 0);
    REQUIRE(meshAssigned.vertexCount() == 1);
    meshAssigned.vertex(0).position() = Point3f(4.0f, 5.0f, 6.0f);
    REQUIRE(meshAssigned.vertex(0).position() == Point3f(4.0f, 5.0f, 6.0f));

    // Self-move assignment
    meshMoveAssigned = std::move(meshMoveAssigned);
    REQUIRE(meshMoveAssigned.vertexCount() == mesh.vertexCount());
    REQUIRE(meshMoveAssigned.faceCount() == mesh.faceCount());
}

inline void testMeshMoveNonCompact()
{
    MyMesh mesh = createSampleQuadMesh();
    mesh.deleteVertex(1);
    REQUIRE(mesh.vertexCount() == 3);
    REQUIRE(mesh.vertexContainerSize() == 4);
    REQUIRE(mesh.deletedVertexCount() == 1);
    REQUIRE(!mesh.isCompact());

    // Move construction with deleted elements
    MyMesh moved = std::move(mesh);

    // Verify moved-from mesh
    REQUIRE(mesh.vertexCount() == 0);
    REQUIRE(mesh.vertexContainerSize() == 0);
    REQUIRE(mesh.deletedVertexCount() == 0);
    REQUIRE(mesh.empty());
    REQUIRE(mesh.isCompact());

    // Verify moved-to mesh retains deleted elements and active state
    REQUIRE(moved.vertexCount() == 3);
    REQUIRE(moved.vertexContainerSize() == 4);
    REQUIRE(moved.deletedVertexCount() == 1);
    REQUIRE(!moved.isCompact());
    REQUIRE(moved.vertex(1).deleted());
    REQUIRE(!moved.vertex(0).deleted());

    // Move assignment with deleted elements
    MyMesh assignedMoved;
    assignedMoved = std::move(moved);

    REQUIRE(moved.vertexCount() == 0);
    REQUIRE(moved.vertexContainerSize() == 0);
    REQUIRE(moved.deletedVertexCount() == 0);
    REQUIRE(moved.empty());
    REQUIRE(moved.isCompact());

    REQUIRE(assignedMoved.vertexCount() == 3);
    REQUIRE(assignedMoved.vertexContainerSize() == 4);
    REQUIRE(assignedMoved.deletedVertexCount() == 1);
    REQUIRE(!assignedMoved.isCompact());
    REQUIRE(assignedMoved.vertex(1).deleted());
    REQUIRE(!assignedMoved.vertex(0).deleted());
}

inline void testParallelVectorsTupleMove()
{
    ParallelVectorsTuple<int, float, Point3f> tuple1;
    tuple1.enable<0>();
    tuple1.enable<1>();
    tuple1.enable<2>();
    tuple1.resize(5);
    tuple1.get<int>()[0]     = 42;
    tuple1.get<float>()[1]   = 3.14f;
    tuple1.get<Point3f>()[2] = Point3f(1.0f, 2.0f, 3.0f);

    // Copy construct
    ParallelVectorsTuple<int, float, Point3f> tupleCopy = tuple1;
    REQUIRE(tupleCopy.size() == 5);
    REQUIRE(tupleCopy.get<int>()[0] == 42);

    // Copy assign
    ParallelVectorsTuple<int, float, Point3f> tupleCopyAssigned;
    tupleCopyAssigned = tuple1;
    REQUIRE(tupleCopyAssigned.size() == 5);
    REQUIRE(tupleCopyAssigned.get<int>()[0] == 42);

    // Move construct
    ParallelVectorsTuple<int, float, Point3f> tuple2 = std::move(tuple1);
    REQUIRE(tuple1.size() == 0);
    REQUIRE(tuple2.size() == 5);
    REQUIRE(tuple2.get<int>()[0] == 42);
    REQUIRE(tuple2.get<float>()[1] == 3.14f);
    REQUIRE(tuple2.get<Point3f>()[2] == Point3f(1.0f, 2.0f, 3.0f));

    // Move assign
    tuple1 = std::move(tuple2);
    REQUIRE(tuple2.size() == 0);
    REQUIRE(tuple1.size() == 5);
    REQUIRE(tuple1.get<int>()[0] == 42);
    REQUIRE(tuple1.get<float>()[1] == 3.14f);
    REQUIRE(tuple1.get<Point3f>()[2] == Point3f(1.0f, 2.0f, 3.0f));

    // Self-move assign
    tuple1 = std::move(tuple1);
    REQUIRE(tuple1.size() == 5);
    REQUIRE(tuple1.get<int>()[0] == 42);

    // Swap
    ParallelVectorsTuple<int, float, Point3f> tuple3;
    tuple3.enable<0>();
    tuple3.resize(2);
    tuple3.get<int>()[0] = 99;

    swap(tuple1, tuple3);
    REQUIRE(tuple1.size() == 2);
    REQUIRE(tuple1.get<int>()[0] == 99);
    REQUIRE(tuple3.size() == 5);
    REQUIRE(tuple3.get<int>()[0] == 42);
}

inline void testElementContainerMove()
{
    using VCont = MyMesh::ContainerType<ElemId::VERTEX>;
    VCont cont1;
    cont1.resizeElements(4);
    REQUIRE(cont1.elementCount() == 4);
    REQUIRE(cont1.elementContainerSize() == 4);
    REQUIRE(cont1.isCompact());

    // Copy construct
    VCont contCopy = cont1;
    REQUIRE(contCopy.elementCount() == 4);
    REQUIRE(contCopy.elementContainerSize() == 4);

    // Copy assign
    VCont contCopyAssigned;
    contCopyAssigned = cont1;
    REQUIRE(contCopyAssigned.elementCount() == 4);
    REQUIRE(contCopyAssigned.elementContainerSize() == 4);

    // Move construct
    VCont cont2 = std::move(cont1);
    REQUIRE(cont1.elementCount() == 0);
    REQUIRE(cont1.elementContainerSize() == 0);
    REQUIRE(cont1.isCompact());
    REQUIRE(cont2.elementCount() == 4);
    REQUIRE(cont2.elementContainerSize() == 4);

    // Move assign
    cont1 = std::move(cont2);
    REQUIRE(cont2.elementCount() == 0);
    REQUIRE(cont2.elementContainerSize() == 0);
    REQUIRE(cont2.isCompact());
    REQUIRE(cont1.elementCount() == 4);
    REQUIRE(cont1.elementContainerSize() == 4);

    // Self-move assign
    cont1 = std::move(cont1);
    REQUIRE(cont1.elementCount() == 4);
    REQUIRE(cont1.elementContainerSize() == 4);

    // Swap
    VCont cont3;
    cont3.resizeElements(1);
    swap(cont1, cont3);
    REQUIRE(cont1.elementCount() == 1);
    REQUIRE(cont3.elementCount() == 4);
}

TEST_CASE("Lifetime")
{
    MyMesh mesh = createSampleQuadMesh();

    testElementValidity(mesh);
    testMeshCopy(mesh);
    testMeshMove(createSampleQuadMesh());
    testMeshSwap(mesh);
    testMeshAssignment(mesh);
    testMeshMoveNonCompact();
    testParallelVectorsTupleMove();
    testElementContainerMove();
}
