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

#include <iostream>

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates mesh creation in VCLib:
// 1. Creating basic geometric primitives (tetrahedron, cube, sphere, etc.)
// 2. Understanding the difference between TriMesh and PolyMesh
// 3. Different sphere generation algorithms
// 4. Creating meshes with custom parameters
// 5. Saving created meshes to files

int main()
{
    /****** Setup save settings ******/
    
    std::cout << "=== VCLib Mesh Creation Tutorial ===" << std::endl;
    
    // Configure which mesh data to save
    vcl::MeshInfo info;
    info.setVertices();
    info.setFaces();
    info.setPerVertexPosition(true, vcl::PrimitiveType::FLOAT);
    info.setPerFaceVertexReferences();

    vcl::SaveSettings settings;
    settings.info = info;
    
    /****** Creating Basic Primitives with TriMesh ******/
    
    std::cout << "\n=== Creating Basic Primitives (TriMesh) ===" << std::endl;
    
    // 1. Tetrahedron - The simplest 3D primitive
    std::cout << "Creating tetrahedron..." << std::endl;
    vcl::TriMesh tetrahedron = vcl::createTetrahedron<vcl::TriMesh>();
    std::cout << "  Vertices: " << tetrahedron.vertexNumber() 
              << ", Faces: " << tetrahedron.faceNumber() << std::endl;
    vcl::savePly(tetrahedron, VCLIB_RESULTS_PATH "/003_tetrahedron.ply", settings);
    
    // 2. Hexahedron (Cube) - Box primitive
    std::cout << "Creating hexahedron (cube)..." << std::endl;
    vcl::TriMesh cube = vcl::createHexahedron<vcl::TriMesh>();
    std::cout << "  Vertices: " << cube.vertexNumber() 
              << ", Faces: " << cube.faceNumber() << std::endl;
    vcl::savePly(cube, VCLIB_RESULTS_PATH "/003_cube_tri.ply", settings);
    
    // 3. Icosahedron - Regular polyhedron with 20 faces
    std::cout << "Creating icosahedron..." << std::endl;
    vcl::TriMesh icosahedron = vcl::createIcosahedron<vcl::TriMesh>(true); // normalized vertices
    std::cout << "  Vertices: " << icosahedron.vertexNumber() 
              << ", Faces: " << icosahedron.faceNumber() << std::endl;
    vcl::savePly(icosahedron, VCLIB_RESULTS_PATH "/003_icosahedron.ply", settings);
    
    // 4. Dodecahedron - More complex polyhedron (triangulated)
    std::cout << "Creating dodecahedron (triangulated)..." << std::endl;
    vcl::ConsoleLogger log;
    log.enablePrintTimer();
    log.startTimer();
    vcl::TriMesh dodecahedron = vcl::createDodecahedron<vcl::TriMesh>(log);
    std::cout << "  Vertices: " << dodecahedron.vertexNumber() 
              << ", Faces: " << dodecahedron.faceNumber() << std::endl;
    vcl::savePly(dodecahedron, VCLIB_RESULTS_PATH "/003_dodecahedron_tri.ply", settings);
    
    /****** Creating Parametric Primitives ******/
    
    std::cout << "\n=== Creating Parametric Primitives ===" << std::endl;
    
    // Custom cube with specific dimensions
    std::cout << "Creating custom cube..." << std::endl;
    vcl::TriMesh customCube = vcl::createCube<vcl::TriMesh>(
        vcl::Point3d(-2, -2, -2), 4.0);  // min corner and edge length
    std::cout << "  Custom cube (4x4x4) - Vertices: " << customCube.vertexNumber() 
              << ", Faces: " << customCube.faceNumber() << std::endl;
    vcl::savePly(customCube, VCLIB_RESULTS_PATH "/003_custom_cube.ply", settings);
    
    // Custom tetrahedron with specific points
    std::cout << "Creating custom tetrahedron..." << std::endl;
    vcl::TriMesh customTetrahedron = vcl::createTetrahedron<vcl::TriMesh>(
        vcl::Point3d(0, 0, 0),     // apex
        vcl::Point3d(2, 0, 0),     // base vertex 1
        vcl::Point3d(1, 2, 0),     // base vertex 2
        vcl::Point3d(1, 1, 2));    // base vertex 3
    std::cout << "  Custom tetrahedron - Vertices: " << customTetrahedron.vertexNumber() 
              << ", Faces: " << customTetrahedron.faceNumber() << std::endl;
    vcl::savePly(customTetrahedron, VCLIB_RESULTS_PATH "/003_custom_tetrahedron.ply", settings);
    
    /****** Creating Spheres with Different Algorithms ******/
    
    std::cout << "\n=== Creating Spheres with Different Algorithms ===" << std::endl;
    
    // Create a sphere centered at origin with radius 1
    vcl::Sphere<vcl::TriMesh::ScalarType> sphere(vcl::Point3<vcl::TriMesh::ScalarType>(0, 0, 0), 1.0);
    
    // 1. UV Sphere (latitude/longitude parametrization)
    std::cout << "Creating UV sphere..." << std::endl;
    vcl::TriMesh uvSphere = vcl::createSphereUV<vcl::TriMesh>(sphere, 20, 40); // 20 parallels, 40 meridians
    std::cout << "  UV Sphere (20x40) - Vertices: " << uvSphere.vertexNumber() 
              << ", Faces: " << uvSphere.faceNumber() << std::endl;
    vcl::savePly(uvSphere, VCLIB_RESULTS_PATH "/003_sphere_uv.ply", settings);
    
    // 2. Normalized Cube Sphere
    std::cout << "Creating normalized cube sphere..." << std::endl;
    vcl::TriMesh normalizedCubeSphere = vcl::createSphereNormalizedCube<vcl::TriMesh>(sphere, 30);
    std::cout << "  Normalized Cube Sphere (30 divisions) - Vertices: " << normalizedCubeSphere.vertexNumber() 
              << ", Faces: " << normalizedCubeSphere.faceNumber() << std::endl;
    vcl::savePly(normalizedCubeSphere, VCLIB_RESULTS_PATH "/003_sphere_normalized_cube.ply", settings);
    
    // 3. Spherified Cube
    std::cout << "Creating spherified cube..." << std::endl;
    vcl::TriMesh spherifiedCube = vcl::createSphereSpherifiedCube<vcl::TriMesh>(sphere, 30);
    std::cout << "  Spherified Cube (30 divisions) - Vertices: " << spherifiedCube.vertexNumber() 
              << ", Faces: " << spherifiedCube.faceNumber() << std::endl;
    vcl::savePly(spherifiedCube, VCLIB_RESULTS_PATH "/003_sphere_spherified_cube.ply", settings);
    
    // 4. Icosahedron-based Sphere
    std::cout << "Creating icosahedron sphere..." << std::endl;
    vcl::TriMesh icosahedronSphere = vcl::createSphereIcosahedron<vcl::TriMesh>(sphere, 3);
    std::cout << "  Icosahedron Sphere (3 subdivisions) - Vertices: " << icosahedronSphere.vertexNumber() 
              << ", Faces: " << icosahedronSphere.faceNumber() << std::endl;
    vcl::savePly(icosahedronSphere, VCLIB_RESULTS_PATH "/003_sphere_icosahedron.ply", settings);
    
    // 5. Using the general createSphere function with different parameters
    std::cout << "Creating sphere with CreateSphereArgs..." << std::endl;
    vcl::CreateSphereArgs args;
    args.mode = vcl::CreateSphereArgs::UV;
    args.parallels = 15;
    args.meridians = 30;
    vcl::TriMesh generalSphere = vcl::createSphere<vcl::TriMesh>(sphere, args);
    std::cout << "  General Sphere (UV mode, 15x30) - Vertices: " << generalSphere.vertexNumber() 
              << ", Faces: " << generalSphere.faceNumber() << std::endl;
    vcl::savePly(generalSphere, VCLIB_RESULTS_PATH "/003_sphere_general.ply", settings);
    
    /****** Creating Primitives with PolyMesh ******/
    
    std::cout << "\n=== Creating Primitives with PolyMesh ===" << std::endl;
    
    // PolyMesh can handle polygonal faces (not just triangles)
    
    // 1. Hexahedron as quads
    std::cout << "Creating hexahedron (PolyMesh - quads)..." << std::endl;
    vcl::PolyMesh cubeQuads = vcl::createHexahedron<vcl::PolyMesh>();
    std::cout << "  Cube (quads) - Vertices: " << cubeQuads.vertexNumber() 
              << ", Faces: " << cubeQuads.faceNumber() << std::endl;
    // Note: This creates 6 quad faces instead of 12 triangular faces
    vcl::savePly(cubeQuads, VCLIB_RESULTS_PATH "/003_cube_poly.ply", settings);
    
    // 2. Dodecahedron as pentagons
    std::cout << "Creating dodecahedron (PolyMesh - pentagons)..." << std::endl;
    vcl::PolyMesh dodecahedronPoly = vcl::createDodecahedron<vcl::PolyMesh>();
    std::cout << "  Dodecahedron (pentagons) - Vertices: " << dodecahedronPoly.vertexNumber() 
              << ", Faces: " << dodecahedronPoly.faceNumber() << std::endl;
    // Note: This creates 12 pentagonal faces instead of many triangular faces
    vcl::savePly(dodecahedronPoly, VCLIB_RESULTS_PATH "/003_dodecahedron_poly.ply", settings);
    
    // 3. Custom cube with specific position and size
    std::cout << "Creating custom cube (PolyMesh)..." << std::endl;
    vcl::PolyMesh customCubePoly = vcl::createCube<vcl::PolyMesh>(
        vcl::Point3d(5, 5, 5), 3.0);  // offset position
    std::cout << "  Custom cube (3x3x3 at (5,5,5)) - Vertices: " << customCubePoly.vertexNumber() 
              << ", Faces: " << customCubePoly.faceNumber() << std::endl;
    vcl::savePly(customCubePoly, VCLIB_RESULTS_PATH "/003_custom_cube_poly.ply", settings);
    
    // 4. Sphere with PolyMesh (quad faces)
    std::cout << "Creating sphere (PolyMesh - quads)..." << std::endl;
    vcl::Sphere<vcl::PolyMesh::ScalarType> spherePoly = sphere.cast<vcl::PolyMesh::ScalarType>();
    vcl::PolyMesh sphereQuads = vcl::createSphereSpherifiedCube<vcl::PolyMesh>(spherePoly, 20);
    std::cout << "  Sphere (quads) - Vertices: " << sphereQuads.vertexNumber() 
              << ", Faces: " << sphereQuads.faceNumber() << std::endl;
    vcl::savePly(sphereQuads, VCLIB_RESULTS_PATH "/003_sphere_poly.ply", settings);
    
    /****** Creating Cone and Cylinder Primitives ******/
    
    std::cout << "\n=== Creating Cone and Cylinder Primitives ===" << std::endl;
    
    // 1. Cone
    std::cout << "Creating cone..." << std::endl;
    vcl::TriMesh cone = vcl::createCone<vcl::TriMesh>(
        1.0,  // radius bottom
        0.0,  // radius top (0 = pointed cone)
        2.0,  // height
        24);  // subdivisions
    std::cout << "  Cone - Vertices: " << cone.vertexNumber() 
              << ", Faces: " << cone.faceNumber() << std::endl;
    vcl::savePly(cone, VCLIB_RESULTS_PATH "/003_cone.ply", settings);
    
    // 2. Cylinder
    std::cout << "Creating cylinder..." << std::endl;
    vcl::TriMesh cylinder = vcl::createCylinder<vcl::TriMesh>(
        0.8,  // radius
        3.0,  // height
        32);  // subdivisions
    std::cout << "  Cylinder - Vertices: " << cylinder.vertexNumber() 
              << ", Faces: " << cylinder.faceNumber() << std::endl;
    vcl::savePly(cylinder, VCLIB_RESULTS_PATH "/003_cylinder.ply", settings);
    
    // 3. Truncated cone
    std::cout << "Creating truncated cone..." << std::endl;
    vcl::TriMesh truncatedCone = vcl::createCone<vcl::TriMesh>(
        1.5,  // radius bottom
        0.5,  // radius top (non-zero = truncated)
        2.5,  // height
        20);  // subdivisions
    std::cout << "  Truncated Cone - Vertices: " << truncatedCone.vertexNumber() 
              << ", Faces: " << truncatedCone.faceNumber() << std::endl;
    vcl::savePly(truncatedCone, VCLIB_RESULTS_PATH "/003_truncated_cone.ply", settings);
    
    /****** Summary and Comparison ******/
    
    std::cout << "\n=== Summary: TriMesh vs PolyMesh ===" << std::endl;
    std::cout << "TriMesh:" << std::endl;
    std::cout << "  - All faces are triangles" << std::endl;
    std::cout << "  - More faces for complex shapes" << std::endl;
    std::cout << "  - Compatible with most graphics pipelines" << std::endl;
    std::cout << "  - Cube: " << cube.faceNumber() << " triangular faces" << std::endl;
    std::cout << "  - Dodecahedron: " << dodecahedron.faceNumber() << " triangular faces" << std::endl;
    
    std::cout << "\nPolyMesh:" << std::endl;
    std::cout << "  - Faces can have arbitrary number of vertices" << std::endl;
    std::cout << "  - More compact representation for regular shapes" << std::endl;
    std::cout << "  - Better preserves original geometry intent" << std::endl;
    std::cout << "  - Cube: " << cubeQuads.faceNumber() << " quad faces" << std::endl;
    std::cout << "  - Dodecahedron: " << dodecahedronPoly.faceNumber() << " pentagonal faces" << std::endl;
    
    std::cout << "\n=== Sphere Generation Algorithms ===" << std::endl;
    std::cout << "UV Sphere: Good for texturing, has poles" << std::endl;
    std::cout << "Normalized Cube: More uniform distribution" << std::endl;
    std::cout << "Spherified Cube: Smoother than normalized cube" << std::endl;
    std::cout << "Icosahedron: Most uniform, suitable for subdivision" << std::endl;
    
    std::cout << "\nAll meshes have been saved to the results directory." << std::endl;
    std::cout << "Example completed successfully!" << std::endl;
    
    return 0;
}
