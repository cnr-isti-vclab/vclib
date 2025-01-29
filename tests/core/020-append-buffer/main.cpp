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

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vclib/algorithms/mesh.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/views.h>

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
void testCoordsMatrix(
    const auto& tm,
    const auto& vwm,
    const auto& vtd,
    const auto& ftr)
{
    uint nRows = tm.vertexNumber() + vtd.size();
    MatrixType verts(nRows, 3);

    auto stg = vcl::matrixStorageType<MatrixType>();

    vcl::vertexCoordsToBuffer(tm, verts.data(), stg, nRows);
    vcl::appendDuplicateVertexCoordsToBuffer(tm, vtd, verts.data(), stg);

    vcl::uint i = 0;
    // first vertices corresponds
    for (const auto& c : tm.vertices() | vcl::views::coords) {
        REQUIRE(verts(i, 0) == c.x());
        REQUIRE(verts(i, 1) == c.y());
        REQUIRE(verts(i, 2) == c.z());
        ++i;
    }

    // following vertices are duplicates (same as the vtd indices)
    for (uint vd : vtd) {
        const auto& c = tm.vertex(vd).coord();
        REQUIRE(verts(i, 0) == c.x());
        REQUIRE(verts(i, 1) == c.y());
        REQUIRE(verts(i, 2) == c.z());
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
        vcl::loadPly<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_textured.ply");

    std::vector<std::pair<uint, uint>> vertWedgeMap;
    std::list<uint> vertsToDuplicate;
    std::list<std::list<std::pair<uint, uint>>> facesToReassign;
    uint nV = vcl::countVerticesToDuplicateByWedgeTexCoords(
        tm, vertWedgeMap, vertsToDuplicate, facesToReassign);

    SECTION("Append duplicated vertex coordinates...")
    {
        using ScalarType = typename TriMesh::VertexType::CoordType::ScalarType;

        SECTION("Eigen Row Major")
        {
            testCoordsMatrix<EigenRowMatrix<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen 3 Row Major")
        {
            testCoordsMatrix<Eigen3RowMatrix<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen Col Major")
        {
            testCoordsMatrix<EigenColMatrix<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("Eigen 3 Col Major")
        {
            testCoordsMatrix<Eigen3ColMatrix<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
        SECTION("vcl::Array2")
        {
            testCoordsMatrix<vcl::Array2<ScalarType>>(
                tm, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
    }
}
