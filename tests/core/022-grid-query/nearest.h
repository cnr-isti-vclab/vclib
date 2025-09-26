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

#ifndef NEAREST_H
#define NEAREST_H

#include "common.h"

#include <vclib/algorithms.h>
#include <vclib/io.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<vcl::FaceMeshConcept MeshType, typename PointType>
auto bruteforceNearestFaces(
    const MeshType&               mesh,
    const std::vector<PointType>& points)
{
    using ScalarType = PointType::ScalarType;
    using FaceType   = MeshType::FaceType;

    auto distFun = vcl::distFunction<PointType, FaceType>();

    std::vector<vcl::uint>  nearest(points.size());
    std::vector<ScalarType> dists(
        points.size(), std::numeric_limits<ScalarType>::max());

    vcl::Timer t(
        "Computing brute force distances for " + vcl::meshTypeName<MeshType>());
    for (vcl::uint i = 0; const auto& p : points) {
        for (const auto& f : mesh.faces()) {
            ScalarType dist = distFun(p, f);
            if (dist < dists[i]) {
                dists[i]   = dist;
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
    const Grid&                   grid,
    const std::vector<PointType>& points,
    const std::string&            meshName,
    const std::string&            gridName)
{
    using ScalarType = PointType::ScalarType;
    vcl::Timer t("Computing nearests - " + meshName + " - " + gridName);
    t.start();
    std::vector<vcl::uint>  nearestGrid(points.size());
    std::vector<ScalarType> dists(
        points.size(), std::numeric_limits<ScalarType>::max());
    for (vcl::uint i = 0; i < points.size(); i++) {
        auto it        = grid.closestValue(points[i], dists[i]);
        nearestGrid[i] = it->second->index();
    }
    t.stopAndPrint();
    return std::make_pair(nearestGrid, dists);
}

template<template<typename, typename> typename Grid, typename MeshType>
void closestFacesTest(
    const MeshType&    mesh,
    const auto&        points,
    const std::string& gridName)
{
    auto [nearest, dists] = bruteforceNearestFaces(mesh, points);

    vcl::Timer t(vcl::meshTypeName<MeshType>() + ": Computing " + gridName);
    auto       grid = computeGrid<Grid>(mesh);
    t.stopAndPrint();

    auto [nearestGrid, distsGrid] =
        gridNearestFaces(grid, points, vcl::meshTypeName<MeshType>(), gridName);

    for (vcl::uint i = 0; i < points.size(); i++) {
        if (!vcl::epsilonEquals(dists[i], distsGrid[i])) {
            std::cerr << "Error point " << i << std::endl;
            std::cerr << "pos: \n" << points[i] << std::endl;
            std::cerr << "cell: \n";
            std::cerr << grid.cell(points[i]) << std::endl;
            std::cerr << " dist: " << dists[i] << " distGrid: " << distsGrid[i]
                      << std::endl;
            std::cerr << "computed closest: " << nearest[i]
                      << " grid closest: " << nearestGrid[i] << std::endl;
        }
        REQUIRE(vcl::epsilonEquals(dists[i], distsGrid[i]));
    }
}

#endif // NEAREST_H
