// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "helpers.h"

#include <vclib/algorithms/mesh/import_export/export_matrix.h>
#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/io/mesh/load_mesh.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

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

TEMPLATE_TEST_CASE(
    "Export Mesh Adjacencies to Matrix",
    "",
    Meshes,
    Meshesf,
    Meshes64,
    Meshes64f)
{
    using TriMesh = std::tuple_element_t<0, TestType>;

    TriMesh tm =
        vcl::loadMesh<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

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
