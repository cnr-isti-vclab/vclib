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

#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

using Meshes  = std::tuple<vcl::TriMesh, vcl::PolyMesh>;
using Meshesf = std::tuple<vcl::TriMeshf, vcl::PolyMeshf>;
using MeshesIndexed =
    std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed>;
using MeshesIndexedf = std::
    tuple<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf>;

// Test to load obj from a istringstream
TEMPLATE_TEST_CASE(
    "Load gltf",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    vcl::MeshInfo info;

    SECTION("Load Duck.gltf")
    {
        info.clear();

        TriMesh tm;

        vcl::loadGltf(tm, VCLIB_EXAMPLE_MESHES_PATH "/gltf/Duck/Duck.gltf", info);
        REQUIRE(tm.vertexNumber() == 2399);
        REQUIRE(tm.faceNumber() == 4212);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(!info.hasEdges());
    }
}
