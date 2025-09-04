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

#ifndef MESH_CURVATURE_H
#define MESH_CURVATURE_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates mesh curvature computation algorithms:
// 1. Principal curvature using Taubin95 algorithm
// 2. Principal curvature using PCA method
// 3. Color mapping based on mean and Gaussian curvature

auto meshCurvature()
{
    std::cout << "=== VCLib Example 015: Mesh Curvature ===\n\n";

    /****** Load a mesh for curvature computation ******/

    std::cout << "=== Loading Bunny Mesh ===" << std::endl;

    vcl::TriMesh originalMesh =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    originalMesh.name() = "Original Bunny Mesh";

    std::cout << "Original mesh loaded:" << std::endl;
    std::cout << "  Vertices: " << originalMesh.vertexNumber() << std::endl;
    std::cout << "  Faces: " << originalMesh.faceNumber() << std::endl;

    // Enable required components
    originalMesh.enablePerVertexAdjacentFaces();
    originalMesh.enablePerFaceAdjacentFaces();
    originalMesh.enablePerVertexPrincipalCurvature();
    originalMesh.enablePerVertexColor();
    originalMesh.enablePerVertexQuality();

    vcl::updatePerVertexAndFaceNormals(originalMesh);
    vcl::updatePerFaceAdjacentFaces(originalMesh);
    vcl::updatePerVertexAdjacentFaces(originalMesh);

    /****** Principal Curvature with Taubin95 Algorithm ******/

    std::cout << "\n=== Principal Curvature (Taubin95) ===" << std::endl;

    vcl::TriMesh taubinMesh = originalMesh;
    taubinMesh.name()       = "Taubin95 Curvature";

    {
        vcl::Timer timer("Taubin95 principal curvature");
        vcl::updatePrincipalCurvatureTaubin95(taubinMesh);

        // you can use also the general function:
        // vcl::updatePrincipalCurvature(
        //     taubinMesh, vcl::VCL_PRINCIPAL_CURVATURE_TAUBIN95);
        timer.stopAndPrint();
    }

    // Color mapping based on mean curvature
    vcl::setPerVertexQualityFromPrincipalCurvatureMean(taubinMesh);
    vcl::Histogramd h = vcl::vertexQualityHistogram(taubinMesh);

    vcl::setPerVertexColorFromQuality(
        taubinMesh,
        vcl::Color::ColorMap::RedBlue,
        h.valueAtPercentile(0.1),
        h.valueAtPercentile(0.9));

    std::cout << "Taubin95 curvature computed." << std::endl;
    std::cout << "  Mean curvature range: " << h.minRangeValue() << " to "
              << h.maxRangeValue() << std::endl;

    /****** Principal Curvature with PCA Method ******/

    std::cout << "\n=== Principal Curvature (PCA) ===" << std::endl;

    vcl::TriMesh pcaMesh = originalMesh;
    pcaMesh.name()       = "PCA Curvature";

    double radius = vcl::boundingBox(pcaMesh).diagonal() * 0.05;

    {
        vcl::Timer timer("PCA principal curvature");
        vcl::updatePrincipalCurvaturePCA(pcaMesh, radius, true);
        timer.stopAndPrint();
    }

    // Color mapping based on Gaussian curvature
    vcl::setPerVertexQualityFromPrincipalCurvatureGaussian(pcaMesh);
    vcl::Histogramd h2 = vcl::vertexQualityHistogram(pcaMesh);

    vcl::setPerVertexColorFromQuality(
        pcaMesh,
        vcl::Color::ColorMap::RedBlue,
        h2.valueAtPercentile(0.05),
        h2.valueAtPercentile(0.95));

    std::cout << "PCA curvature computed (radius: " << radius << ")."
              << std::endl;
    std::cout << "  Gaussian curvature range: " << h2.minRangeValue() << " to "
              << h2.maxRangeValue() << std::endl;

    /****** General Principal Curvature Function ******/

    std::cout << "\n=== Principal Curvature (General Function) ==="
              << std::endl;

    vcl::TriMesh generalMesh = originalMesh;
    generalMesh.name()       = "General Curvature";

    {
        vcl::Timer timer("General principal curvature");
        vcl::updatePrincipalCurvature(
            generalMesh, vcl::PrincipalCurvatureAlgorithm::TAUBIN95);
        timer.stopAndPrint();
    }

    // Color mapping with maximum curvature
    for (auto& v : generalMesh.vertices()) {
        v.quality() = std::max(
            std::abs(v.principalCurvature().maxValue()),
            std::abs(v.principalCurvature().minValue()));
    }

    vcl::Histogramd h3 = vcl::vertexQualityHistogram(generalMesh);

    vcl::setPerVertexColorFromQuality(
        generalMesh,
        vcl::Color::ColorMap::RedBlue,
        h3.valueAtPercentile(0.1),
        h3.valueAtPercentile(0.9));

    std::cout << "General curvature computed." << std::endl;
    std::cout << "  Maximum curvature range: " << h3.minRangeValue() << " to "
              << h3.maxRangeValue() << std::endl;

    /****** Display statistics ******/

    std::cout << "\n=== Curvature Statistics ===" << std::endl;

    // Show curvature info for first vertex
    const auto& v = taubinMesh.vertex(0);
    std::cout << "Sample vertex curvature (Taubin95):" << std::endl;
    std::cout << "  Max value (k1): " << v.principalCurvature().maxValue()
              << std::endl;
    std::cout << "  Min value (k2): " << v.principalCurvature().minValue()
              << std::endl;
    std::cout << "  Mean curvature: "
              << (v.principalCurvature().maxValue() +
                  v.principalCurvature().minValue()) *
                     0.5
              << std::endl;
    std::cout << "  Gaussian curvature: "
              << v.principalCurvature().maxValue() *
                     v.principalCurvature().minValue()
              << std::endl;

    return std::make_tuple(originalMesh, taubinMesh, pcaMesh, generalMesh);
}

#endif // MESH_CURVATURE_H
