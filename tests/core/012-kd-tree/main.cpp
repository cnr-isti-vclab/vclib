// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
