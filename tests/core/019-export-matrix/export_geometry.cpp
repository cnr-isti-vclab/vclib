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

TEMPLATE_TEST_CASE(
    "Export Mesh Geometry to Matrix",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

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
}
