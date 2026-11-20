// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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

template<typename ScalarType, typename...>
using EigenVector = Eigen::Vector<ScalarType, Eigen::Dynamic>;

template<typename ScalarType, typename...>
using VclVector = vcl::Vector<ScalarType, -1>;

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

    for (auto el : tm.template elements<ELEM_ID>()) {
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

    for (auto el : tm.template elements<ELEM_ID>()) {
        double qual  = dis(gen);
        el.quality() = qual;
    }
}

// test functions

template<typename MatrixType>
void testPositionsMatrix(const auto& tm)
{
    auto verts = vcl::vertexPositionsMatrix<MatrixType>(tm);

    REQUIRE(verts.rows() == tm.vertexCount());
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
    auto tris = vcl::faceVertexIndicesMatrix<MatrixType>(tm);

    REQUIRE(tris.rows() == tm.faceCount());
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

    REQUIRE(sizes.size() == pm.faceCount());

    for (vcl::uint i = 0; const auto& f : pm.faces()) {
        REQUIRE(sizes[i] == f.vertexCount());
        ++i;
    }
}

template<typename VectorType>
void testFaceVector(const auto& pm)
{
    auto faces = vcl::faceVertexIndicesVector<VectorType>(pm);

    vcl::uint nIndices = faceVertexReferencesCount(pm);
    REQUIRE(faces.size() == nIndices);

    for (vcl::uint i = 0; const auto& f : pm.faces()) {
        for (auto vi : f.vertexIndices()) {
            REQUIRE(faces[i] == vi);
            ++i;
        }
    }
}

template<typename MatrixType>
void testFaceMatrix(const auto& pm)
{
    auto faces = vcl::faceVertexIndicesMatrix<MatrixType>(pm);

    REQUIRE(faces.rows() == pm.faceCount());
    REQUIRE(faces.cols() == vcl::largestFaceSize(pm));

    for (vcl::uint i = 0; const auto& f : pm.faces()) {
        vcl::uint j = 0;
        for (j = 0; j < f.vertexCount(); ++j) {
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
    auto                   tris =
        vcl::triangulatedFaceVertexIndicesMatrix<MatrixType>(pm, indexMap);

    auto tCount = triangulatedFaceCount(pm);

    REQUIRE(tris.rows() == tCount);
    REQUIRE(tris.cols() == 3);

    for (vcl::uint i = 0; i < 3; ++i) {
        // polygon associated to ith triangle
        auto fIdx = indexMap.polygon(i);
        auto f    = pm.face(fIdx);

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

template<template<typename, typename...> typename Container, typename T>
void testVertexAdjacentVerticesVectors(const auto& tm)
{
    auto adjacencies = vcl::vertexAdjacentVerticesVectors<Container, T>(tm);

    REQUIRE(adjacencies.size() == tm.vertexCount());

    for (vcl::uint i = 0; const auto& v : tm.vertices()) {
        const auto& adjList = adjacencies[i];
        REQUIRE(adjList.size() == v.adjVertexCount());

        vcl::uint j = 0;
        for (auto adjV : v.adjVertexIndices()) {
            REQUIRE(adjList[j] == static_cast<T>(adjV));
            ++j;
        }
        ++i;
    }
}

template<typename MatrixType>
void testVertexAdjacentVerticesMatrix(const auto& tm)
{
    auto adjMatrix = vcl::vertexAdjacentVerticesMatrix<MatrixType>(tm);

    vcl::uint lva = vcl::largestVertexAdjacentVerticesCount(tm);

    REQUIRE(adjMatrix.rows() == tm.vertexCount());
    REQUIRE(adjMatrix.cols() == lva);

    for (vcl::uint i = 0; const auto& v : tm.vertices()) {
        vcl::uint j = 0;
        for (auto adjV : v.adjVertexIndices()) {
            REQUIRE(adjMatrix(i, j) == static_cast<vcl::uint>(adjV));
            ++j;
        }
        // Check that remaining entries are set to -1 (UINT_NULL)
        for (; j < lva; ++j) {
            REQUIRE(adjMatrix(i, j) == vcl::uint(-1));
        }
        ++i;
    }
}

template<template<typename, typename...> typename Container, typename T>
void testVertexAdjacentFacesVectors(const auto& tm)
{
    auto adjacencies = vcl::vertexAdjacentFacesVectors<Container, T>(tm);

    REQUIRE(adjacencies.size() == tm.vertexCount());

    for (vcl::uint i = 0; const auto& v : tm.vertices()) {
        const auto& adjList = adjacencies[i];
        REQUIRE(adjList.size() == v.adjFaceCount());

        vcl::uint j = 0;
        for (auto adjF : v.adjFaceIndices()) {
            REQUIRE(adjList[j] == static_cast<T>(adjF));
            ++j;
        }
        ++i;
    }
}

template<typename MatrixType>
void testVertexAdjacentFacesMatrix(const auto& tm)
{
    auto adjMatrix = vcl::vertexAdjacentFacesMatrix<MatrixType>(tm);

    vcl::uint lfa = vcl::largestVertexAdjacentFacesCount(tm);

    REQUIRE(adjMatrix.rows() == tm.vertexCount());
    REQUIRE(adjMatrix.cols() == lfa);

    for (vcl::uint i = 0; const auto& v : tm.vertices()) {
        vcl::uint j = 0;
        for (auto adjF : v.adjFaceIndices()) {
            REQUIRE(adjMatrix(i, j) == static_cast<vcl::uint>(adjF));
            ++j;
        }
        // Check that remaining entries are set to -1 (UINT_NULL)
        for (; j < lfa; ++j) {
            REQUIRE(adjMatrix(i, j) == vcl::uint(-1));
        }
        ++i;
    }
}

template<template<typename, typename...> typename Container, typename T>
void testFaceAdjacentFacesVectors(const auto& tm)
{
    auto adjacencies = vcl::faceAdjacentFacesVectors<Container, T>(tm);

    REQUIRE(adjacencies.size() == tm.faceCount());

    for (vcl::uint i = 0; const auto& f : tm.faces()) {
        const auto& adjList = adjacencies[i];
        REQUIRE(adjList.size() == f.adjFaceCount());

        vcl::uint j = 0;
        for (auto adjF : f.adjFaceIndices()) {
            REQUIRE(adjList[j] == static_cast<T>(adjF));
            ++j;
        }
        ++i;
    }
}

template<typename MatrixType>
void testFaceAdjacentFacesMatrix(const auto& tm)
{
    auto adjMatrix = vcl::faceAdjacentFacesMatrix<MatrixType>(tm);

    vcl::uint lfa = vcl::largestFaceSize(tm);

    REQUIRE(adjMatrix.rows() == tm.faceCount());
    REQUIRE(adjMatrix.cols() == lfa);

    for (vcl::uint i = 0; const auto& f : tm.faces()) {
        vcl::uint j = 0;
        for (auto adjF : f.adjFaceIndices()) {
            REQUIRE(adjMatrix(i, j) == static_cast<vcl::uint>(adjF));
            ++j;
        }
        // Check that remaining entries are set to -1 (UINT_NULL)
        for (; j < lfa; ++j) {
            REQUIRE(adjMatrix(i, j) == vcl::uint(-1));
        }
        ++i;
    }
}

using Meshes   = std::tuple<vcl::TriMesh, vcl::PolyMesh, vcl::EdgeMesh>;
using Meshesf  = std::tuple<vcl::TriMeshf, vcl::PolyMeshf, vcl::EdgeMeshf>;
using Meshes64 = std::tuple<vcl::TriMesh64, vcl::PolyMesh64, vcl::EdgeMesh64>;
using Meshes64f =
    std::tuple<vcl::TriMesh64f, vcl::PolyMesh64f, vcl::EdgeMesh64f>;

TEMPLATE_TEST_CASE(
    "Export Mesh to Matrix",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
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
        using ScalarType = vcl::ScalarType<TriMesh>;

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

    SECTION("Vertex Adjacent Vertices Vectors...")
    {
        // Update per-vertex adjacency information first
        tm.enableVertexAdjacentVertices();
        vcl::updateVertexAdjacentVertices(tm);

        SECTION("std::vector<std::vector<vcl::uint>>")
        {
            testVertexAdjacentVerticesVectors<std::vector, vcl::uint>(tm);
        }
        SECTION("Eigen::Vector<Eigen::Vector<vcl::uint>>")
        {
            testVertexAdjacentVerticesVectors<EigenVector, vcl::uint>(tm);
        }
        SECTION("vcl::Vector<vcl::Vector<vcl::uint>>")
        {
            testVertexAdjacentVerticesVectors<VclVector, vcl::uint>(tm);
        }
    }

    SECTION("Vertex Adjacent Vertices Matrix...")
    {
        // Update per-vertex adjacency information first
        tm.enableVertexAdjacentVertices();
        vcl::updateVertexAdjacentVertices(tm);

        SECTION("Eigen Row Major")
        {
            testVertexAdjacentVerticesMatrix<EigenRowMatrix<vcl::uint>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testVertexAdjacentVerticesMatrix<EigenColMatrix<vcl::uint>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testVertexAdjacentVerticesMatrix<vcl::Array2<vcl::uint>>(tm);
        }
    }

    SECTION("Vertex Adjacent Faces Vectors...")
    {
        // Update per-vertex adjacent faces information first
        tm.enableVertexAdjacentFaces();
        vcl::updateVertexAdjacentFaces(tm);

        SECTION("std::vector<std::vector<vcl::uint>>")
        {
            testVertexAdjacentFacesVectors<std::vector, vcl::uint>(tm);
        }
        SECTION("Eigen::Vector<Eigen::Vector<vcl::uint>>")
        {
            testVertexAdjacentFacesVectors<EigenVector, vcl::uint>(tm);
        }
        SECTION("vcl::Vector<vcl::Vector<vcl::uint>>")
        {
            testVertexAdjacentFacesVectors<VclVector, vcl::uint>(tm);
        }
    }

    SECTION("Vertex Adjacent Faces Matrix...")
    {
        // Update per-vertex adjacent faces information first
        tm.enableVertexAdjacentFaces();
        vcl::updateVertexAdjacentFaces(tm);

        SECTION("Eigen Row Major")
        {
            testVertexAdjacentFacesMatrix<EigenRowMatrix<vcl::uint>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testVertexAdjacentFacesMatrix<EigenColMatrix<vcl::uint>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testVertexAdjacentFacesMatrix<vcl::Array2<vcl::uint>>(tm);
        }
    }

    SECTION("Face Adjacent Faces Vectors...")
    {
        // Update per-face adjacent faces information first
        tm.enableFaceAdjacentFaces();
        vcl::updateFaceAdjacentFaces(tm);

        SECTION("std::vector<std::vector<vcl::uint>>")
        {
            testFaceAdjacentFacesVectors<std::vector, vcl::uint>(tm);
        }
        SECTION("Eigen::Vector<Eigen::Vector<vcl::uint>>")
        {
            testFaceAdjacentFacesVectors<EigenVector, vcl::uint>(tm);
        }
        SECTION("vcl::Vector<vcl::Vector<vcl::uint>>")
        {
            testFaceAdjacentFacesVectors<VclVector, vcl::uint>(tm);
        }
    }

    SECTION("Face Adjacent Faces Matrix...")
    {
        // Update per-face adjacent faces information first
        tm.enableFaceAdjacentFaces();
        vcl::updateFaceAdjacentFaces(tm);

        SECTION("Eigen Row Major")
        {
            testFaceAdjacentFacesMatrix<EigenRowMatrix<vcl::uint>>(tm);
        }
        SECTION("Eigen Col Major")
        {
            testFaceAdjacentFacesMatrix<EigenColMatrix<vcl::uint>>(tm);
        }
        SECTION("vcl::Array2")
        {
            testFaceAdjacentFacesMatrix<vcl::Array2<vcl::uint>>(tm);
        }
    }
}

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
