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

template<vcl::uint ELEM_ID>
void randomSelection(auto& tm)
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (auto& el : tm.template elements<ELEM_ID>()) {
        bool sel      = dis(gen);
        el.selected() = sel;
    }
}

template<vcl::uint ELEM_ID>
void randomColor(auto& tm)
{
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    tm.template enablePerElementComponent<ELEM_ID, vcl::CompId::COLOR>();

    for (auto& el : tm.template elements<ELEM_ID>()) {
        el.color() = vcl::Color(dis(gen), dis(gen), dis(gen), dis(gen));
    }
}

template<vcl::uint ELEM_ID>
void randomQuality(auto& tm)
{
    std::random_device               rd;
    std::mt19937                     gen(rd());
    std::uniform_real_distribution<> dis(-100, 100);

    tm.template enablePerElementComponent<ELEM_ID, vcl::CompId::QUALITY>();

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
    vcl::uint  nRows = tm.vertexNumber() + vtd.size();
    MatrixType verts(nRows, 3);

    auto stg = vcl::matrixStorageType<MatrixType>();

    vcl::vertexPositionsToBuffer(tm, verts.data(), stg, nRows);
    vcl::appendDuplicateVertexPositionsToBuffer(tm, vtd, verts.data(), stg);

    vcl::uint i = 0;
    // first vertices corresponds
    for (const auto& c : tm.vertices() | vcl::views::positions) {
        REQUIRE(verts(i, 0) == c.x());
        REQUIRE(verts(i, 1) == c.y());
        REQUIRE(verts(i, 2) == c.z());
        ++i;
    }

    // following vertices are duplicates (same as the vtd indices)
    for (vcl::uint vd : vtd) {
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
    using ScalarType = MeshType::VertexType::PositionType::ScalarType;

    vcl::uint               nVerts = tm.vertexNumber() + vtd.size();
    vcl::Array2<ScalarType> verts(nVerts, 3);
    vcl::vertexPositionsToBuffer(tm, verts.data());
    vcl::appendDuplicateVertexPositionsToBuffer(tm, vtd, verts.data());

    auto stg = vcl::matrixStorageType<MatrixType>();

    auto tris = vcl::faceIndicesMatrix<MatrixType>(tm);
    vcl::replaceFaceIndicesByVertexDuplicationToBuffer(
        tm, vtd, ftr, tris.data(), 3, stg);

    REQUIRE(tris.rows() == tm.faceNumber());
    REQUIRE(tris.cols() == 3);

    for (vcl::uint i = 0; const auto& f : tm.faces()) {
        for (vcl::uint j = 0; j < 3; ++j) {
            // original positions should correspond to the duplicated ones
            // that are stored in verts matrix and indexed by tris matrix
            REQUIRE(f.vertex(j)->position().x() == verts(tris(i, j), 0));
            REQUIRE(f.vertex(j)->position().y() == verts(tris(i, j), 1));
            REQUIRE(f.vertex(j)->position().z() == verts(tris(i, j), 2));
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
    vcl::uint  nRows = tm.vertexNumber() + vtd.size();
    VectorType sel(nRows);

    vcl::vertexSelectionToBuffer(tm, sel.data());
    vcl::appendDuplicateVertexSelectionToBuffer(tm, vtd, sel.data());

    vcl::uint i = 0;
    // first vertices corresponds
    for (const auto& v : tm.vertices()) {
        REQUIRE((bool) sel[i] == v.selected());
        ++i;
    }

    // following vertices are duplicates (same as the vtd indices)
    for (vcl::uint vd : vtd) {
        REQUIRE((bool) sel[i] == tm.vertex(vd).selected());
        ++i;
    }
}

using Meshes  = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using MeshesIndexed =
    std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed, vcl::EdgeMeshIndexed>;
using MeshesIndexedf = std::
    tuple<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf, vcl::EdgeMeshIndexedf>;

TEMPLATE_TEST_CASE(
    "Wedge driven duplication...",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_textured.ply");

    std::vector<std::pair<vcl::uint, vcl::uint>>          vertWedgeMap;
    std::list<vcl::uint>                                  vertsToDuplicate;
    std::list<std::list<std::pair<vcl::uint, vcl::uint>>> facesToReassign;
    vcl::uint nV = vcl::countVerticesToDuplicateByWedgeTexCoords(
        tm, vertWedgeMap, vertsToDuplicate, facesToReassign);

    SECTION("Append duplicated vertex positions...")
    {
        using ScalarType =
            typename TriMesh::VertexType::PositionType::ScalarType;

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
            testTriangleMatrix<EigenRowMatrix<vcl::uint>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen 3 Row Major")
        {
            testTriangleMatrix<Eigen3RowMatrix<vcl::uint>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen Col Major")
        {
            testTriangleMatrix<EigenColMatrix<vcl::uint>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen 3 Col Major")
        {
            testTriangleMatrix<Eigen3ColMatrix<vcl::uint>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("vcl::Array2")
        {
            testTriangleMatrix<vcl::Array2<vcl::uint>>(
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
