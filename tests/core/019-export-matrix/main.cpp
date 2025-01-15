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

TEMPLATE_TEST_CASE(
    "Export TriMesh to Eigen Matrix",
    "",
    vcl::TriMesh,
    vcl::TriMeshf,
    vcl::TriMeshIndexed,
    vcl::TriMeshIndexedf)
{
    using TriMesh = TestType;

    TriMesh tm =
        vcl::loadPly<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    THEN("Export coordinates in column major matrix works as expected")
    {
        using ScalarType = typename TriMesh::VertexType::CoordType::ScalarType;
        using MatrixType = Eigen::
            Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;

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

    THEN("Export coordinates in row major matrix works as expected")
    {
        using ScalarType = typename TriMesh::VertexType::CoordType::ScalarType;
        using MatrixType = Eigen::
            Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

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

    THEN("Export coordinates in vcl::Array2 works as expected")
    {
        using ScalarType = typename TriMesh::VertexType::CoordType::ScalarType;
        using MatrixType = vcl::Array2<ScalarType>;

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

}
