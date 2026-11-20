// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

inline void testConstMeshColors(const MyMesh& constMesh)
{
    // Mesh color const correctness
    REQUIRE(constMesh.color() == Color::Red);
    static_assert(
        std::is_same_v<decltype(constMesh.color()), const vcl::Color&>);

    // Vertex & Face color const correctness
    REQUIRE(constMesh.vertex(0).color() == Color::Blue);
    static_assert(std::is_same_v<
                  decltype(constMesh.vertex(0).color()),
                  const vcl::Color&>);
    REQUIRE(constMesh.face(0).color() == Color::DarkRed);
    static_assert(
        std::is_same_v<decltype(constMesh.face(0).color()), const vcl::Color&>);
}

inline void testConstComponentViews(const MyMesh& constMesh)
{
    auto constPosView = constMesh.vertices().positions();
    REQUIRE(constPosView.size() == 4);
    REQUIRE(constPosView[0] == Point3f(0.0f, 0.0f, 1.0f));
    static_assert(std::is_same_v<decltype(constPosView[0]), const Point3f&>);
    static_assert(
        std::is_same_v<decltype(constPosView.data()), const Point3f*>);

    for (const Point3f& p : constMesh.vertices().positions()) {
        static_assert(std::is_const_v<std::remove_reference_t<decltype(p)>>);
        (void) p;
    }

    for (const auto& vi : constMesh.face(0).vertexIndices()) {
        static_assert(std::is_const_v<std::remove_reference_t<decltype(vi)>>);
        (void) vi;
    }
}

inline void testConstElementProxies(MyMesh& mesh, const MyMesh& constMesh)
{
    // Access returns ConstVertex / ConstFace proxies
    MyMesh::ConstVertex cv0 = constMesh.vertex(0);
    REQUIRE(cv0.position() == Point3f(0.0f, 0.0f, 1.0f));

    for (MyMesh::ConstVertex v : constMesh.face(0).vertices()) {
        REQUIRE(v.isValid());
    }

    // Conversion from mutable Vertex to ConstVertex
    MyMesh::ConstVertex cvFromMutable = mesh.vertex(2);
    REQUIRE(cvFromMutable.index() == 2);

    // Test vertexIndices() view (mutable vs const)
    auto f0               = mesh.face(0);
    f0.vertexIndices()[1] = 1; // Mutate via view operator[]
    for (auto& vi : f0.vertexIndices()) {
        static_assert(!std::is_const_v<std::remove_reference_t<decltype(vi)>>);
        vi = vi; // Mutate via reference in range-for
    }
}

inline void testProxyAndComponentViewConversions(
    MyMesh&       mesh,
    const MyMesh& constMesh)
{
    // ElementContainerProxy mutable to const conversion
    auto                           vProxy      = mesh.vertices();
    decltype(constMesh.vertices()) constVProxy = vProxy;
    REQUIRE(constVProxy.size() == 4);

    // ComponentView mutable to const conversion
    auto posView = mesh.vertices().positions();
    decltype(constMesh.vertices().positions()) constPosView = posView;
    REQUIRE(constPosView.size() == 4);
    REQUIRE(constPosView[0] == Point3f(0.0f, 0.0f, 1.0f));
}

inline void testIteratorArithmetic(MyMesh& mesh)
{
    auto it  = mesh.vertices().begin();
    auto it2 = it + 2;
    // Test operator-(integral) when literal 1 is passed
    auto it1 = it2 - 1;
    REQUIRE(it1.index() == 1);
    // Test operator-(iterator) difference
    REQUIRE((it2 - it1) == 1);
    REQUIRE((it1 - it) == 1);
    // Test friend operator+(integral, it)
    auto it1_from_left = 1 + it;
    REQUIRE(it1_from_left == it1);
}

inline void testIndexIfCompact(MyMesh& mesh)
{
    // Test indexIfCompact<ELEM_ID>(i)
    REQUIRE(mesh.indexIfCompact<ElemId::VERTEX>(0) == 0);
    REQUIRE(mesh.indexIfCompact<ElemId::VERTEX>(1) == 1);
    REQUIRE(mesh.indexIfCompact<ElemId::VERTEX>(2) == 2);

    mesh.deleteVertex(1);
    REQUIRE(mesh.indexIfCompact<ElemId::VERTEX>(0) == 0);
    REQUIRE(mesh.indexIfCompact<ElemId::VERTEX>(1) == MyMesh::NULL_INDEX);
    REQUIRE(mesh.indexIfCompact<ElemId::VERTEX>(2) == 1);
}

TEST_CASE("Const Correctness")
{
    MyMesh        mesh      = createSampleQuadMesh();
    const MyMesh& constMesh = mesh;

    testConstMeshColors(constMesh);
    testConstComponentViews(constMesh);
    testConstElementProxies(mesh, constMesh);
    testProxyAndComponentViewConversions(mesh, constMesh);
    testIteratorArithmetic(mesh);
    testIndexIfCompact(mesh);
}
