// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "types.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/base.h>
#include <vclib/meshes.h>
#include <vclib/space/complex.h>

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cstdint>
#include <set>
#include <type_traits>
#include <vector>

TEST_CASE("Shuffle - Shuffle Vertices")
{
    vcl::TriMesh mesh;
    for (uint i = 0; i < 20; ++i) {
        mesh.addVertex(Point3d(i, i * 2, i * 3));
    }
    REQUIRE(mesh.vertexCount() == 20);

    // 1. Shuffled vertex index vector
    auto indices = vcl::fillAndShuffleVertexIndexVector(mesh);
    static_assert(
        std::
            is_same_v<decltype(indices), std::vector<vcl::TriMesh::IndexType>>);
    REQUIRE(indices.size() == 20);

    std::set<uint> idxSet(indices.begin(), indices.end());
    REQUIRE(idxSet.size() == 20);
    for (uint i = 0; i < 20; ++i) {
        REQUIRE(idxSet.contains(i));
    }

    // 2. Shuffled vertex proxy vector (mutable)
    auto vVec = vcl::fillAndShuffleVertexVector(mesh);
    static_assert(std::is_same_v<
                  decltype(vVec),
                  std::vector<vcl::VertexType<vcl::TriMesh>>>);
    REQUIRE(vVec.size() == 20);
    std::set<uint> vIdxSet;
    for (auto v : vVec) {
        vIdxSet.insert(v.index());
    }
    REQUIRE(vIdxSet.size() == 20);

    // 3. Shuffled vertex proxy vector (const)
    const auto& cmesh = mesh;
    auto        cvVec = vcl::fillAndShuffleVertexVector(cmesh);
    static_assert(std::is_same_v<
                  decltype(cvVec),
                  std::vector<vcl::ConstVertexType<vcl::TriMesh>>>);
    REQUIRE(cvVec.size() == 20);

    // 4. Deterministic with seed
    auto ind1 = vcl::fillAndShuffleVertexIndexVector(mesh, 12345u);
    auto ind2 = vcl::fillAndShuffleVertexIndexVector(mesh, 12345u);
    REQUIRE(ind1 == ind2);

    auto ind3 = vcl::fillAndShuffleVertexIndexVector(mesh, 54321u);
    REQUIRE(ind1 != ind3);

    // 5. Mesh with deleted vertices
    mesh.deleteVertex(5);
    mesh.deleteVertex(10);
    REQUIRE(mesh.vertexCount() == 18);
    REQUIRE(mesh.deletedVertexCount() == 2);

    auto delIndices = vcl::fillAndShuffleVertexIndexVector(mesh);
    REQUIRE(delIndices.size() == 18);
    for (auto idx : delIndices) {
        REQUIRE((idx != 5 && idx != 10));
        REQUIRE(!mesh.vertex(idx).deleted());
    }

    auto delVVec = vcl::fillAndShuffleVertexVector(mesh);
    REQUIRE(delVVec.size() == 18);
    for (auto v : delVVec) {
        REQUIRE((v.index() != 5 && v.index() != 10));
        REQUIRE(!v.deleted());
    }
}

TEST_CASE("Shuffle - Shuffle Faces")
{
    vcl::TriMesh mesh;
    for (uint i = 0; i < 10; ++i) {
        mesh.addVertex(Point3d(i, 0, 0));
        mesh.addVertex(Point3d(i, 1, 0));
    }
    for (uint i = 0; i < 8; ++i) {
        mesh.addFace(i, i + 1, i + 2);
    }
    REQUIRE(mesh.faceCount() == 8);

    // 1. Shuffled face index vector
    auto fIndices = vcl::fillAndShuffleFaceIndexVector(mesh);
    static_assert(std::is_same_v<
                  decltype(fIndices),
                  std::vector<vcl::TriMesh::IndexType>>);
    REQUIRE(fIndices.size() == 8);

    std::set<uint> fIdxSet(fIndices.begin(), fIndices.end());
    REQUIRE(fIdxSet.size() == 8);
    for (uint i = 0; i < 8; ++i) {
        REQUIRE(fIdxSet.contains(i));
    }

    // 2. Shuffled face vector (mutable)
    auto fVec = vcl::fillAndShuffleFaceVector(mesh);
    static_assert(std::is_same_v<
                  decltype(fVec),
                  std::vector<vcl::FaceType<vcl::TriMesh>>>);
    REQUIRE(fVec.size() == 8);

    // 3. Shuffled face vector (const)
    const auto& cmesh = mesh;
    auto        cfVec = vcl::fillAndShuffleFaceVector(cmesh);
    static_assert(std::is_same_v<
                  decltype(cfVec),
                  std::vector<vcl::ConstFaceType<vcl::TriMesh>>>);
    REQUIRE(cfVec.size() == 8);

    // 4. Mesh with deleted faces
    mesh.deleteFace(2);
    REQUIRE(mesh.faceCount() == 7);
    auto delFIndices = vcl::fillAndShuffleFaceIndexVector(mesh);
    REQUIRE(delFIndices.size() == 7);
    for (auto idx : delFIndices) {
        REQUIRE(idx != 2);
        REQUIRE(!mesh.face(idx).deleted());
    }
}

TEST_CASE("Shuffle - Shuffle Edges")
{
    MyEdgeMesh mesh = createSampleEdgeMesh();
    REQUIRE(mesh.edgeCount() == 4);

    auto eIndices = vcl::fillAndShuffleEdgeIndexVector(mesh);
    static_assert(
        std::is_same_v<decltype(eIndices), std::vector<MyEdgeMesh::IndexType>>);
    REQUIRE(eIndices.size() == 4);

    auto eVec = vcl::fillAndShuffleEdgeVector(mesh);
    static_assert(
        std::is_same_v<decltype(eVec), std::vector<vcl::EdgeType<MyEdgeMesh>>>);
    REQUIRE(eVec.size() == 4);

    const auto& cmesh = mesh;
    auto        ceVec = vcl::fillAndShuffleEdgeVector(cmesh);
    static_assert(std::is_same_v<
                  decltype(ceVec),
                  std::vector<vcl::ConstEdgeType<MyEdgeMesh>>>);
    REQUIRE(ceVec.size() == 4);
}

TEST_CASE("Shuffle - Shuffle64 Bit")
{
    vcl::TriMesh64 mesh;
    for (uint i = 0; i < 15; ++i) {
        mesh.addVertex(Point3d(i, i, i));
    }
    for (uint i = 0; i < 10; ++i) {
        mesh.addFace(i, i + 1, i + 2);
    }

    auto vIndices = vcl::fillAndShuffleVertexIndexVector(mesh, 42u);
    static_assert(std::is_same_v<decltype(vIndices), std::vector<uint64_t>>);
    REQUIRE(vIndices.size() == 15);

    auto fIndices = vcl::fillAndShuffleFaceIndexVector(mesh, 42u);
    static_assert(std::is_same_v<decltype(fIndices), std::vector<uint64_t>>);
    REQUIRE(fIndices.size() == 10);
}

TEST_CASE("Shuffle - Sort Proxy Vector and StaticGrid Do Not Corrupt Mesh")
{
    vcl::Sphere<double> s(vcl::Point3d(0, 0, 0), 1.0);
    vcl::TriMesh        mesh = vcl::createSphereIcosahedron<vcl::TriMesh>(s, 1);
    // mesh.compactVertices();
    mesh.enableVertexColor();
    mesh.enableVertexPrincipalCurvature();

    std::vector<uint>         origIndices;
    std::vector<vcl::Point3d> origPositions;
    std::vector<vcl::Color>   origColors;
    origIndices.reserve(mesh.vertexCount());
    origPositions.reserve(mesh.vertexCount());
    origColors.reserve(mesh.vertexCount());

    for (auto v : mesh.vertices()) {
        origIndices.push_back(v.index());
        origPositions.push_back(v.position());
        v.color() = vcl::Color(
            static_cast<uint8_t>(v.index() % 255),
            static_cast<uint8_t>((v.index() * 3) % 255),
            static_cast<uint8_t>((v.index() * 7) % 255));
        origColors.push_back(v.color());
    }

    std::vector<std::array<uint, 3>> origFaces;
    origFaces.reserve(mesh.faceCount());
    for (auto f : mesh.faces()) {
        origFaces.push_back(
            {f.vertexIndex(0), f.vertexIndex(1), f.vertexIndex(2)});
    }

    // Sorting a vector of mutable Vertex proxies must only reorder handles
    auto vVec = vcl::fillAndShuffleVertexVector(mesh, 123u);
    std::sort(vVec.begin(), vVec.end());
    REQUIRE(vVec.size() == mesh.vertexCount());
    for (uint i = 0; i < vVec.size(); ++i) {
        REQUIRE(vVec[i].index() == origIndices[i]);
        REQUIRE(vVec[i].position() == origPositions[i]);
        REQUIRE(vVec[i].color() == origColors[i]);
    }

    // Building StaticGrid over mutable vertices/faces and running PCA curvature
    // must not mutate vertex positions, colors, or face topology
    vcl::StaticGrid vGrid(mesh.vertices());
    vcl::StaticGrid fGrid(mesh.faces());
    vcl::updatePrincipalCurvaturePCA(mesh, 0.5, true);

    uint idx = 0;
    for (auto v : mesh.vertices()) {
        REQUIRE(v.index() == origIndices[idx]);
        REQUIRE(v.position() == origPositions[idx]);
        REQUIRE(v.color() == origColors[idx]);
        ++idx;
    }
    idx = 0;
    for (auto f : mesh.faces()) {
        REQUIRE(f.vertexIndex(0) == origFaces[idx][0]);
        REQUIRE(f.vertexIndex(1) == origFaces[idx][1]);
        REQUIRE(f.vertexIndex(2) == origFaces[idx][2]);
        ++idx;
    }
}

TEST_CASE("Permute - permuteInPlace 32 and 64 bit")
{
    {
        std::vector<std::string> vec        = {"zero", "one", "two", "three"};
        std::vector<uint>        newIndices = {2, 0, 3, 1};
        vcl::permuteInPlace(vec, newIndices);
        REQUIRE(
            (vec == std::vector<std::string> {"one", "three", "zero", "two"}));
    }
    {
        std::vector<std::string> vec        = {"zero", "one", "two", "three"};
        std::vector<uint64_t>    newIndices = {2, 0, 3, 1};
        vcl::permuteInPlace(vec, newIndices);
        REQUIRE(
            (vec == std::vector<std::string> {"one", "three", "zero", "two"}));
    }
}

TEST_CASE("Curvature - Taubin95 on Sphere")
{
    vcl::Sphere<double> s(vcl::Point3d(0, 0, 0), 1.0);
    vcl::TriMesh        mesh = vcl::createSphereIcosahedron<vcl::TriMesh>(s, 2);
    mesh.enableVertexPrincipalCurvature();
    mesh.enableVertexAdjacentFaces();
    mesh.enableFaceAdjacentFaces();
    vcl::updateFaceAdjacentFaces(mesh);
    vcl::updateVertexAdjacentFaces(mesh);

    vcl::updatePrincipalCurvatureTaubin95(mesh);

    for (auto v : mesh.vertices()) {
        // For a unit sphere, both principal curvatures should be
        // approximately 1.0
        REQUIRE(std::abs(v.principalCurvature().maxValue() - 1.0) < 0.2);
        REQUIRE(std::abs(v.principalCurvature().minValue() - 1.0) < 0.2);
    }
}
