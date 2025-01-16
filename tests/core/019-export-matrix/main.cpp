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
        bool sel      = dis(gen);
        el.color() = vcl::Color(dis(gen), dis(gen), dis(gen), dis(gen));
    }
}

// test functions

template<typename MatrixType>
void testCoordsMatrix(const auto& tm)
{
    auto verts = vcl::vertexMatrix<MatrixType>(tm);

    REQUIRE(verts.rows() == tm.vertexNumber());
    REQUIRE(verts.cols() == 3);

    vcl::uint i = 0;
    for (const auto& c : tm.vertices() | vcl::views::coords) {
        REQUIRE(verts(i, 0) == c.x());
        REQUIRE(verts(i, 1) == c.y());
        REQUIRE(verts(i, 2) == c.z());
        ++i;
    }
}

template<typename MatrixType>
void testTrianglesMatrix(const auto& tm)
{
    auto tris = vcl::faceMatrix<MatrixType>(tm);

    REQUIRE(tris.rows() == tm.faceNumber());
    REQUIRE(tris.cols() == 3);

    vcl::uint i = 0;
    for (const auto& f : tm.faces()) {
        for (vcl::uint j = 0; j < 3; ++j) {
            REQUIRE(tris(i, j) == f.vertexIndex(j));
        }
        ++i;
    }
}

template<typename VectorType>
void testVertexSelection(const auto& tm)
{
    auto sel = vcl::vertexSelectionVector<VectorType>(tm);

    REQUIRE(sel.size() == tm.vertexNumber());

    vcl::uint i = 0;
    for (const auto& v : tm.vertices()) {
        REQUIRE((bool) sel[i] == v.selected());
        ++i;
    }
}

template<typename VectorType>
void testFaceSelection(const auto& tm)
{
    auto sel = vcl::faceSelectionVector<VectorType>(tm);

    REQUIRE(sel.size() == tm.faceNumber());

    vcl::uint i = 0;
    for (const auto& f : tm.faces()) {
        REQUIRE((bool) sel[i] == f.selected());
        ++i;
    }
}

template<typename MatrixType>
void testVertNormalsMatrix(const auto& tm)
{
    auto vertNormals = vcl::vertexNormalsMatrix<MatrixType>(tm);

    REQUIRE(vertNormals.rows() == tm.vertexNumber());
    REQUIRE(vertNormals.cols() == 3);

    vcl::uint i = 0;
    for (const auto& n : tm.vertices() | vcl::views::normals) {
        REQUIRE(vertNormals(i, 0) == n.x());
        REQUIRE(vertNormals(i, 1) == n.y());
        REQUIRE(vertNormals(i, 2) == n.z());
        ++i;
    }
}

template<typename MatrixType>
void testFaceNormalsMatrix(const auto& tm)
{
    auto faceNormals = vcl::faceNormalsMatrix<MatrixType>(tm);

    REQUIRE(faceNormals.rows() == tm.faceNumber());
    REQUIRE(faceNormals.cols() == 3);

    vcl::uint i = 0;
    for (const auto& n : tm.faces() | vcl::views::normals) {
        REQUIRE(faceNormals(i, 0) == n.x());
        REQUIRE(faceNormals(i, 1) == n.y());
        REQUIRE(faceNormals(i, 2) == n.z());
        ++i;
    }
}

template<typename MatrixType>
void testVertColorsMatrix(const auto& tm)
{
    auto vertColors = vcl::vertexColorsMatrix<MatrixType>(tm);

    REQUIRE(vertColors.rows() == tm.vertexNumber());
    REQUIRE(vertColors.cols() == 4);

    vcl::uint i = 0;
    for (const auto& c : tm.vertices() | vcl::views::colors) {
        REQUIRE(vertColors(i, 0) == c.red());
        REQUIRE(vertColors(i, 1) == c.green());
        REQUIRE(vertColors(i, 2) == c.blue());
        REQUIRE(vertColors(i, 3) == c.alpha());
        ++i;
    }
}

TEMPLATE_TEST_CASE(
    "Export TriMesh to Matrix",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh tm =
        vcl::loadPly<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    SECTION("Coordinates...")
    {
        using ScalarType = typename TriMesh::VertexType::CoordType::ScalarType;

        SECTION("Eigen Row Major")
        {
            testCoordsMatrix<EigenRowMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testCoordsMatrix<Eigen3RowMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testCoordsMatrix<EigenColMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testCoordsMatrix<Eigen3ColMatrix<ScalarType>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testCoordsMatrix<vcl::Array2<ScalarType>>(tm);
        }
    }

    SECTION("Triangles...")
    {
        SECTION("Eigen Row Major")
        {
            testTrianglesMatrix<EigenRowMatrix<vcl::uint>>(tm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testTrianglesMatrix<Eigen3RowMatrix<vcl::uint>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testTrianglesMatrix<EigenColMatrix<vcl::uint>>(tm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testTrianglesMatrix<Eigen3ColMatrix<vcl::uint>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testTrianglesMatrix<vcl::Array2<vcl::uint>>(tm);
        }
    }

    SECTION("Vertex selection...")
    {
        randomSelection<vcl::ElemId::VERTEX>(tm);

        SECTION("Eigen Vector<vcl::uint>")
        {
            testVertexSelection<Eigen::VectorX<vcl::uint>>(tm);
        }

        SECTION("Eigen Vector<bool>")
        {
            testVertexSelection<Eigen::VectorX<bool>>(tm);
        }

        SECTION("std vector<vcl::uint>")
        {
            testVertexSelection<std::vector<vcl::uint>>(tm);
        }

        SECTION("std vector<char>")
        {
            testVertexSelection<std::vector<char>>(tm);
        }

        SECTION("vcl::Vector<vcl::uint>")
        {
            testVertexSelection<vcl::Vector<vcl::uint, -1>>(tm);
        }

        SECTION("vcl::Vector<char>")
        {
            testVertexSelection<vcl::Vector<char, -1>>(tm);
        }
    }

    SECTION("Face selection...")
    {
        randomSelection<vcl::ElemId::FACE>(tm);

        SECTION("Eigen Vector<vcl::uint>")
        {
            testFaceSelection<Eigen::VectorX<vcl::uint>>(tm);
        }

        SECTION("Eigen Vector<bool>")
        {
            testFaceSelection<Eigen::VectorX<bool>>(tm);
        }

        SECTION("std vector<vcl::uint>")
        {
            testFaceSelection<std::vector<vcl::uint>>(tm);
        }

        SECTION("std vector<char>")
        {
            testFaceSelection<std::vector<char>>(tm);
        }

        SECTION("vcl::Vector<vcl::uint>")
        {
            testFaceSelection<vcl::Vector<vcl::uint, -1>>(tm);
        }

        SECTION("vcl::Vector<char>")
        {
            testFaceSelection<vcl::Vector<char, -1>>(tm);
        }
    }

    SECTION("Vertex Normals...")
    {
        using ScalarType = typename TriMesh::VertexType::NormalType::ScalarType;

        vcl::updatePerVertexNormals(tm);

        SECTION("Eigen Row Major")
        {
            testVertNormalsMatrix<EigenRowMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testVertNormalsMatrix<Eigen3RowMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testVertNormalsMatrix<EigenColMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testVertNormalsMatrix<Eigen3ColMatrix<ScalarType>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testVertNormalsMatrix<vcl::Array2<ScalarType>>(tm);
        }
    }

    SECTION("Face Normals...")
    {
        using ScalarType = typename TriMesh::FaceType::NormalType::ScalarType;

        vcl::updatePerFaceNormals(tm);

        SECTION("Eigen Row Major")
        {
            testFaceNormalsMatrix<EigenRowMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testFaceNormalsMatrix<Eigen3RowMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testFaceNormalsMatrix<EigenColMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testFaceNormalsMatrix<Eigen3ColMatrix<ScalarType>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testFaceNormalsMatrix<vcl::Array2<ScalarType>>(tm);
        }
    }

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
    }
}
