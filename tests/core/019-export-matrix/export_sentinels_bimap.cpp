// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "helpers.h"

#include <vclib/algorithms/mesh/import_export/export_matrix.h>
#include <vclib/algorithms/mesh/update/topology.h>

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <vector>

TEST_CASE(
    "Export Material Index and Adjacency Sentinels",
    "[export][sentinels]")
{
    using TriMesh = vcl::TriMesh;
    TriMesh mesh;
    mesh.addVertices(3);
    mesh.addFace(0, 1, 2);

    SECTION("Material Index Sentinel Export")
    {
        mesh.enableVertexMaterialIndex();
        mesh.enableFaceMaterialIndex();

        mesh.vertex(0).materialIndex() = 0;
        mesh.vertex(1).materialIndex() = 42;
        mesh.vertex(2).materialIndex() = vcl::USHORT_NULL;

        mesh.face(0).materialIndex() = vcl::USHORT_NULL;

        auto vMatInt = vcl::vertexMaterialIndicesVector<Eigen::VectorXi>(mesh);
        REQUIRE(vMatInt.size() == 3);
        REQUIRE(vMatInt(0) == 0);
        REQUIRE(vMatInt(1) == 42);
        REQUIRE(vMatInt(2) == -1);

        auto vMatDouble =
            vcl::vertexMaterialIndicesVector<Eigen::VectorXd>(mesh);
        REQUIRE(vMatDouble.size() == 3);
        REQUIRE(vMatDouble(0) == 0.0);
        REQUIRE(vMatDouble(1) == 42.0);
        REQUIRE(vMatDouble(2) == -1.0);

        auto fMatInt = vcl::faceMaterialIndicesVector<Eigen::VectorXi>(mesh);
        REQUIRE(fMatInt.size() == 1);
        REQUIRE(fMatInt(0) == -1);

        auto fMatDouble = vcl::faceMaterialIndicesVector<Eigen::VectorXd>(mesh);
        REQUIRE(fMatDouble.size() == 1);
        REQUIRE(fMatDouble(0) == -1.0);
    }

    SECTION("64-bit Adjacency Sentinel Export")
    {
        mesh.enableFaceAdjacentFaces();
        vcl::updateFaceAdjacentFaces(mesh);

        // Since it's a single triangle, all adjacent faces are NULL_INDEX
        auto fAdj64 =
            vcl::faceAdjacentFacesMatrix<EigenRowMatrix<int64_t>>(mesh);
        REQUIRE(fAdj64.rows() == 1);
        REQUIRE(fAdj64.cols() == 3);
        REQUIRE(fAdj64(0, 0) == -1);
        REQUIRE(fAdj64(0, 1) == -1);
        REQUIRE(fAdj64(0, 2) == -1);

        auto fAdjInt = vcl::faceAdjacentFacesMatrix<EigenRowMatrix<int>>(mesh);
        REQUIRE(fAdjInt(0, 0) == -1);
        REQUIRE(fAdjInt(0, 1) == -1);
        REQUIRE(fAdjInt(0, 2) == -1);
    }
}

TEST_CASE("TriPolyIndexBiMap - isMappingTrivial and exportBuffer cleanup")
{
    SECTION("isMappingTrivial")
    {
        vcl::TriPolyIndexBiMap map;
        REQUIRE(map.isMappingTrivial());

        // Identity 1:1 mapping: 0->0, 1->1, 2->2
        map.insert(0, 0);
        map.insert(1, 1);
        map.insert(2, 2);
        REQUIRE(map.isMappingTrivial());

        // Permuted mapping: polygon 0 -> triangle 1, polygon 1 -> triangle 0
        vcl::TriPolyIndexBiMap permutedMap;
        permutedMap.insert(1, 0);
        permutedMap.insert(0, 1);
        REQUIRE_FALSE(permutedMap.isMappingTrivial());

        // Non-trivial 1:N mapping (quad triangulated into 2 triangles)
        vcl::TriPolyIndexBiMap multiMap;
        multiMap.insert(0, 0);
        multiMap.insert(1, 0);
        REQUIRE_FALSE(multiMap.isMappingTrivial());

        // Non-trivial mapping with gap (deleted face in middle)
        vcl::TriPolyIndexBiMap gapMap;
        gapMap.insert(0, 0);
        gapMap.insert(1, 2); // polygon 1 was deleted
        REQUIRE_FALSE(gapMap.isMappingTrivial());
    }

    SECTION(
        "triangulatedFaceVertexIndicesToBuffer clears detail::indexMap on "
        "TriMesh")
    {
        vcl::TriMesh tm;
        tm.addVertices(3);
        tm.vertex(0).position() = vcl::Point3d(0, 0, 0);
        tm.vertex(1).position() = vcl::Point3d(1, 0, 0);
        tm.vertex(2).position() = vcl::Point3d(0, 1, 0);
        tm.addFace(0, 1, 2);

        std::vector<vcl::uint> buffer(3);
        // Call using default indexMap argument (&detail::indexMap)
        vcl::triangulatedFaceVertexIndicesToBuffer(tm, buffer.data());

        // detail::indexMap should be empty after the call
        REQUIRE(vcl::detail::indexMap.triangleCount() == 0);
        REQUIRE(vcl::detail::indexMap.polygonCount() == 0);
    }
}
