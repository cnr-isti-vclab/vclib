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
#include <vclib/space.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<typename MeshType, typename PointType>
std::vector<unsigned int> getKNearestNeighbors(
    const PointType& p,
    unsigned int     k,
    std::string      mesh = "bone.ply")
{
    MeshType m = vcl::loadMesh<MeshType>(VCLIB_EXAMPLE_MESHES_PATH "/" + mesh);

    vcl::KDTree tree(m);

    return tree.kNearestNeighborsIndices(p, k);
}

TEMPLATE_TEST_CASE(
    "KD-Tree nearest neighbors to [0.5, 0.5, 0.5] in bone.ply",
    "",
    vcl::TriMesh,
    vcl::TriMeshf)
{
    using TriMesh = TestType;

    using PointType = TriMesh::VertexType::PositionType;

    const PointType p(0.5, 0.5, 0.5);

    REQUIRE(getKNearestNeighbors<TriMesh>(p, 1)[0] == 1558);
    REQUIRE(
        getKNearestNeighbors<TriMesh>(p, 5) ==
        std::vector<unsigned int> {1558, 1613, 1720, 1576, 163});
}
