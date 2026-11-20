// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/space/core.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

template<typename ScalarType>
using EigenRowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

template<typename ScalarType>
using Eigen3RowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 3, Eigen::RowMajor>;

template<typename ScalarType>
using Eigen4RowMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 4, Eigen::RowMajor>;

template<typename ScalarType>
using EigenColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;

template<typename ScalarType>
using Eigen3ColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 3, Eigen::ColMajor>;

template<typename ScalarType>
using Eigen4ColMatrix =
    Eigen::Matrix<ScalarType, Eigen::Dynamic, 4, Eigen::ColMajor>;

// utility functions

template<vcl::ElemId ELEM_ID>
void randomSelection(auto& tm)
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (auto el : tm.template elements<ELEM_ID>()) {
        bool sel      = dis(gen);
        el.selected() = sel;
    }
}

template<vcl::ElemId ELEM_ID>
void randomColor(auto& tm)
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    tm.template enableElementComponent<ELEM_ID, vcl::CompId::COLOR>();

    for (auto& el : tm.template elements<ELEM_ID>()) {
        el.color() = vcl::Color(dis(gen), dis(gen), dis(gen), dis(gen));
    }
}

template<vcl::ElemId ELEM_ID>
void randomQuality(auto& tm)
{
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis(-100, 100);

    tm.template enableElementComponent<ELEM_ID, vcl::CompId::QUALITY>();

    for (auto& el : tm.template elements<ELEM_ID>()) {
        double qual  = dis(gen);
        el.quality() = qual;
    }
}

// test functions

template<typename MatrixType>
void testPositionsMatrix(
    const auto& tm,
    const auto& vwm,
    const auto& vtd,
    const auto& ftr)
{
    using MeshType  = vcl::RemoveRef<decltype(tm)>;
    using IndexType = typename MeshType::IndexType;

    IndexType  nRows = tm.vertexCount() + vtd.size();
    MatrixType verts(nRows, 3);

    auto stg = vcl::matrixStorageType<MatrixType>();

    vcl::vertexPositionsToBuffer(tm, verts.data(), stg, nRows);
    vcl::appendDuplicateVertexPositionsToBuffer(tm, vtd, verts.data(), stg);

    IndexType i = 0;
    // first vertices corresponds
    for (const auto& c : tm.vertices() | vcl::views::positions) {
        REQUIRE(verts(i, 0) == c.x());
        REQUIRE(verts(i, 1) == c.y());
        REQUIRE(verts(i, 2) == c.z());
        ++i;
    }

    // following vertices are duplicates (same as the vtd indices)
    for (auto vd : vtd) {
        const auto& c = tm.vertex(vd).position();
        REQUIRE(verts(i, 0) == c.x());
        REQUIRE(verts(i, 1) == c.y());
        REQUIRE(verts(i, 2) == c.z());
        ++i;
    }
}

template<typename MatrixType>
void testTriangleMatrix(
    const auto& tm,
    const auto& vwm,
    const auto& vtd,
    const auto& ftr)
{
    using MeshType   = vcl::RemoveRef<decltype(tm)>;
    using ScalarType = vcl::ScalarType<MeshType>;
    using IndexType  = typename MeshType::IndexType;

    IndexType               nVerts = tm.vertexCount() + vtd.size();
    vcl::Array2<ScalarType> verts(nVerts, 3);
    vcl::vertexPositionsToBuffer(tm, verts.data());
    vcl::appendDuplicateVertexPositionsToBuffer(tm, vtd, verts.data());

    auto stg = vcl::matrixStorageType<MatrixType>();

    auto tris = vcl::faceVertexIndicesMatrix<MatrixType>(tm);
    vcl::replaceFaceVertexIndicesByVertexDuplicationToBuffer(
        tm, vtd, ftr, tris.data(), 3, stg);

    REQUIRE(tris.rows() == tm.faceCount());
    REQUIRE(tris.cols() == 3);

    for (IndexType i = 0; const auto& f : tm.faces()) {
        for (vcl::uint j = 0; j < 3; ++j) {
            // original positions should correspond to the duplicated ones
            // that are stored in verts matrix and indexed by tris matrix
            REQUIRE(f.vertex(j).position().x() == verts(tris(i, j), 0));
            REQUIRE(f.vertex(j).position().y() == verts(tris(i, j), 1));
            REQUIRE(f.vertex(j).position().z() == verts(tris(i, j), 2));
        }
        ++i;
    }
}

template<typename VectorType>
void testVertexSelectionVector(
    const auto& tm,
    const auto& vwm,
    const auto& vtd,
    const auto& ftr)
{
    using MeshType  = vcl::RemoveRef<decltype(tm)>;
    using IndexType = typename MeshType::IndexType;

    IndexType  nRows = tm.vertexCount() + vtd.size();
    VectorType sel(nRows);

    vcl::vertexSelectionToBuffer(tm, sel.data());
    vcl::appendDuplicateVertexSelectionToBuffer(tm, vtd, sel.data());

    IndexType i = 0;
    // first vertices corresponds
    for (const auto& v : tm.vertices()) {
        REQUIRE((bool) sel[i] == v.selected());
        ++i;
    }

    // following vertices are duplicates (same as the vtd indices)
    for (auto vd : vtd) {
        REQUIRE((bool) sel[i] == tm.vertex(vd).selected());
        ++i;
    }
}

using Meshes   = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf  = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using Meshes64 = std::tuple<vcl::TriMesh64, vcl::PolyMesh64, vcl::EdgeMesh64>;
using Meshes64f =
    std::tuple<vcl::TriMesh64f, vcl::PolyMesh64f, vcl::EdgeMesh64f>;

TEMPLATE_TEST_CASE(
    "Wedge driven duplication...",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh = std::tuple_element_t<0, TestType>;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_textured.ply");

    using IndexType = TriMesh::IndexType;

    std::vector<std::pair<IndexType, vcl::uint>>          vertWedgeMap;
    std::list<IndexType>                                  vertsToDuplicate;
    std::list<std::list<std::pair<IndexType, vcl::uint>>> facesToReassign;
    [[maybe_unused]] auto nV = vcl::verticesToDuplicateByWedgeTexCoordsCount(
        tm, vertWedgeMap, vertsToDuplicate, facesToReassign);

    SECTION("Append duplicated vertex positions...")
    {
        using ScalarType = vcl::ScalarType<TriMesh>;

        SECTION("Eigen Row Major")
        {
            testPositionsMatrix<EigenRowMatrix<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen 3 Row Major")
        {
            testPositionsMatrix<Eigen3RowMatrix<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen Col Major")
        {
            testPositionsMatrix<EigenColMatrix<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen 3 Col Major")
        {
            testPositionsMatrix<Eigen3ColMatrix<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("vcl::Array2")
        {
            testPositionsMatrix<vcl::Array2<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
    }

    SECTION("Set vertex indices after duplication...")
    {
        SECTION("Eigen Row Major")
        {
            testTriangleMatrix<EigenRowMatrix<IndexType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen 3 Row Major")
        {
            testTriangleMatrix<Eigen3RowMatrix<IndexType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen Col Major")
        {
            testTriangleMatrix<EigenColMatrix<IndexType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen 3 Col Major")
        {
            testTriangleMatrix<Eigen3ColMatrix<IndexType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("vcl::Array2")
        {
            testTriangleMatrix<vcl::Array2<IndexType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
    }

    SECTION("Append duplicated vertex selection...")
    {
        randomSelection<vcl::ElemId::VERTEX>(tm);

        SECTION("Eigen Vector<vcl::uint>")
        {
            testVertexSelectionVector<Eigen::VectorX<vcl::uint>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }

        SECTION("Eigen Vector<bool>")
        {
            testVertexSelectionVector<Eigen::VectorX<bool>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }

        SECTION("std vector<vcl::uint>")
        {
            testVertexSelectionVector<std::vector<vcl::uint>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }

        SECTION("std vector<char>")
        {
            testVertexSelectionVector<std::vector<char>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }

        SECTION("vcl::Vector<vcl::uint>")
        {
            testVertexSelectionVector<vcl::Vector<vcl::uint, -1>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }

        SECTION("vcl::Vector<char>")
        {
            testVertexSelectionVector<vcl::Vector<char, -1>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
    }
}

TEMPLATE_TEST_CASE(
    "Wedge driven duplication on non-compact mesh...",
    "",
    vcl::TriMesh,
    vcl::TriMesh64)
{
    using TriMesh   = TestType;
    using IndexType = typename TriMesh::IndexType;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_textured.ply");

    // Add an isolated vertex (unreferenced), then delete face 0
    auto extraV =
        tm.addVertex(typename TriMesh::Vertex::PositionType(10, 10, 10));
    tm.deleteFace(0);

    REQUIRE(!tm.isCompact());

    std::vector<std::pair<IndexType, vcl::uint>>          vertWedgeMap;
    std::list<IndexType>                                  vertsToDuplicate;
    std::list<std::list<std::pair<IndexType, vcl::uint>>> facesToReassign;
    auto nV = vcl::verticesToDuplicateByWedgeTexCoordsCount(
        tm, vertWedgeMap, vertsToDuplicate, facesToReassign);

    REQUIRE(vertWedgeMap.size() == tm.vertexContainerSize());

    // 1. TexCoords buffer
    std::vector<double> texCoordBuffer((tm.vertexCount() + nV) * 2, 0.0);
    vcl::wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
        tm, vertWedgeMap, facesToReassign, texCoordBuffer.data());

    // 2. Face Material Indices as Duplicated Vertex Material Indices
    // (extraV is unreferenced, so vertWedgeMap[extraV].first == NULL_INDEX)
    std::vector<int> matBuffer(tm.vertexCount() + nV, 0);
    vcl::faceMaterialIndicesAsDuplicatedVertexMaterialIndicesToBuffer(
        tm, vertWedgeMap, facesToReassign, matBuffer.data());
    // The unreferenced vertex row should be SENTINEL (-1)
    auto      vertCompIndices = vcl::detail::vertCompactIndices(tm, true);
    IndexType extraVRow =
        vertCompIndices.empty() ? extraV : vertCompIndices[extraV];
    REQUIRE(matBuffer[extraVRow] == -1);

    // 3. Replace Triangulated Face Vertex Indices
    vcl::TriPolyIndexBiMap indexMap;
    std::vector<IndexType> triBuffer(tm.faceCount() * 3);
    vcl::triangulatedFaceVertexIndicesToBuffer(tm, triBuffer.data(), indexMap);
    vcl::replaceTriangulatedFaceVertexIndicesByVertexDuplicationToBuffer(
        tm, vertsToDuplicate, facesToReassign, indexMap, triBuffer.data());

    // 4. Replace Face Vertex Indices
    std::vector<IndexType> faceBuffer(tm.faceCount() * 3);
    vcl::faceVertexIndicesToBuffer(tm, faceBuffer.data());
    vcl::replaceFaceVertexIndicesByVertexDuplicationToBuffer(
        tm, vertsToDuplicate, facesToReassign, faceBuffer.data(), 3);
}
