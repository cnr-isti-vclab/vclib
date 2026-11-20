// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "helpers.h"

#include <vclib/algorithms/mesh/import_export/export_matrix.h>
#include <vclib/io/mesh/load_mesh.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<typename MatrixType>
void testVertColorsMatrix(const auto& tm)
{
    auto vertColors = vcl::vertexColorsMatrix<MatrixType>(tm);

    REQUIRE(vertColors.rows() == tm.vertexCount());
    REQUIRE(vertColors.cols() == 4);

    for (vcl::uint i = 0; const auto& c : tm.vertices() | vcl::views::colors) {
        REQUIRE(vertColors(i, 0) == c.red());
        REQUIRE(vertColors(i, 1) == c.green());
        REQUIRE(vertColors(i, 2) == c.blue());
        REQUIRE(vertColors(i, 3) == c.alpha());
        ++i;
    }
}

template<typename VectorType>
void testVertColorsVector(const auto& tm)
{
    auto vertColors =
        vcl::vertexColorsVector<VectorType>(tm, vcl::Color::Format::RGBA);

    REQUIRE(vertColors.size() == tm.vertexCount());

    for (vcl::uint i = 0; const auto& c : tm.vertices() | vcl::views::colors) {
        REQUIRE(vertColors[i] == c.rgba());
        ++i;
    }
}

template<typename MatrixType>
void testFaceColorsMatrix(const auto& tm)
{
    auto faceColors = vcl::faceColorsMatrix<MatrixType>(tm);

    REQUIRE(faceColors.rows() == tm.faceCount());
    REQUIRE(faceColors.cols() == 4);

    for (vcl::uint i = 0; const auto& c : tm.faces() | vcl::views::colors) {
        REQUIRE(faceColors(i, 0) == c.red());
        REQUIRE(faceColors(i, 1) == c.green());
        REQUIRE(faceColors(i, 2) == c.blue());
        REQUIRE(faceColors(i, 3) == c.alpha());
        ++i;
    }
}

template<typename VectorType>
void testFaceColorsVector(const auto& tm)
{
    auto faceColors =
        vcl::faceColorsVector<VectorType>(tm, vcl::Color::Format::RGBA);

    REQUIRE(faceColors.size() == tm.faceCount());

    for (vcl::uint i = 0; const auto& c : tm.faces() | vcl::views::colors) {
        REQUIRE(faceColors[i] == c.rgba());
        ++i;
    }
}

template<typename VectorType>
void testVertexQualityVector(const auto& tm)
{
    auto qual = vcl::vertexQualityVector<VectorType>(tm);

    REQUIRE(qual.size() == tm.vertexCount());

    for (vcl::uint i = 0; const auto& v : tm.vertices()) {
        REQUIRE(qual[i] == v.quality());
        ++i;
    }
}

template<typename VectorType>
void testFaceQualityVector(const auto& tm)
{
    auto qual = vcl::faceQualityVector<VectorType>(tm);

    REQUIRE(qual.size() == tm.faceCount());

    for (vcl::uint i = 0; const auto& f : tm.faces()) {
        REQUIRE(qual[i] == f.quality());
        ++i;
    }
}

TEMPLATE_TEST_CASE(
    "Export Mesh Colors and Quality to Matrix",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh = std::tuple_element_t<0, TestType>;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    SECTION("Vertex Colors...")
    {
        randomColor<vcl::ElemId::VERTEX>(tm);

        SECTION("Eigen Row Major")
        {
            testVertColorsMatrix<EigenRowMatrix<uint8_t>>(tm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testVertColorsMatrix<Eigen4RowMatrix<uint8_t>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testVertColorsMatrix<EigenColMatrix<uint8_t>>(tm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testVertColorsMatrix<Eigen4ColMatrix<uint8_t>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testVertColorsMatrix<vcl::Array2<uint8_t>>(tm);
        }
        SECTION("Eigen::Vector<vcl::uint>")
        {
            testVertColorsVector<Eigen::VectorX<vcl::uint>>(tm);
        }
        SECTION("std::vector<vcl::uint>")
        {
            testVertColorsVector<std::vector<vcl::uint>>(tm);
        }
        SECTION("vcl::Vector<vcl::uint>")
        {
            testVertColorsVector<vcl::Vector<vcl::uint, -1>>(tm);
        }
    }

    SECTION("Face Colors...")
    {
        randomColor<vcl::ElemId::FACE>(tm);

        SECTION("Eigen Row Major")
        {
            testFaceColorsMatrix<EigenRowMatrix<uint8_t>>(tm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testFaceColorsMatrix<Eigen4RowMatrix<uint8_t>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testFaceColorsMatrix<EigenColMatrix<uint8_t>>(tm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testFaceColorsMatrix<Eigen4ColMatrix<uint8_t>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testFaceColorsMatrix<vcl::Array2<uint8_t>>(tm);
        }
        SECTION("Eigen::Vector<vcl::uint>")
        {
            testFaceColorsVector<Eigen::VectorX<vcl::uint>>(tm);
        }
        SECTION("std::vector<vcl::uint>")
        {
            testFaceColorsVector<std::vector<vcl::uint>>(tm);
        }
        SECTION("vcl::Vector<vcl::uint>")
        {
            testFaceColorsVector<vcl::Vector<vcl::uint, -1>>(tm);
        }
    }

    SECTION("Vertex Quality...")
    {
        using ScalarType = vcl::VertexType<TriMesh>::QualityType;

        randomQuality<vcl::ElemId::VERTEX>(tm);

        SECTION("Eigen::Vector")
        {
            testVertexQualityVector<Eigen::VectorX<ScalarType>>(tm);
        }
        SECTION("std::vector")
        {
            testVertexQualityVector<std::vector<ScalarType>>(tm);
        }
        SECTION("vcl::Vector")
        {
            testVertexQualityVector<vcl::Vector<ScalarType, -1>>(tm);
        }
    }

    SECTION("Face Quality...")
    {
        using ScalarType = vcl::FaceType<TriMesh>::QualityType;

        randomQuality<vcl::ElemId::FACE>(tm);

        SECTION("Eigen::Vector")
        {
            testFaceQualityVector<Eigen::VectorX<ScalarType>>(tm);
        }
        SECTION("std::vector")
        {
            testFaceQualityVector<std::vector<ScalarType>>(tm);
        }
        SECTION("vcl::Vector")
        {
            testFaceQualityVector<vcl::Vector<ScalarType, -1>>(tm);
        }
    }
}
