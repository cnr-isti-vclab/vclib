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
std::vector<std::size_t> getKNearestNeighbors(
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

    using PointType = TriMesh::Vertex::PositionType;

    const PointType p(0.5, 0.5, 0.5);

    REQUIRE(getKNearestNeighbors<TriMesh>(p, 1)[0] == 1558);
    REQUIRE(
        getKNearestNeighbors<TriMesh>(p, 5) ==
        std::vector<std::size_t> {1558, 1613, 1720, 1576, 163});

    TriMesh m = vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bone.ply");
    vcl::KDTree tree(m);

    typename PointType::Scalar dist  = 0;
    std::size_t                nnIdx = tree.nearestNeighborIndex(p, dist);
    PointType                  nnPt  = tree.nearestNeighbor(p);
    REQUIRE(nnIdx == 1558);
    REQUIRE(nnPt == m.vertex(1558).position());

    auto knnIndices = tree.kNearestNeighborsIndices(p, 5);
    auto knnPoints  = tree.kNearestNeighbors(p, 5);
    REQUIRE(knnIndices.size() == 5);
    REQUIRE(knnPoints.size() == 5);
    for (std::size_t i = 0; i < 5; ++i) {
        REQUIRE(knnPoints[i] == m.vertex(knnIndices[i]).position());
    }

    std::vector<typename PointType::Scalar> distances;
    auto distIndices = tree.neighborsIndicesInDistance(p, 0.1, distances);
    auto distPoints  = tree.neighborsInDistance(p, 0.1);
    REQUIRE(distIndices.size() == distPoints.size());
    for (std::size_t i = 0; i < distIndices.size(); ++i) {
        REQUIRE(distPoints[i] == m.vertex(distIndices[i]).position());
    }
}

TEMPLATE_TEST_CASE(
    "KD-Tree on non-compact mesh with deleted vertices",
    "",
    vcl::TriMesh,
    vcl::TriMeshf)
{
    using TriMesh   = TestType;
    using PointType = typename TriMesh::Vertex::PositionType;
    using Scalar    = typename PointType::Scalar;

    TriMesh m;
    m.addVertex(PointType(0, 0, 0));    // index 0 (will delete)
    m.addVertex(PointType(10, 0, 0));   // index 1
    m.addVertex(PointType(0, 10, 0));   // index 2 (will delete)
    m.addVertex(PointType(0, 0, 10));   // index 3
    m.addVertex(PointType(20, 20, 20)); // index 4

    m.deleteVertex(0);
    m.deleteVertex(2);

    REQUIRE(m.vertexCount() == 3);
    REQUIRE(m.vertexContainerSize() == 5);
    REQUIRE(!m.isCompact());

    vcl::KDTree tree(m);

    PointType   q(9.9, 0.1, 0.0);
    Scalar      dist = 0;
    std::size_t idx  = tree.nearestNeighborIndex(q, dist);
    REQUIRE(idx == 1);
    PointType nearestP = tree.nearestNeighbor(q);
    REQUIRE(nearestP == PointType(10, 0, 0));

    auto knnIndices = tree.kNearestNeighborsIndices(q, 2);
    auto knnPoints  = tree.kNearestNeighbors(q, 2);
    REQUIRE(knnIndices.size() == 2);
    REQUIRE(knnPoints.size() == 2);
    REQUIRE(knnIndices[0] == 1);
    REQUIRE(knnPoints[0] == PointType(10, 0, 0));
    REQUIRE(knnIndices[1] == 3);
    REQUIRE(knnPoints[1] == PointType(0, 0, 10));

    // Distance search
    std::vector<Scalar> dists;
    auto inDistIdx = tree.neighborsIndicesInDistance(q, 1.0, dists);
    auto inDistPts = tree.neighborsInDistance(q, 1.0);
    REQUIRE(inDistIdx.size() == 1);
    REQUIRE(inDistIdx[0] == 1);
    REQUIRE(inDistPts.size() == 1);
    REQUIRE(inDistPts[0] == PointType(10, 0, 0));
}
