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
void testPositionsMatrix(const auto& tm)
{
    auto verts = vcl::vertexPositionsMatrix<MatrixType>(tm);

    REQUIRE(verts.rows() == tm.vertexNumber());
    REQUIRE(verts.cols() == 3);

    for (vcl::uint   i = 0;
         const auto& c : tm.vertices() | vcl::views::positions) {
        REQUIRE(verts(i, 0) == c.x());
        REQUIRE(verts(i, 1) == c.y());
        REQUIRE(verts(i, 2) == c.z());
        ++i;
    }
}

template<typename MatrixType>
void testTrianglesMatrix(const auto& tm)
{
    auto tris = vcl::faceIndicesMatrix<MatrixType>(tm);

    REQUIRE(tris.rows() == tm.faceNumber());
    REQUIRE(tris.cols() == 3);

    for (vcl::uint i = 0; const auto& f : tm.faces()) {
        for (vcl::uint j = 0; j < 3; ++j) {
            REQUIRE(tris(i, j) == f.vertexIndex(j));
        }
        ++i;
    }
}

template<typename VectorType>
void testFaceSizesVector(const auto& pm)
{
    auto sizes = vcl::faceSizesVector<VectorType>(pm);

    REQUIRE(sizes.size() == pm.faceNumber());

    for (vcl::uint i = 0; const auto& f : pm.faces()) {
        REQUIRE(sizes[i] == f.vertexNumber());
        ++i;
    }
}

template<typename VectorType>
void testFaceVector(const auto& pm)
{
    auto faces = vcl::faceIndicesVector<VectorType>(pm);

    vcl::uint nIndices = countPerFaceVertexReferences(pm);
    REQUIRE(faces.size() == nIndices);

    for (vcl::uint i = 0; const auto& f : pm.faces()) {
        for (const auto* v : f.vertices()) {
            REQUIRE(faces[i] == pm.index(v));
            ++i;
        }
    }
}

template<typename MatrixType>
void testFaceMatrix(const auto& pm)
{
    auto faces = vcl::faceIndicesMatrix<MatrixType>(pm);

    REQUIRE(faces.rows() == pm.faceNumber());
    REQUIRE(faces.cols() == vcl::largestFaceSize(pm));

    for (vcl::uint i = 0; const auto& f : pm.faces()) {
        vcl::uint j = 0;
        for (j = 0; j < f.vertexNumber(); ++j) {
            REQUIRE(faces(i, j) == f.vertexIndex(j));
        }
        for (; j < faces.cols(); ++j) {
            REQUIRE(faces(i, j) == -1);
        }

        ++i;
    }
}

template<typename MatrixType>
void testTriangulatedFaceMatrix(const auto& pm)
{
    vcl::TriPolyIndexBiMap indexMap;
    auto tris = vcl::triangulatedFaceIndicesMatrix<MatrixType>(pm, indexMap);

    vcl::uint tNumber = countTriangulatedTriangles(pm);

    REQUIRE(tris.rows() == tNumber);
    REQUIRE(tris.cols() == 3);

    for (vcl::uint i = 0; i < 3; ++i) {
        // polygon associated to ith triangle
        vcl::uint   fIdx = indexMap.polygon(i);
        const auto& f    = pm.face(fIdx);

        for (vcl::uint j = 0; j < 3; ++j) {
            REQUIRE(f.containsVertex(tris(i, j)));
        }
        ++i;
    }
}

template<typename VectorType>
void testVertexSelectionVector(const auto& tm)
{
    auto sel = vcl::vertexSelectionVector<VectorType>(tm);

    REQUIRE(sel.size() == tm.vertexNumber());

    for (vcl::uint i = 0; const auto& v : tm.vertices()) {
        REQUIRE((bool) sel[i] == v.selected());
        ++i;
    }
}

template<typename VectorType>
void testFaceSelectionVector(const auto& tm)
{
    auto sel = vcl::faceSelectionVector<VectorType>(tm);

    REQUIRE(sel.size() == tm.faceNumber());

    for (vcl::uint i = 0; const auto& f : tm.faces()) {
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

    REQUIRE(faceNormals.rows() == tm.faceNumber());
    REQUIRE(faceNormals.cols() == 3);

    for (vcl::uint i = 0; const auto& n : tm.faces() | vcl::views::normals) {
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

    REQUIRE(vertColors.size() == tm.vertexNumber());

    for (vcl::uint i = 0; const auto& c : tm.vertices() | vcl::views::colors) {
        REQUIRE(vertColors[i] == c.rgba());
        ++i;
    }
}

template<typename MatrixType>
void testFaceColorsMatrix(const auto& tm)
{
    auto faceColors = vcl::faceColorsMatrix<MatrixType>(tm);

    REQUIRE(faceColors.rows() == tm.faceNumber());
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

    REQUIRE(faceColors.size() == tm.faceNumber());

    for (vcl::uint i = 0; const auto& c : tm.faces() | vcl::views::colors) {
        REQUIRE(faceColors[i] == c.rgba());
        ++i;
    }
}

template<typename VectorType>
void testVertexQualityVector(const auto& tm)
{
    auto qual = vcl::vertexQualityVector<VectorType>(tm);

    REQUIRE(qual.size() == tm.vertexNumber());

    for (vcl::uint i = 0; const auto& v : tm.vertices()) {
        REQUIRE(qual[i] == v.quality());
        ++i;
    }
}

template<typename VectorType>
void testFaceQualityVector(const auto& tm)
{
    auto qual = vcl::faceQualityVector<VectorType>(tm);

    REQUIRE(qual.size() == tm.faceNumber());

    for (vcl::uint i = 0; const auto& f : tm.faces()) {
        REQUIRE(qual[i] == f.quality());
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
    "Export TriMesh to Matrix",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;
    using EdgeMesh = std::tuple_element_t<2, TestType>;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");
    PolyMesh pm = vcl::loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                          "/rhombicosidodecahedron.obj");

    SECTION("Positions...")
    {
        using ScalarType =
            typename TriMesh::VertexType::PositionType::ScalarType;

        SECTION("Eigen Row Major")
        {
            testPositionsMatrix<EigenRowMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testPositionsMatrix<Eigen3RowMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testPositionsMatrix<EigenColMatrix<ScalarType>>(tm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testPositionsMatrix<Eigen3ColMatrix<ScalarType>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testPositionsMatrix<vcl::Array2<ScalarType>>(tm);
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

    SECTION("Faces...")
    {
        SECTION("Eigen Row Major")
        {
            testFaceMatrix<EigenRowMatrix<vcl::uint>>(tm);
            testFaceMatrix<EigenRowMatrix<vcl::uint>>(pm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testFaceMatrix<Eigen3RowMatrix<vcl::uint>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testFaceMatrix<EigenColMatrix<vcl::uint>>(tm);
            testFaceMatrix<EigenColMatrix<vcl::uint>>(pm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testFaceMatrix<Eigen3ColMatrix<vcl::uint>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testFaceMatrix<vcl::Array2<vcl::uint>>(tm);
            testFaceMatrix<vcl::Array2<vcl::uint>>(pm);
        }
        SECTION("Eigen Vector<vcl::uint>")
        {
            testFaceVector<Eigen::VectorX<vcl::uint>>(pm);
        }

        SECTION("std vector<vcl::uint>")
        {
            testFaceVector<std::vector<vcl::uint>>(pm);
        }

        SECTION("vcl::Vector<vcl::uint>")
        {
            testFaceVector<vcl::Vector<vcl::uint, -1>>(pm);
        }
    }

    SECTION("Face sizes...")
    {
        SECTION("Eigen Vector<vcl::uint>")
        {
            testFaceSizesVector<Eigen::VectorX<vcl::uint>>(pm);
        }

        SECTION("std vector<vcl::uint>")
        {
            testFaceSizesVector<std::vector<vcl::uint>>(pm);
        }

        SECTION("vcl::Vector<vcl::uint>")
        {
            testFaceSizesVector<vcl::Vector<vcl::uint, -1>>(pm);
        }
    }

    SECTION("Triangulated Faces...")
    {
        SECTION("Eigen Row Major")
        {
            testTriangulatedFaceMatrix<EigenRowMatrix<vcl::uint>>(pm);
        }
        SECTION("Eigen 3 Row Major")
        {
            testTriangulatedFaceMatrix<Eigen3RowMatrix<vcl::uint>>(pm);
        }
        SECTION("Eigen Col Major")
        {
            testTriangulatedFaceMatrix<EigenColMatrix<vcl::uint>>(pm);
        }
        SECTION("Eigen 3 Col Major")
        {
            testTriangulatedFaceMatrix<Eigen3ColMatrix<vcl::uint>>(pm);
        }
        SECTION("vcl::Array2")
        {
            testTriangulatedFaceMatrix<vcl::Array2<vcl::uint>>(pm);
        }
    }

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
        using ScalarType = typename TriMesh::VertexType::QualityType;

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
        using ScalarType = typename TriMesh::FaceType::QualityType;

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
