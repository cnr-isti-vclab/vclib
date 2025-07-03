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

#ifndef MESH_CREATION_H
#define MESH_CREATION_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

auto meshCreation()
{
    std::cout << "=== VCLib Example 003: Mesh Creation ===" << std::endl;

    /****** Creating Basic Primitives with TriMesh ******/

    std::cout << "\n=== Creating Basic Primitives (TriMesh) ===" << std::endl;

    // 1. Tetrahedron - The simplest 3D primitive
    std::cout << "Creating tetrahedron..." << std::endl;
    vcl::TriMesh tetrahedron = vcl::createTetrahedron<vcl::TriMesh>();
    std::cout << "  Vertices: " << tetrahedron.vertexNumber()
              << ", Faces: " << tetrahedron.faceNumber() << std::endl;

    // 2. Hexahedron (Cube) - Box primitive
    std::cout << "Creating hexahedron (cube)..." << std::endl;
    vcl::TriMesh cube = vcl::createHexahedron<vcl::TriMesh>();
    std::cout << "  Vertices: " << cube.vertexNumber()
              << ", Faces: " << cube.faceNumber() << std::endl;

    // 3. Dodecahedron - More complex polyhedron (triangulated)
    std::cout << "Creating dodecahedron (triangulated)..." << std::endl;
    vcl::TriMesh dodecahedron = vcl::createDodecahedron<vcl::TriMesh>();
    std::cout << "  Vertices: " << dodecahedron.vertexNumber()
              << ", Faces: " << dodecahedron.faceNumber() << std::endl;

    /****** Creating Parametric Primitives ******/

    std::cout << "\n=== Creating Parametric Primitives ===" << std::endl;

    // Custom cube with specific dimensions
    std::cout << "Creating custom cube..." << std::endl;
    vcl::TriMesh customCube = vcl::createCube<vcl::TriMesh>(
        vcl::Point3d(-2, -2, -2), 4.0); // min corner and edge length
    std::cout << "  Custom cube (4x4x4) - Vertices: "
              << customCube.vertexNumber()
              << ", Faces: " << customCube.faceNumber() << std::endl;

    /****** Creating Primitives with PolyMesh ******/

    std::cout << "\n=== Creating Primitives with PolyMesh ===" << std::endl;

    // PolyMesh can handle polygonal faces (not just triangles)

    // 1. Hexahedron as quads
    std::cout << "Creating hexahedron (PolyMesh - quads)..." << std::endl;
    vcl::PolyMesh cubeQuads = vcl::createHexahedron<vcl::PolyMesh>();
    std::cout << "  Cube (quads) - Vertices: " << cubeQuads.vertexNumber()
              << ", Faces: " << cubeQuads.faceNumber() << std::endl;
    // Note: This creates 6 quad faces instead of 12 triangular faces

    // 2. Dodecahedron as pentagons
    std::cout << "Creating dodecahedron (PolyMesh - pentagons)..." << std::endl;
    vcl::PolyMesh dodecahedronPoly = vcl::createDodecahedron<vcl::PolyMesh>();
    std::cout << "  Dodecahedron (pentagons) - Vertices: "
              << dodecahedronPoly.vertexNumber()
              << ", Faces: " << dodecahedronPoly.faceNumber() << std::endl;
    // Note: This creates 12 pentagonal faces instead of many triangular faces

    // 5. Sphere with PolyMesh (quad faces)
    std::cout << "Creating sphere (PolyMesh - quads)..." << std::endl;
    vcl::Sphere<vcl::TriMesh::ScalarType> sphere(
        vcl::Point3<vcl::TriMesh::ScalarType>(0, 0, 0), 1.0);
    vcl::Sphere<vcl::PolyMesh::ScalarType> spherePoly =
        sphere.cast<vcl::PolyMesh::ScalarType>();
    vcl::PolyMesh sphereQuads =
        vcl::createSphereSpherifiedCube<vcl::PolyMesh>(spherePoly, 20);
    std::cout << "  Sphere (quads) - Vertices: " << sphereQuads.vertexNumber()
              << ", Faces: " << sphereQuads.faceNumber() << std::endl;

    /****** Summary and Comparison ******/

    std::cout << "\n=== Summary: TriMesh vs PolyMesh ===" << std::endl;
    std::cout << "TriMesh:" << std::endl;
    std::cout << "  - All faces are triangles" << std::endl;
    std::cout << "  - More faces for complex shapes" << std::endl;
    std::cout << "  - Compatible with most graphics pipelines" << std::endl;
    std::cout << "  - Cube: " << cube.faceNumber() << " triangular faces"
              << std::endl;
    std::cout << "  - Dodecahedron: " << dodecahedron.faceNumber()
              << " triangular faces" << std::endl;

    std::cout << "\nPolyMesh:" << std::endl;
    std::cout << "  - Faces can have arbitrary number of vertices" << std::endl;
    std::cout << "  - More compact representation for regular shapes"
              << std::endl;
    std::cout << "  - Better preserves original geometry intent" << std::endl;
    std::cout << "  - Cube: " << cubeQuads.faceNumber() << " quad faces"
              << std::endl;
    std::cout << "  - Dodecahedron: " << dodecahedronPoly.faceNumber()
              << " pentagonal faces" << std::endl;

    std::cout << "Example completed successfully!" << std::endl;

    return std::make_tuple(
        tetrahedron,
        cube,
        dodecahedron,
        customCube,
        cubeQuads,
        dodecahedronPoly,
        sphereQuads);
}

#endif // MESH_CREATION_H
