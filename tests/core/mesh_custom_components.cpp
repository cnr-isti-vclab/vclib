// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEMPLATE_TEST_CASE(
    "Test Custom Components and Handles",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMesh64,
    vcl::TriMesh64f)
{
    using TriMesh = TestType;

    TriMesh m;
    m.addVertices(10);

    m.template addVertexCustomComponent<int>("flag");

    REQUIRE(m.hasVertexCustomComponent("flag"));

    for (auto&& v : m.vertices()) {
        v.template customComponent<int>("flag") = -4;
    }

    REQUIRE(m.vertex(3).template customComponent<int>("flag") == -4);

    vcl::CustomComponentVectorHandle<int> v =
        m.template vertexCustomComponentVectorHandle<int>("flag");

    int tmp = 8;
    for (auto& m : v) {
        m = tmp;
    }
    int tmp2 = 4;

    v.front() = tmp2;

    m.vertex(1).template customComponent<int>("flag") = 2;

    REQUIRE(m.vertex(0).template customComponent<int>("flag") == 4);
    REQUIRE(tmp == 8);
    REQUIRE(m.vertex(9).template customComponent<int>("flag") == 8);

    // Test Iterator operator[] and random_access_iterator concept
    static_assert(std::random_access_iterator<
                  typename vcl::CustomComponentVectorHandle<int>::Iterator>);
    static_assert(
        std::random_access_iterator<
            typename vcl::CustomComponentVectorHandle<int>::ConstIterator>);
    auto it = v.begin();
    REQUIRE(it[0] == 4);
    REQUIRE(it[1] == 2);

    m.deleteVertexCustomComponent("flag");

    REQUIRE(!m.hasVertexCustomComponent("flag"));

    // Test replacement of a custom component with a different type
    m.template addVertexCustomComponent<int>("val");
    m.vertex(0).template customComponent<int>("val") = 42;
    m.template addVertexCustomComponent<float>("val");
    REQUIRE(m.template isVertexCustomComponentOfType<float>("val"));
    REQUIRE(!m.template isVertexCustomComponentOfType<int>("val"));
    REQUIRE(m.vertex(0).template customComponent<float>("val") == 0.0f);

    // Test resize and const access
    m.resizeVertices(20);
    const TriMesh& constM = m;
    REQUIRE(constM.vertex(15).template customComponent<float>("val") == 0.0f);
}
