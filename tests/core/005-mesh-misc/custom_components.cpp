// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Custom Components - Mesh Custom Components")
{
    // Concepts check
    static_assert(mesh::HasCustomComponents<CCMesh>);
    static_assert(!mesh::HasCustomComponents<MyMesh>);

    CCMesh m;

    // Initially no components
    REQUIRE(!m.hasCustomComponent("int_val"));
    REQUIRE(m.customComponentNames().empty());

    // Add custom components
    m.addCustomComponent<int>("int_val", 42);
    m.addCustomComponent<std::string>("str_val", std::string("vclib"));
    m.addCustomComponent<double>("scale", 3.14159);

    REQUIRE(m.hasCustomComponent("int_val"));
    REQUIRE(m.hasCustomComponent("str_val"));
    REQUIRE(m.hasCustomComponent("scale"));
    REQUIRE(!m.hasCustomComponent("non_existent"));

    // Type checking
    REQUIRE(m.isCustomComponentOfType<int>("int_val"));
    REQUIRE(!m.isCustomComponentOfType<float>("int_val"));
    REQUIRE(m.isCustomComponentOfType<std::string>("str_val"));
    REQUIRE(m.customComponentType("int_val") == typeid(int));
    REQUIRE(m.customComponentType("str_val") == typeid(std::string));

    // Names
    auto names = m.customComponentNames();
    REQUIRE(names.size() == 3);
    auto intNames = m.customComponentNamesOfType<int>();
    REQUIRE((intNames.size() == 1 && intNames[0] == "int_val"));

    // Read and modify
    REQUIRE(m.customComponent<int>("int_val") == 42);
    m.customComponent<int>("int_val") = 100;
    REQUIRE(m.customComponent<int>("int_val") == 100);

    // Const access
    const CCMesh& cm = m;
    REQUIRE(cm.customComponent<int>("int_val") == 100);
    REQUIRE(cm.customComponent<std::string>("str_val") == "vclib");

    // Exceptions
    bool caughtNotFound = false;
    try {
        (void) m.customComponent<int>("missing");
    }
    catch (const std::out_of_range&) {
        caughtNotFound = true;
    }
    REQUIRE(caughtNotFound);

    bool caughtWrongType = false;
    try {
        (void) m.customComponent<float>("int_val");
    }
    catch (const vcl::BadCustomComponentTypeException&) {
        caughtWrongType = true;
    }
    REQUIRE(caughtWrongType);

    // Deletion
    m.deleteCustomComponent("scale");
    REQUIRE(!m.hasCustomComponent("scale"));
    REQUIRE(m.customComponentNames().size() == 2);

    // Clear
    m.clearCustomComponents();
    REQUIRE(!m.hasCustomComponent("int_val"));
    REQUIRE(m.customComponentNames().empty());
}

TEST_CASE("Custom Components - Element Custom Components")
{
    // Concepts check
    static_assert(HasVertexCustomComponents<CCMesh>);
    static_assert(HasFaceCustomComponents<CCMesh>);
    static_assert(!HasVertexCustomComponents<MyMesh>);
    static_assert(!HasFaceCustomComponents<MyMesh>);

    static_assert(vert::HasCustomComponents<CCMesh::Vertex>);
    static_assert(face::HasCustomComponents<CCMesh::Face>);

    CCMesh m;
    m.resizeVertices(4);
    m.resizeFaces(2);

    m.vertex(0).position() = Point3f(0.0f, 0.0f, 0.0f);
    m.vertex(1).position() = Point3f(1.0f, 0.0f, 0.0f);
    m.vertex(2).position() = Point3f(0.0f, 1.0f, 0.0f);
    m.vertex(3).position() = Point3f(1.0f, 1.0f, 0.0f);

    m.face(0).setVertices(0, 1, 2);
    m.face(1).setVertices(1, 3, 2);

    // Add vertex and face custom components
    m.addVertexCustomComponent<double>("curvature");
    m.addFaceCustomComponent<int>("mat_id");

    // Container / Mesh level queries
    REQUIRE(m.hasVertexCustomComponent("curvature"));
    REQUIRE(!m.hasVertexCustomComponent("non_existent"));
    REQUIRE(m.hasFaceCustomComponent("mat_id"));
    REQUIRE(m.isVertexCustomComponentOfType<double>("curvature"));
    REQUIRE(m.vertexCustomComponentType("curvature") == typeid(double));
    REQUIRE(m.faceCustomComponentType("mat_id") == typeid(int));

    // Generic element queries
    REQUIRE(m.hasElementCustomComponent<ElemId::VERTEX>("curvature"));
    REQUIRE(m.hasElementCustomComponent<ElemId::FACE>("mat_id"));
    REQUIRE((
        m.isElementCustomComponentOfType<ElemId::VERTEX, double>("curvature")));
    REQUIRE(
        m.elementCustomComponentType<ElemId::VERTEX>("curvature") ==
        typeid(double));

    // Proxy access: write and read
    for (uint i = 0; i < 4; ++i) {
        REQUIRE(m.vertex(i).hasCustomComponent("curvature"));
        REQUIRE(m.vertex(i).isCustomComponentOfType<double>("curvature"));
        m.vertex(i).customComponent<double>("curvature") = 1.0 + i * 0.5;
    }

    m.face(0).customComponent<int>("mat_id") = 10;
    m.face(1).customComponent<int>("mat_id") = 20;

    for (uint i = 0; i < 4; ++i) {
        REQUIRE(
            m.vertex(i).customComponent<double>("curvature") == 1.0 + i * 0.5);
    }
    REQUIRE(m.face(0).customComponent<int>("mat_id") == 10);
    REQUIRE(m.face(1).customComponent<int>("mat_id") == 20);

    // Const proxy access
    const CCMesh& cm = m;
    REQUIRE(cm.vertex(2).customComponent<double>("curvature") == 2.0);
    REQUIRE(cm.face(1).customComponent<int>("mat_id") == 20);
}

TEST_CASE("Custom Components - Custom Component Handles")
{
    CCMesh m;
    m.resizeVertices(3);

    m.addVertexCustomComponent<int>("weight");
    auto h = m.vertexCustomComponentVectorHandle<int>("weight");

    REQUIRE(h.size() == 3);
    REQUIRE(!h.empty());

    // Write via handle operator[]
    for (uint i = 0; i < 3; ++i) {
        h[i] = static_cast<int>(10 * (i + 1));
    }

    // Verify reflection on proxies
    REQUIRE(m.vertex(0).customComponent<int>("weight") == 10);
    REQUIRE(m.vertex(1).customComponent<int>("weight") == 20);
    REQUIRE(m.vertex(2).customComponent<int>("weight") == 30);

    // Range-based for loop over handle
    int sum = 0;
    for (int val : h) {
        sum += val;
    }
    REQUIRE(sum == 60);

    // front, back, at
    REQUIRE(h.front() == 10);
    REQUIRE(h.back() == 30);
    REQUIRE(h.at(1) == 20);

    // Const handle
    const CCMesh& cm = m;
    auto          ch = cm.vertexCustomComponentVectorHandle<int>("weight");
    REQUIRE(ch.size() == 3);
    REQUIRE(ch[1] == 20);

    // Converting constructor from handle to const handle
    ConstCustomComponentVectorHandle<int> ch2 = h;
    REQUIRE(ch2[2] == 30);
}

TEST_CASE("Custom Components - Custom Components Lifecycle")
{
    // 1. Lazy initialization on resize
    {
        CCMesh m;
        m.resizeVertices(2);
        m.addVertexCustomComponent<float>("scalar");

        m.vertex(0).customComponent<float>("scalar") = 4.5f;
        m.vertex(1).customComponent<float>("scalar") = 5.5f;

        // Resize larger: new elements are uninitialized until accessed
        m.addVertices(2); // total 4
        REQUIRE(m.vertex(0).customComponent<float>("scalar") == 4.5f);
        REQUIRE(m.vertex(1).customComponent<float>("scalar") == 5.5f);
        REQUIRE(m.vertex(2).customComponent<float>("scalar") == 0.0f);
        REQUIRE(m.vertex(3).customComponent<float>("scalar") == 0.0f);
    }

    // 2. Compaction
    {
        CCMesh m;
        m.resizeVertices(4);
        m.addVertexCustomComponent<int>("id");
        for (uint i = 0; i < 4; ++i) {
            m.vertex(i).customComponent<int>("id") = static_cast<int>(i * 10);
        }

        m.deleteVertex(1); // delete element at index 1
        REQUIRE(m.vertexCount() == 3);
        REQUIRE(m.vertexContainerSize() == 4);

        m.compactVertices();
        REQUIRE(m.vertexCount() == 3);
        REQUIRE(m.vertexContainerSize() == 3);

        // Remaining elements should be 0, 2, 3 -> compacted to 0, 1, 2
        REQUIRE(m.vertex(0).customComponent<int>("id") == 0);
        REQUIRE(m.vertex(1).customComponent<int>("id") == 20);
        REQUIRE(m.vertex(2).customComponent<int>("id") == 30);
    }

    // 3. Append
    {
        CCMesh m1;
        m1.resizeVertices(2);
        m1.addVertexCustomComponent<int>("tag");
        m1.vertex(0).customComponent<int>("tag") = 1;
        m1.vertex(1).customComponent<int>("tag") = 2;

        CCMesh m2;
        m2.resizeVertices(2);
        m2.addVertexCustomComponent<int>("tag");
        m2.vertex(0).customComponent<int>("tag") = 10;
        m2.vertex(1).customComponent<int>("tag") = 20;

        m1.append(m2);

        REQUIRE(m1.vertexCount() == 4);
        REQUIRE(m1.vertex(0).customComponent<int>("tag") == 1);
        REQUIRE(m1.vertex(1).customComponent<int>("tag") == 2);
        REQUIRE(m1.vertex(2).customComponent<int>("tag") == 10);
        REQUIRE(m1.vertex(3).customComponent<int>("tag") == 20);
    }

    // 4. Mesh importFrom
    {
        CCMesh src;
        src.resizeVertices(2);
        src.addCustomComponent<std::string>("mesh_tag", "imported");
        src.addVertexCustomComponent<double>("attr");
        src.vertex(0).customComponent<double>("attr") = 9.9;
        src.vertex(1).customComponent<double>("attr") = 8.8;

        CCMesh dst;
        dst.importFrom(src);

        REQUIRE(dst.hasCustomComponent("mesh_tag"));
        REQUIRE(dst.customComponent<std::string>("mesh_tag") == "imported");
        REQUIRE(dst.vertexCount() == 2);
        REQUIRE(dst.hasVertexCustomComponent("attr"));
        REQUIRE(dst.vertex(0).customComponent<double>("attr") == 9.9);
        REQUIRE(dst.vertex(1).customComponent<double>("attr") == 8.8);
    }
}
