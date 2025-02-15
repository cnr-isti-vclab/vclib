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
#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/miscellaneous.h>
#include <vclib/space.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

template<vcl::FaceMeshConcept MeshType>
constexpr std::string meshName()
{
    constexpr bool indexed = MeshType::FaceType::INDEXED;
    using ScalarType = MeshType::VertexType::CoordType::ScalarType;

    std::string name;
    if constexpr (vcl::HasTriangles<MeshType>)
        name = "Tri";
    else
        name = "Poly";

    name += "Mesh";

    if constexpr (indexed)
        name += "Indexed";

    if constexpr (std::same_as<ScalarType, float>)
        name += "f";

    return name;
}

template<vcl::Box3Concept BoxType>
auto randomPoints(vcl::uint n, const BoxType& bbox)
{
    using ScalarType = BoxType::PointType::ScalarType;
    using DistrType = std::uniform_real_distribution<ScalarType>;

    std::vector<vcl::Point3<ScalarType>> points(n);

    ScalarType ext = bbox.diagonal() * 0.1;

    std::random_device rd;
    auto seed = rd();

    std::cerr << "Random seed: " << seed << std::endl;

    std::mt19937       gen(seed);
    DistrType disX(bbox.min().x() - ext, bbox.max().x() + ext);
    DistrType disY(bbox.min().y() - ext, bbox.max().y() + ext);
    DistrType disZ(bbox.min().z() - ext, bbox.max().z() + ext);

    for (vcl::uint i = 0; i < n; i++)
        points[i] = vcl::Point3<ScalarType>(disX(gen), disY(gen), disZ(gen));

    return points;
}

template<
    template<typename, typename> typename Grid,
    vcl::FaceMeshConcept MeshType>
auto computeGrid(const MeshType& mesh)
{
    using ScalarType = MeshType::VertexType::CoordType::ScalarType;
    using FaceType = MeshType::FaceType;

    return Grid<const FaceType*, ScalarType>(
        mesh.faces() | vcl::views::constAddrOf);
}

template<vcl::FaceMeshConcept MeshType, typename PointType>
auto bruteforceNearestFaces(
    const MeshType& mesh,
    const std::vector<PointType>&     points)
{
    using ScalarType = PointType::ScalarType;
    using FaceType = MeshType::FaceType;

    auto distFun = vcl::distFunction<PointType, FaceType>();

    std::vector<vcl::uint> nearest(points.size());
    std::vector<ScalarType> dists(
        points.size(), std::numeric_limits<ScalarType>::max());

    vcl::Timer t(
        "Computing brute force distances for " + meshName<MeshType>() + ")");
    for (vcl::uint i = 0; const auto& p : points) {
        for (const auto& f : mesh.faces()) {
            ScalarType dist = distFun(p, f);
            if (dist < dists[i]) {
                dists[i] = dist;
                nearest[i] = mesh.index(f);
            }
        }
        ++i;
    }
    t.stopAndPrint();

    return std::make_pair(nearest, dists);
}

template<typename Grid, typename PointType>
auto gridNearestFaces(
    const Grid& grid,
    const std::vector<PointType>& points,
    const std::string& meshName,
    const std::string& gridName)
{
    using ScalarType = PointType::ScalarType;
    vcl::Timer t(
        "Computing nearests - " + meshName + " - " + gridName);
    t.start();
    std::vector<vcl::uint> nearestGrid(points.size());
    std::vector<ScalarType> dists(
        points.size(), std::numeric_limits<ScalarType>::max());
    for (vcl::uint i = 0; i < points.size(); i++) {
        auto it = grid.closestValue(points[i], dists[i]);
        nearestGrid[i] = it->second->index();
    }
    t.stopAndPrint();
    return std::make_pair(nearestGrid, dists);
}

template<template<typename, typename> typename Grid, typename MeshType>
void closestPointTest(const MeshType& mesh, const auto& points, const std::string& gridName)
{
    auto [nearest, dists] = bruteforceNearestFaces(mesh, points);

    vcl::Timer t(meshName<MeshType>() + ": Computing " + gridName);
    auto grid = computeGrid<Grid>(mesh);
    t.stopAndPrint();

    auto [nearestGrid, distsGrid] =
        gridNearestFaces(grid, points, meshName<MeshType>(), gridName);

    for (vcl::uint i = 0; i < points.size(); i++) {
        if (dists[i] != distsGrid[i]) {
            std::cerr << "Error point " << i << std::endl;
            std::cerr << "coord: \n" << points[i] << std::endl;
            std::cerr << "cell: \n";
            std::cerr << grid.cell(points[i]) << std::endl;
            std::cerr << " dist: " << dists[i]
                      << " distGrid: " << distsGrid[i] << std::endl;
            std::cerr << "computed closest: " << nearest[i]
                      << " grid closest: " << nearestGrid[i] << std::endl;
        }
        REQUIRE(dists[i] == distsGrid[i]);
    }
}

using Meshes = std::tuple<vcl::TriMesh, vcl::PolyMesh>;
using Meshesf = std::tuple<vcl::TriMeshf, vcl::PolyMeshf>;
using MeshesIndexed =
    std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed>;
using MeshesIndexedf = std::
    tuple<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf>;

template<typename ValueType, typename ScalarType>
using HSGrid3 = vcl::HashTableGrid3<ValueType, ScalarType>;

TEMPLATE_TEST_CASE(
    "Query Grids...",
    "",
    Meshes)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    using namespace vcl;

    const uint N_POINTS_TEST = 1000;

    SECTION("TriMesh")
    {
        using PointType = TriMesh::VertexType::CoordType;

        TriMesh tm = load<TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
        vcl::updateBoundingBox(tm);

        std::vector<PointType> points =
            randomPoints(N_POINTS_TEST, tm.boundingBox());

        SECTION("HashTableGrid")
        {
            closestPointTest<HSGrid3>(tm, points, "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            closestPointTest<StaticGrid3>(tm, points, "StaticGrid");
        }
    }

    SECTION("PolyMesh")
    {
        using PointType = PolyMesh::VertexType::CoordType;

        PolyMesh pm = load<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
        vcl::updateBoundingBox(pm);

        std::vector<PointType> points =
            randomPoints(N_POINTS_TEST, pm.boundingBox());

        SECTION("HashTableGrid")
        {
            closestPointTest<HSGrid3>(pm, points, "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            closestPointTest<StaticGrid3>(pm, points, "StaticGrid");
        }
    }
}
