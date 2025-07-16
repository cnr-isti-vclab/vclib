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

#ifndef MESH_POINT_SAMPLING_H
#define MESH_POINT_SAMPLING_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates various point sampling algorithms available in
// VCLib:
// 1. All vertices sampling
// 2. Uniform face sampling
// 3. Montecarlo sampling
// 4. Poisson sampling

auto meshPointSampling()
{
    std::cout << "=== VCLib Example 019: Mesh Point Sampling ===\n\n";

    /****** Load a mesh ******/

    std::cout << "=== Loading Mesh ===" << std::endl;

    vcl::TriMesh originalMesh =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    originalMesh.name() = "Original Bunny Mesh";

    vcl::updatePerVertexAndFaceNormals(originalMesh);

    std::cout << "Loaded mesh with " << originalMesh.vertexNumber()
              << " vertices and " << originalMesh.faceNumber() << " faces"
              << std::endl;

    /****** All Vertices Sampling ******/

    std::cout << "\n=== All Vertices Sampling ===" << std::endl;

    // Sample all vertices of the mesh
    auto allVerticesSampler =
        vcl::allVerticesPointSampling<vcl::MeshSampler<vcl::PointCloud>>(
            originalMesh);
    vcl::PointCloud allVertices = allVerticesSampler.samples();
    allVertices.name()          = "All Vertices Sampling";

    std::cout << "Sampled " << allVertices.vertexNumber()
              << " vertices (all vertices)" << std::endl;

    /****** Uniform Face Sampling ******/

    std::cout << "\n=== Uniform Face Sampling ===" << std::endl;

    // Sample 500 points uniformly from face centers
    vcl::uint numSamples = 500;
    auto      uniformSampler =
        vcl::faceUniformPointSampling<vcl::MeshSampler<vcl::PointCloud>>(
            originalMesh, numSamples);
    vcl::PointCloud uniform = uniformSampler.samples();
    uniform.name()          = "Uniform Face Sampling";

    std::cout << "Sampled " << uniform.vertexNumber()
              << " points using uniform face sampling" << std::endl;

    /****** Montecarlo Sampling ******/

    std::cout << "\n=== Montecarlo Sampling ===" << std::endl;

    // Sample 750 points using montecarlo method
    numSamples = 750;
    auto montecarloSampler =
        vcl::montecarloPointSampling<vcl::MeshSampler<vcl::PointCloud>>(
            originalMesh, numSamples);
    vcl::PointCloud montecarlo = montecarloSampler.samples();

    std::cout << "Sampled " << montecarlo.vertexNumber()
              << " points using montecarlo sampling" << std::endl;
    montecarlo.name() = "Montecarlo Sampling";

    /****** Poisson Sampling ******/

    std::cout << "\n=== Poisson Sampling ===" << std::endl;

    // Sample using poisson distribution (approximate number of samples)
    double samplePerAreaUnit = 1000.0;
    auto   poissonSampler =
        vcl::montecarloPoissonPointSampling<vcl::MeshSampler<vcl::PointCloud>>(
            originalMesh, samplePerAreaUnit);
    vcl::PointCloud poisson = poissonSampler.samples();
    poisson.name()          = "Poisson Sampling";

    std::cout << "Sampled " << poisson.vertexNumber()
              << " points using poisson sampling (approximate)" << std::endl;

    /****** Sampling comparison ******/

    std::cout << "\n=== Sampling Summary ===" << std::endl;
    std::cout << "Original mesh: " << originalMesh.vertexNumber()
              << " vertices, " << originalMesh.faceNumber() << " faces"
              << std::endl;
    std::cout << "All vertices:  " << allVertices.vertexNumber() << " points"
              << std::endl;
    std::cout << "Uniform:       " << uniform.vertexNumber() << " points"
              << std::endl;
    std::cout << "Montecarlo:    " << montecarlo.vertexNumber() << " points"
              << std::endl;
    std::cout << "Poisson:       " << poisson.vertexNumber() << " points"
              << std::endl;

    return std::make_tuple(
        originalMesh, allVertices, uniform, montecarlo, poisson);
}

#endif // MESH_POINT_SAMPLING_H
