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

#ifndef MESH_MATRIX_CONVERSION_H
#define MESH_MATRIX_CONVERSION_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/space/core/array.h>

// This example demonstrates how to export meshes to matrices and import 
// matrices back to meshes, including:
// 1. Basic vertex positions and face indices
// 2. Additional mesh components (normals, colors)
// 3. Working with different matrix types (Eigen, VCLib Array2)
// 4. Creating meshes from matrices

vcl::Color randomColor()
{
    // generate random color using std::mt19937

    static std::mt19937                                gen;
    std::uniform_int_distribution<unsigned int> dist(0, 255);
    return vcl::Color(dist(gen), dist(gen), dist(gen), dist(gen));
}

auto matrixConversionExample()
{
    std::cout << "=== VCLib Example 010: Mesh Matrix Conversion ===\n\n";

    /****** Export: Mesh to Matrix ******/

    std::cout << "=== Export: Mesh to Matrix ===" << std::endl;

    // Load a textured mesh with normals and colors
    vcl::TriMesh mesh = vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny_textured.ply");
    vcl::updatePerVertexNormals(mesh);
    
    // Enable and set some colors for demonstration
    mesh.enablePerVertexColor();
    mesh.enablePerFaceColor(); 
    for (auto& v : mesh.vertices()) 
        v.color() = randomColor();
    for (auto& f : mesh.faces()) 
        f.color() = randomColor();

    std::cout << "Loaded mesh: " << mesh.vertexNumber() << " vertices, " 
              << mesh.faceNumber() << " faces" << std::endl;

    // Export basic geometry
    Eigen::MatrixXd vertices = vcl::vertexPositionsMatrix<Eigen::MatrixXd>(mesh);
    Eigen::MatrixXi faces = vcl::faceIndicesMatrix<Eigen::MatrixXi>(mesh);
    
    // Export additional components
    Eigen::MatrixXd vertexNormals = vcl::vertexNormalsMatrix<Eigen::MatrixXd>(mesh);
    Eigen::MatrixX4i vertexColors = vcl::vertexColorsMatrix<Eigen::MatrixX4i>(mesh);
    Eigen::MatrixX4i faceColors = vcl::faceColorsMatrix<Eigen::MatrixX4i>(mesh);

    std::cout << "Exported matrices:" << std::endl;
    std::cout << "- Vertices: " << vertices.rows() << "x" << vertices.cols() << std::endl;
    std::cout << "- Faces: " << faces.rows() << "x" << faces.cols() << std::endl;
    std::cout << "- Vertex normals: " << vertexNormals.rows() << "x" << vertexNormals.cols() << std::endl;
    std::cout << "- Vertex colors: " << vertexColors.rows() << "x" << vertexColors.cols() << std::endl;
    std::cout << "- Face colors: " << faceColors.rows() << "x" << faceColors.cols() << std::endl;

    /****** Import: Matrix to Mesh ******/

    std::cout << "\n=== Import: Matrix to Mesh ===" << std::endl;

    // Create a new mesh from the exported matrices
    vcl::TriMesh importedMesh = vcl::meshFromMatrices<vcl::TriMesh>(
        vertices, faces, vertexNormals);

    // Import additional components separately
    vcl::importVertexColorsFromMatrix(importedMesh, vertexColors);
    vcl::importFaceColorsFromMatrix(importedMesh, faceColors);

    std::cout << "Imported mesh: " << importedMesh.vertexNumber() << " vertices, " 
              << importedMesh.faceNumber() << " faces" << std::endl;

    // Verify data integrity
    std::cout << "Data verification:" << std::endl;
    std::cout << "- First vertex position: " << importedMesh.vertex(0).position() << std::endl;
    std::cout << "- First vertex normal: " << importedMesh.vertex(0).normal() << std::endl;
    std::cout << "- First vertex color: " << importedMesh.vertex(0).color() << std::endl;

    /****** Working with different matrix types ******/

    std::cout << "\n=== Different Matrix Types ===" << std::endl;

    // Load a polygon mesh and export with VCLib Array2
    vcl::PolyMesh polyMesh = vcl::load<vcl::PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");
    
    vcl::Array2<double> polyVertices = vcl::vertexPositionsMatrix<vcl::Array2<double>>(polyMesh);
    vcl::Array2<int> polyFaces = vcl::faceIndicesMatrix<vcl::Array2<int>>(polyMesh);

    std::cout << "Polygon mesh with VCLib Array2:" << std::endl;
    std::cout << "- Vertices: " << polyVertices.rows() << "x" << polyVertices.cols() << std::endl;
    std::cout << "- Faces: " << polyFaces.rows() << "x" << polyFaces.cols() << std::endl;

    /****** Creating meshes from scratch with matrices ******/

    std::cout << "\n=== Creating Mesh from Scratch ===" << std::endl;

    // Create simple cube data as Eigen matrices (faces in counterclockwise order)
    Eigen::MatrixXd cubeVertices(8, 3);
    cubeVertices << 
        -1, -1, -1,  // 0
         1, -1, -1,  // 1
         1,  1, -1,  // 2
        -1,  1, -1,  // 3
        -1, -1,  1,  // 4
         1, -1,  1,  // 5
         1,  1,  1,  // 6
        -1,  1,  1;  // 7

    Eigen::MatrixXi cubeFaces(12, 3);
    cubeFaces << 
        0, 2, 1,  0, 3, 2,  // bottom (z = -1)
        4, 5, 6,  4, 6, 7,  // top    (z = +1)
        0, 1, 5,  0, 5, 4,  // front  (y = -1)
        2, 3, 7,  2, 7, 6,  // back   (y = +1)
        0, 4, 7,  0, 7, 3,  // left   (x = -1)
        1, 2, 6,  1, 6, 5;  // right  (x = +1)

    // Create mesh from these matrices
    vcl::TriMesh cubeMesh = vcl::meshFromMatrices<vcl::TriMesh>(cubeVertices, cubeFaces);
    
    std::cout << "Created cube mesh: " << cubeMesh.vertexNumber() << " vertices, " 
              << cubeMesh.faceNumber() << " faces" << std::endl;

    return std::make_tuple(importedMesh, cubeMesh);
}

#endif // MESH_MATRIX_CONVERSION_H
