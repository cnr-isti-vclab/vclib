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

#ifndef MESH_SPHERE_INTERSECTION_H
#define MESH_SPHERE_INTERSECTION_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates mesh-sphere intersection using different sphere
// sizes and positions to show various intersection scenarios

auto meshSphereIntersection()
{
    std::cout << "=== VCLib Example 021: Mesh-Sphere Intersection ===\n\n";

    /****** Load base mesh ******/

    std::cout << "=== Loading Base Mesh ===" << std::endl;

    vcl::TriMesh originalMesh =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    originalMesh.name() = "Original Bunny";

    vcl::updatePerFaceNormals(originalMesh);
    vcl::updatePerVertexNormals(originalMesh);

    std::cout << "Loaded bunny mesh: " << originalMesh.vertexNumber()
              << " vertices, " << originalMesh.faceNumber() << " faces"
              << std::endl;

    /****** Define spheres for intersection ******/

    std::cout << "\n=== Defining Intersection Spheres ===" << std::endl;

    // Center sphere that intersects the bunny's body
    vcl::Sphere<vcl::TriMesh::ScalarType> centerSphere(
        {0.28, 0.18, 0.49}, 0.08);

    // Smaller sphere that intersects only part of the ears
    vcl::Sphere<vcl::TriMesh::ScalarType> earSphere({0.25, 0.47, 0.27}, 0.04);

    // Larger sphere that encompasses most of the mesh
    vcl::Sphere<vcl::TriMesh::ScalarType> largeSphere({0.28, 0.18, 0.49}, 0.15);

    std::cout << "Center sphere: radius " << centerSphere.radius() << " at ("
              << centerSphere.center().x() << ", " << centerSphere.center().y()
              << ", " << centerSphere.center().z() << ")" << std::endl;
    std::cout << "Ear sphere: radius " << earSphere.radius() << " at ("
              << earSphere.center().x() << ", " << earSphere.center().y()
              << ", " << earSphere.center().z() << ")" << std::endl;
    std::cout << "Large sphere: radius " << largeSphere.radius() << " at ("
              << largeSphere.center().x() << ", " << largeSphere.center().y()
              << ", " << largeSphere.center().z() << ")" << std::endl;

    /****** Compute intersections ******/

    std::cout << "\n=== Computing Intersections ===" << std::endl;

    // Center intersection
    vcl::TriMesh centerSphereIntersection =
        vcl::intersection(originalMesh, centerSphere);
    centerSphereIntersection.name() = "Bunny-Center Sphere Intersection";

    // Ear intersection
    vcl::TriMesh earSphereIntersection =
        vcl::intersection(originalMesh, earSphere);
    earSphereIntersection.name() = "Bunny-Ear Sphere Intersection";

    // Large intersection
    vcl::TriMesh largeSphereIntersection =
        vcl::intersection(originalMesh, largeSphere);
    largeSphereIntersection.name() = "Bunny-Large Sphere Intersection";

    std::cout << "Center sphere intersection: "
              << centerSphereIntersection.vertexNumber() << " vertices, "
              << centerSphereIntersection.faceNumber() << " faces" << std::endl;
    std::cout << "Ear sphere intersection: "
              << earSphereIntersection.vertexNumber() << " vertices, "
              << earSphereIntersection.faceNumber() << " faces" << std::endl;
    std::cout << "Large sphere intersection: "
              << largeSphereIntersection.vertexNumber() << " vertices, "
              << largeSphereIntersection.faceNumber() << " faces" << std::endl;

    /****** Optional: Create sphere meshes for visualization ******/

    vcl::TriMesh centerSphereMesh =
        vcl::createSphere<vcl::TriMesh>(centerSphere);
    centerSphereMesh.name() = "Center Sphere";

    vcl::TriMesh earSphereMesh = vcl::createSphere<vcl::TriMesh>(earSphere);
    earSphereMesh.name()       = "Ear Sphere";

    vcl::TriMesh largeSphereMesh = vcl::createSphere<vcl::TriMesh>(largeSphere);
    largeSphereMesh.name()       = "Large Sphere";

    return std::make_tuple(
        originalMesh,
        centerSphereMesh,
        centerSphereIntersection,
        earSphereMesh,
        earSphereIntersection,
        largeSphereMesh,
        largeSphereIntersection);
}

#endif // MESH_SPHERE_INTERSECTION_H
