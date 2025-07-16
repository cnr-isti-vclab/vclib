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

#ifndef MESH_SMOOTHING_H
#define MESH_SMOOTHING_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/misc/timer.h>

// This example demonstrates mesh smoothing algorithms:
// 1. Laplacian smoothing with uniform weights
// 2. Laplacian smoothing with cotangent weights
// 3. Taubin smoothing (low-pass filter)
// 4. Per-vertex normal smoothing for point clouds

auto meshSmoothing()
{
    std::cout << "=== VCLib Example 014: Mesh Smoothing ===\n\n";

    /****** Load a mesh for smoothing ******/

    std::cout << "=== Loading Bunny Mesh ===" << std::endl;

    vcl::TriMesh originalMesh =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    originalMesh.name() = "Original Bunny Mesh";

    std::cout << "Original mesh loaded:" << std::endl;
    std::cout << "  Vertices: " << originalMesh.vertexNumber() << std::endl;
    std::cout << "  Faces: " << originalMesh.faceNumber() << std::endl;

    vcl::updatePerVertexAndFaceNormals(originalMesh);

    /****** Laplacian Smoothing with Uniform Weights ******/

    std::cout << "\n=== Laplacian Smoothing (Uniform Weights) ===" << std::endl;

    vcl::TriMesh laplacianMesh = originalMesh;
    laplacianMesh.name()       = "Laplacian Smoothed (Uniform)";

    {
        vcl::Timer timer("Laplacian smoothing (uniform)");
        vcl::laplacianSmoothing(laplacianMesh, 10, false, false);
        timer.stopAndPrint();
    }

    std::cout << "Laplacian smoothing (10 iterations) completed." << std::endl;

    vcl::updatePerVertexAndFaceNormals(laplacianMesh);

    /****** Laplacian Smoothing with Cotangent Weights ******/

    std::cout << "\n=== Laplacian Smoothing (Cotangent Weights) ==="
              << std::endl;

    vcl::TriMesh cotangentMesh = originalMesh;
    cotangentMesh.name()       = "Laplacian Smoothed (Cotangent)";

    {
        vcl::Timer timer("Laplacian smoothing (cotangent)");
        vcl::laplacianSmoothing(cotangentMesh, 10, false, true);
        timer.stopAndPrint();
    }

    std::cout << "Cotangent Laplacian smoothing (10 iterations) completed."
              << std::endl;

    vcl::updatePerVertexAndFaceNormals(cotangentMesh);

    /****** Taubin Smoothing ******/

    std::cout << "\n=== Taubin Smoothing ===" << std::endl;

    vcl::TriMesh taubinMesh = originalMesh;
    taubinMesh.name()       = "Taubin Smoothed";

    {
        vcl::Timer timer("Taubin smoothing");
        vcl::taubinSmoothing(taubinMesh, 50, 0.5f, -0.53f, false);
        timer.stopAndPrint();
    }

    std::cout << "Taubin smoothing (50 iterations, λ=0.5, μ=-0.53) completed."
              << std::endl;

    vcl::updatePerVertexAndFaceNormals(taubinMesh);

    /****** Normal Smoothing for Point Cloud ******/

    std::cout << "\n=== Normal Smoothing ===" << std::endl;

    vcl::TriMesh normalMesh = originalMesh;
    normalMesh.name()       = "Normal Smoothed";

    {
        vcl::Timer timer("Normal smoothing");
        vcl::smoothPerVertexNormalsPointCloud(normalMesh, 10, 3);
        timer.stopAndPrint();
    }

    std::cout
        << "Per-vertex normal smoothing (10 neighbors, 3 iterations) completed."
        << std::endl;

    /****** Display mesh statistics ******/

    std::cout << "\n=== Mesh Statistics ===" << std::endl;
    std::cout << "All meshes have:" << std::endl;
    std::cout << "  Vertices: " << originalMesh.vertexNumber() << std::endl;
    std::cout << "  Faces: " << originalMesh.faceNumber() << std::endl;

    return std::make_tuple(
        originalMesh, laplacianMesh, cotangentMesh, taubinMesh, normalMesh);
}

#endif // MESH_SMOOTHING_H
