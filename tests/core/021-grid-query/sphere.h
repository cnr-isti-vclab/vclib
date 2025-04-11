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

#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/miscellaneous.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<vcl::FaceMeshConcept MeshType, typename SphereType>
auto bruteforceFacesInSpheres(
    const MeshType&                mesh,
    const std::vector<SphereType>& spheres)
{
    using ScalarType = SphereType::ScalarType;
    using FaceType   = MeshType::FaceType;

    auto intersFunction = [](const SphereType& s, const FaceType& f) {
        return s.intersects(vcl::boundingBox(f));
    };

    std::vector<std::vector<vcl::uint>> facesInSpheres(spheres.size());

    vcl::Timer t(
        "Computing brute force faces in sphere for " +
        vcl::meshTypeName<MeshType>());
    for (vcl::uint i = 0; const auto& s : spheres) {
        for (const auto& f : mesh.faces()) {
            intersFunction(s, f);
            if (intersFunction(s, f)) {
                facesInSpheres[i].push_back(mesh.index(f));
            }
        }
        ++i;
    }
    t.stopAndPrint();

    return facesInSpheres;
}

template<typename Grid, typename SphereType>
auto gridFacesInSpheres(
    const Grid&                    grid,
    const std::vector<SphereType>& spheres,
    const std::string&             meshName,
    const std::string&             gridName)
{
    using ScalarType = SphereType::ScalarType;
    vcl::Timer t("Computing nearests - " + meshName + " - " + gridName);
    t.start();
    std::vector<std::vector<vcl::uint>> facesInSpheresGrid(spheres.size());
    for (vcl::uint i = 0; i < spheres.size(); i++) {
        auto vec = grid.valuesInSphere(spheres[i]);
        facesInSpheresGrid[i].resize(vec.size());
        for (vcl::uint j = 0; auto& it : vec) {
            facesInSpheresGrid[i][j] = it->second->index();
            ++j;
        }
    }
    t.stopAndPrint();
    return facesInSpheresGrid;
}

template<template<typename, typename> typename Grid, typename MeshType>
void facesInSpheresTest(
    const MeshType&    mesh,
    const auto&        spheres,
    const std::string& gridName)
{
    auto vsBF = bruteforceFacesInSpheres(mesh, spheres);

    vcl::Timer t(vcl::meshTypeName<MeshType>() + ": Computing " + gridName);
    auto       grid = computeGrid<Grid>(mesh);
    t.stopAndPrint();

    auto vsG = gridFacesInSpheres(
        grid, spheres, vcl::meshTypeName<MeshType>(), gridName);

    for (vcl::uint i = 0; i < spheres.size(); i++) {
        if (vsBF[i].size() != vsG[i].size()) {
            std::cerr << "Error in sphere " << i << std::endl;
            std::cerr << "N. faces that intersects: " << vsBF[i].size() << " - "
                      << vsG[i].size() << std::endl;
        }
        REQUIRE(vsBF[i].size() == vsG[i].size());
        for (vcl::uint j = 0; j < vsBF[i].size(); j++) {
            auto it = std::find(vsBF[i].begin(), vsBF[i].end(), vsG[i][j]);
            if (it == vsBF[i].end()) {
                std::cerr << "Error in sphere " << i << std::endl;
                std::cerr << "Face " << vsG[i][j] << " not found" << std::endl;
            }
            REQUIRE(it != vsBF[i].end());
        }
    }
}

#endif // SPHERE_H
