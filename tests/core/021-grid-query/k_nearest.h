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

#ifndef K_NEAREST_H
#define K_NEAREST_H

#include "common.h"

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/miscellaneous.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<vcl::FaceMeshConcept MeshType, typename PointType>
auto bruteForceKNearestFaces(
    const MeshType&               mesh,
    const std::vector<PointType>& points,
    vcl::uint                     k)
{
    using ScalarType = PointType::ScalarType;
    using FaceType   = MeshType::FaceType;

    auto distFun = vcl::distFunction<PointType, FaceType>();

    std::vector<std::vector<std::pair<uint, ScalarType>>> nearestAndDists(
        points.size());

    vcl::Timer t("Computing brute force kn faces for " + meshName<MeshType>());

    for (vcl::uint i = 0; const auto& p : points) {
        std::vector<std::pair<uint, ScalarType>> facesAndDists(
            mesh.faceNumber());

        vcl::uint j = 0;
        for (const auto& f : mesh.faces()) {
            facesAndDists[j] = {f.index(), distFun(p, f)};
            ++j;
        }

        std::sort(
            facesAndDists.begin(),
            facesAndDists.end(),
            [](const auto& a, const auto& b) {
                return a.second < b.second;
            });

        nearestAndDists[i] = std::vector<std::pair<uint, ScalarType>>(
            facesAndDists.begin(), facesAndDists.begin() + k);

        ++i;
    }
    t.stopAndPrint();

    return nearestAndDists;
}

template<typename Grid, vcl::FaceMeshConcept MeshType, typename PointType>
auto gridKNearestFaces(
    const Grid&                   grid,
    const MeshType&               mesh,
    const std::vector<PointType>& points,
    uint                          k,
    const std::string&            gridName)
{
    using FaceType   = MeshType::FaceType;
    using ScalarType = PointType::ScalarType;

    auto distFun = vcl::distFunction<PointType, FaceType>();

    vcl::Timer t(
        "Computing k nearests - " + meshName<MeshType>() + " - " + gridName);
    t.start();
    std::vector<std::vector<std::pair<uint, ScalarType>>> nearestGrid(
        points.size());
    for (vcl::uint i = 0; i < points.size(); i++) {
        auto vec = grid.kClosestValues(points[i], k);
        nearestGrid[i].resize(vec.size());
        for (vcl::uint j = 0; j < vec.size(); j++) {
            ScalarType dist   = distFun(points[i], *vec[j]->second);
            nearestGrid[i][j] = {vec[j]->second->index(), dist};
        }
    }
    t.stopAndPrint();

    return nearestGrid;
}

template<template<typename, typename> typename Grid, typename MeshType>
void kNearestFacesTest(
    const MeshType&    mesh,
    const auto&        points,
    const uint         k,
    const std::string& gridName)
{
    auto nearestAndDists = bruteForceKNearestFaces(mesh, points, k);

    vcl::Timer t(meshName<MeshType>() + ": Computing " + gridName);
    auto       grid = computeGrid<Grid>(mesh);
    t.stopAndPrint();

    auto nearestGrid = gridKNearestFaces(grid, mesh, points, k, gridName);

    for (vcl::uint i = 0; i < points.size(); i++) {
        for (vcl::uint j = 0; j < k; j++) {
            if (nearestAndDists[i][j].second != nearestGrid[i][j].second) {
                std::cerr << "Error point " << i << std::endl;
                std::cerr << j << " closest\n";
                std::cerr << "Dist: " << nearestAndDists[i][j].second
                          << " DistGrid: " << nearestGrid[i][j].second
                          << std::endl;
            }
            REQUIRE(nearestAndDists[i][j].second == nearestGrid[i][j].second);
        }
    }
}

#endif // K_NEAREST_H
