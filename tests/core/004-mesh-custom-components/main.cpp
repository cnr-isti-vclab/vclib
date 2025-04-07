/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh m;
    m.addVertices(10);

    m.template addPerVertexCustomComponent<int>("flag");

    REQUIRE(m.hasPerVertexCustomComponent("flag"));

    for (typename TriMesh::Vertex& v : m.vertices()) {
        v.template customComponent<int>("flag") = -4;
    }

    REQUIRE(m.vertex(3).template customComponent<int>("flag") == -4);

    vcl::CustomComponentVectorHandle<int> v =
        m.template perVertexCustomComponentVectorHandle<int>("flag");

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

    m.deletePerVertexCustomComponent("flag");

    REQUIRE(!m.hasPerVertexCustomComponent("flag"));
}
