// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MESH_INTERSECTION_H
#define MESH_INTERSECTION_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates mesh intersections with spheres and planes:
// 1. Mesh-sphere intersection using different sphere sizes and positions
// 2. Mesh-plane intersection producing an EdgeMesh section contour

inline auto meshIntersection()
{
    using SSphere = vcl::Sphere<vcl::ScalarType<vcl::TriMesh>>;
    using SPlane  = vcl::Plane<vcl::ScalarType<vcl::TriMesh>>;

    std::cout << "=== VCLib Example 021: Mesh Intersection ===\n\n";

    /****** Load base mesh ******/

    std::cout << "=== Loading Base Mesh ===" << std::endl;

    vcl::TriMesh originalMesh =
        vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    originalMesh.name() = "Original Bunny";

    vcl::updateFaceNormals(originalMesh);
    vcl::updateVertexNormals(originalMesh);

    std::cout << "Loaded bunny mesh: " << originalMesh.vertexCount()
              << " vertices, " << originalMesh.faceCount() << " faces"
              << std::endl;

    /****** Define spheres for intersection ******/

    std::cout << "\n=== Defining Intersection Spheres ===" << std::endl;

    // Center sphere that intersects the bunny's body
    SSphere centerSphere({0.28, 0.18, 0.49}, 0.08);

    // Smaller sphere that intersects only part of the ears
    SSphere earSphere({0.25, 0.47, 0.27}, 0.04);

    // Larger sphere that encompasses most of the mesh
    SSphere largeSphere({0.28, 0.18, 0.49}, 0.15);

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
              << centerSphereIntersection.vertexCount() << " vertices, "
              << centerSphereIntersection.faceCount() << " faces" << std::endl;
    std::cout << "Ear sphere intersection: "
              << earSphereIntersection.vertexCount() << " vertices, "
              << earSphereIntersection.faceCount() << " faces" << std::endl;
    std::cout << "Large sphere intersection: "
              << largeSphereIntersection.vertexCount() << " vertices, "
              << largeSphereIntersection.faceCount() << " faces" << std::endl;

    /****** Optional: Create sphere meshes for visualization ******/

    vcl::TriMesh centerSphereMesh =
        vcl::createSphere<vcl::TriMesh>(centerSphere);
    centerSphereMesh.name() = "Center Sphere";

    vcl::TriMesh earSphereMesh = vcl::createSphere<vcl::TriMesh>(earSphere);
    earSphereMesh.name()       = "Ear Sphere";

    vcl::TriMesh largeSphereMesh = vcl::createSphere<vcl::TriMesh>(largeSphere);
    largeSphereMesh.name()       = "Large Sphere";

    /****** Mesh-plane intersection ******/

    std::cout << "\n=== Mesh-Plane Intersection ===" << std::endl;

    SPlane cutPlane(vcl::Point3d(0.28, 0.18, 0.49), vcl::Point3d(0.0, 1.0, 0.0));

    vcl::EdgeMesh planeIntersection =
        vcl::intersection<vcl::EdgeMesh>(originalMesh, cutPlane);
    planeIntersection.name() = "Bunny-Plane Intersection";

    std::cout << "Plane intersection: " << planeIntersection.vertexCount()
              << " vertices, " << planeIntersection.edgeCount() << " edges"
              << std::endl;

    return std::make_tuple(
        originalMesh,
        centerSphereMesh,
        centerSphereIntersection,
        earSphereMesh,
        earSphereIntersection,
        largeSphereMesh,
        largeSphereIntersection,
        planeIntersection);
}

#endif // MESH_INTERSECTION_H
