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

#ifndef MESH_DISTANCE_H
#define MESH_DISTANCE_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates mesh distance calculation in VCLib:
// 1. Computing Hausdorff distance between two meshes
// 2. Smoothing a mesh and computing distance with original
// 3. Using different distance sampling methods

void meshDistance()
{
    std::cout << "=== VCLib Example 018: Mesh Distance ===\n\n";

    /****** Loading a mesh ******/

    std::cout << "=== Loading Mesh ===" << std::endl;

    vcl::TriMesh mesh = vcl::load<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    std::cout << "Loaded mesh with " << mesh.vertexNumber() << " vertices and "
              << mesh.faceNumber() << " faces" << std::endl;

    // Prepare mesh for distance computation
    vcl::updateBoundingBox(mesh);
    vcl::updatePerFaceNormals(mesh);

    /****** Create a smoothed version ******/

    std::cout << "\n=== Creating Smoothed Version ===" << std::endl;

    vcl::TriMesh smoothedMesh = mesh; // Copy the original mesh

    // Apply Laplacian smoothing
    vcl::laplacianSmoothing(smoothedMesh, 5); // 5 iterations

    std::cout << "Applied 5 iterations of Laplacian smoothing" << std::endl;

    // Update normals for the smoothed mesh
    vcl::updatePerFaceNormals(smoothedMesh);

    /****** Compute distance between original and smoothed mesh ******/

    std::cout << "\n=== Computing Distance (Original vs Smoothed) ===" << std::endl;

    vcl::ConsoleLogger log;

    // Compute Hausdorff distance using vertex uniform sampling
    auto result = vcl::hausdorffDistance(
        mesh, smoothedMesh, log, vcl::HAUSDORFF_VERTEX_UNIFORM);

    std::cout << "Hausdorff Distance Results:" << std::endl;
    std::cout << "  Min distance:  " << result.minDist << std::endl;
    std::cout << "  Max distance:  " << result.maxDist << std::endl;
    std::cout << "  Mean distance: " << result.meanDist << std::endl;
    std::cout << "  RMS distance:  " << result.RMSDist << std::endl;

    /****** Compare different smoothing methods ******/

    std::cout << "\n=== Comparing Smoothing Methods ===" << std::endl;

    // Create a Taubin smoothed version
    vcl::TriMesh taubinMesh = mesh;
    vcl::taubinSmoothing(taubinMesh, 5, 0.5f, -0.53f); // 5 iterations, lambda=0.5, mu=-0.53
    vcl::updatePerFaceNormals(taubinMesh);

    // Compute distance for Taubin smoothing
    auto taubinResult = vcl::hausdorffDistance(
        mesh, taubinMesh, log, vcl::HAUSDORFF_VERTEX_UNIFORM);

    std::cout << "Taubin Smoothing Distance Results:" << std::endl;
    std::cout << "  Min distance:  " << taubinResult.minDist << std::endl;
    std::cout << "  Max distance:  " << taubinResult.maxDist << std::endl;
    std::cout << "  Mean distance: " << taubinResult.meanDist << std::endl;
    std::cout << "  RMS distance:  " << taubinResult.RMSDist << std::endl;

    /****** Compare different sampling methods ******/

    std::cout << "\n=== Comparing Sampling Methods ===" << std::endl;

    // Monte Carlo sampling
    auto mcResult = vcl::hausdorffDistance(
        mesh, smoothedMesh, log, vcl::HAUSDORFF_MONTECARLO);

    std::cout << "Monte Carlo Sampling Results:" << std::endl;
    std::cout << "  Min distance:  " << mcResult.minDist << std::endl;
    std::cout << "  Max distance:  " << mcResult.maxDist << std::endl;
    std::cout << "  Mean distance: " << mcResult.meanDist << std::endl;
    std::cout << "  RMS distance:  " << mcResult.RMSDist << std::endl;

    /****** Load and compare two different meshes ******/

    std::cout << "\n=== Distance Between Different Meshes ===" << std::endl;

    vcl::TriMesh cube =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    vcl::updateBoundingBox(cube);
    vcl::updatePerFaceNormals(cube);

    auto cubeResult =
        vcl::hausdorffDistance(mesh, cube, log, vcl::HAUSDORFF_VERTEX_UNIFORM);

    std::cout << "Bunny vs Cube Distance Results:" << std::endl;
    std::cout << "  Min distance:  " << cubeResult.minDist << std::endl;
    std::cout << "  Max distance:  " << cubeResult.maxDist << std::endl;
    std::cout << "  Mean distance: " << cubeResult.meanDist << std::endl;
    std::cout << "  RMS distance:  " << cubeResult.RMSDist << std::endl;

    std::cout << "\nExample completed successfully!" << std::endl;
}

#endif // MESH_DISTANCE_H
