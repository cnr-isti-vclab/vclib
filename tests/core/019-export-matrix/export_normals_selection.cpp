// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "helpers.h"

#include <vclib/algorithms/mesh/import_export/export_matrix.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/io/mesh/load_mesh.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<typename VectorType>
void testVertexSelectionVector(const auto& tm)
{
    auto sel = vcl::vertexSelectionVector<VectorType>(tm);

    REQUIRE(sel.size() == tm.vertexCount());

    for (vcl::uint i = 0; const auto& v : tm.vertices()) {
        REQUIRE((bool) sel[i] == v.selected());
        ++i;
    }
}

template<typename VectorType>
void testFaceSelectionVector(const auto& tm)
{
    auto sel = vcl::faceSelectionVector<VectorType>(tm);

    REQUIRE(sel.size() == tm.faceCount());

    for (vcl::uint i = 0; const auto& f : tm.faces()) {
        REQUIRE((bool) sel[i] == f.selected());
        ++i;
    }
}

template<typename MatrixType>
void testVertNormalsMatrix(const auto& tm)
{
    auto vertNormals = vcl::vertexNormalsMatrix<MatrixType>(tm);

    REQUIRE(vertNormals.rows() == tm.vertexCount());
    REQUIRE(vertNormals.cols() == 3);

    for (vcl::uint i = 0; const auto& n : tm.vertices() | vcl::views::normals) {
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

    REQUIRE(faceNormals.rows() == tm.faceCount());
    REQUIRE(faceNormals.cols() == 3);

    for (vcl::uint i = 0; const auto& n : tm.faces() | vcl::views::normals) {
        REQUIRE(faceNormals(i, 0) == n.x());
        REQUIRE(faceNormals(i, 1) == n.y());
        REQUIRE(faceNormals(i, 2) == n.z());
        ++i;
    }
}

TEMPLATE_TEST_CASE(
    "Export Mesh Normals and Selection to Matrix",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh = std::tuple_element_t<0, TestType>;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    SECTION("Vertex selection...")
    {
        randomSelection<vcl::ElemId::VERTEX>(tm);

        SECTION("Eigen Vector<vcl::uint>")
        {
            testVertexSelectionVector<Eigen::VectorX<vcl::uint>>(tm);
        }

        SECTION("Eigen Vector<bool>")
        {
            testVertexSelectionVector<Eigen::VectorX<bool>>(tm);
        }

        SECTION("std vector<vcl::uint>")
        {
            testVertexSelectionVector<std::vector<vcl::uint>>(tm);
        }

        SECTION("std vector<char>")
        {
            testVertexSelectionVector<std::vector<char>>(tm);
        }

        SECTION("vcl::Vector<vcl::uint>")
        {
            testVertexSelectionVector<vcl::Vector<vcl::uint, -1>>(tm);
        }

        SECTION("vcl::Vector<char>")
        {
            testVertexSelectionVector<vcl::Vector<char, -1>>(tm);
        }
    }

    SECTION("Face selection...")
    {
        randomSelection<vcl::ElemId::FACE>(tm);

        SECTION("Eigen Vector<vcl::uint>")
        {
            testFaceSelectionVector<Eigen::VectorX<vcl::uint>>(tm);
        }

        SECTION("Eigen Vector<bool>")
        {
            testFaceSelectionVector<Eigen::VectorX<bool>>(tm);
        }

        SECTION("std vector<vcl::uint>")
        {
            testFaceSelectionVector<std::vector<vcl::uint>>(tm);
        }

        SECTION("std vector<char>")
        {
            testFaceSelectionVector<std::vector<char>>(tm);
        }

        SECTION("vcl::Vector<vcl::uint>")
        {
            testFaceSelectionVector<vcl::Vector<vcl::uint, -1>>(tm);
        }

        SECTION("vcl::Vector<char>")
        {
            testFaceSelectionVector<vcl::Vector<char, -1>>(tm);
        }
    }

    SECTION("Vertex Normals...")
    {
        using ScalarType = vcl::VertexType<TriMesh>::NormalType::ScalarType;

        vcl::updateVertexNormals(tm);

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
        using ScalarType = vcl::FaceType<TriMesh>::NormalType::ScalarType;

        vcl::updateFaceNormals(tm);

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
}
