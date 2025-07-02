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

#include <vclib/meshes.h>
#include <vclib/io.h>

// This example demonstrates the basic concepts and operations with VCLib meshes:
// 1. Creating meshes from scratch
// 2. Adding vertices and faces 
// 3. Accessing and modifying mesh elements
// 4. Working with optional components
// 5. Basic mesh information and iteration

int main()
{
    std::cout << "=== VCLib Example 001: Mesh Basics ===\n\n";

    /****** Creating a mesh from scratch ******/
    
    std::cout << "=== Creating a Triangle Mesh ===" << std::endl;
    
    vcl::TriMesh mesh;
    
    // Add vertices to the mesh
    mesh.addVertex(vcl::Point3d(0, 0, 0));  // vertex 0
    mesh.addVertex(vcl::Point3d(1, 0, 0));  // vertex 1
    mesh.addVertex(vcl::Point3d(0, 1, 0));  // vertex 2
    mesh.addVertex(vcl::Point3d(1, 1, 0));  // vertex 3
    
    std::cout << "Added " << mesh.vertexNumber() << " vertices" << std::endl;
    
    // Add faces (triangles) using vertex indices
    mesh.addFace(0, 1, 2);  // first triangle
    mesh.addFace(1, 3, 2);  // second triangle
    
    std::cout << "Added " << mesh.faceNumber() << " faces" << std::endl;
    
    /****** Accessing mesh elements ******/
    
    std::cout << "\n=== Accessing Mesh Elements ===" << std::endl;
    
    // Access vertices by index
    std::cout << "Vertex 0 position: " << mesh.vertex(0).position() << std::endl;
    std::cout << "Vertex 1 position: " << mesh.vertex(1).position() << std::endl;
    
    // Access faces and their vertices
    std::cout << "Face 0 vertices: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << mesh.index(mesh.face(0).vertex(i)) << " ";
        // or mesh.face(0).vertexIndex(i)
    }
    std::cout << std::endl;
    
    // Modify vertex positions
    mesh.vertex(2).position() = vcl::Point3d(0.5, 1.5, 0.2);
    std::cout << "Modified vertex 2 position: " << mesh.vertex(2).position() << std::endl;
    
    /****** Iterating over mesh elements ******/
    
    std::cout << "\n=== Iterating Over Elements ===" << std::endl;
    
    // Iterate over all vertices
    std::cout << "All vertex positions:" << std::endl;
    for (const auto& vertex : mesh.vertices()) {
        std::cout << "  Vertex " << vertex.index()
                  << ": " << vertex.position() << std::endl;
    }
    
    // Iterate over all faces
    std::cout << "All faces:" << std::endl;
    for (const auto& face : mesh.faces()) {
        std::cout << "  Face " << face.index() << ": vertices [";
        for (int i = 0; i < 3; ++i) {
            std::cout << face.vertexIndex(i);
            if (i < 2) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
    
    /****** Working with optional components ******/
    
    std::cout << "\n=== Optional Components ===" << std::endl;
    
    // Check if color component is available
    std::cout << "Per-vertex color enabled: " << mesh.isPerVertexColorEnabled() << std::endl;
    
    // Enable per-vertex color component
    mesh.enablePerVertexColor();
    std::cout << "Enabled per-vertex color" << std::endl;
    std::cout << "Per-vertex color enabled: " << mesh.isPerVertexColorEnabled() << std::endl;
    
    // Set vertex colors
    mesh.vertex(0).color() = vcl::Color::Red;
    mesh.vertex(1).color() = vcl::Color::Green;
    mesh.vertex(2).color() = vcl::Color::Blue;
    mesh.vertex(3).color() = vcl::Color::Yellow;
    
    std::cout << "Set colors for all vertices" << std::endl;
    
    // Enable and set face colors
    mesh.enablePerFaceColor();
    mesh.face(0).color() = vcl::Color::Cyan;
    mesh.face(1).color() = vcl::Color::Magenta;
    
    std::cout << "Set colors for all faces" << std::endl;
    
    /****** Polygon Mesh example ******/
    
    std::cout << "\n=== Creating a Polygon Mesh ===" << std::endl;
    
    vcl::PolyMesh polyMesh;
    
    // Add vertices for a square
    polyMesh.addVertex(vcl::Point3d(0, 0, 0));
    polyMesh.addVertex(vcl::Point3d(2, 0, 0));
    polyMesh.addVertex(vcl::Point3d(2, 2, 0));
    polyMesh.addVertex(vcl::Point3d(0, 2, 0));
    
    // Add a quadrilateral face
    polyMesh.addFace();
    auto& face = polyMesh.face(0);
    
    // Polygon faces can have variable number of vertices
    face.pushVertex(&polyMesh.vertex(0)); // with the pointer of the vertex
    face.pushVertex(1u); // or with the index of the vertex
    face.pushVertex(&polyMesh.vertex(2));
    face.pushVertex(&polyMesh.vertex(3));
    
    std::cout << "Created polygon with " << face.vertexNumber() << " vertices" << std::endl;
    
    // Iterate over vertices of the polygon face
    std::cout << "Polygon vertices: ";
    for (vcl::uint i = 0; i < face.vertexNumber(); ++i) {
        std::cout << face.vertexIndex(i) << " ";
    }
    std::cout << std::endl;
    
    /****** Mesh statistics ******/
    
    std::cout << "\n=== Mesh Statistics ===" << std::endl;
    
    std::cout << "TriMesh:" << std::endl;
    std::cout << "  Vertices: " << mesh.vertexNumber() << std::endl;
    std::cout << "  Faces: " << mesh.faceNumber() << std::endl;
    std::cout << "  Container sizes - Vertices: " << mesh.vertexContainerSize() 
              << ", Faces: " << mesh.faceContainerSize() << std::endl;
    
    std::cout << "PolyMesh:" << std::endl;
    std::cout << "  Vertices: " << polyMesh.vertexNumber() << std::endl;
    std::cout << "  Faces: " << polyMesh.faceNumber() << std::endl;
    
    /****** Computing and displaying bounding box ******/
    
    std::cout << "\n=== Bounding Box ===" << std::endl;
    
    // Compute barycenter manually
    vcl::Point3d barycenter(0, 0, 0);
    for (const auto& vertex : mesh.vertices()) {
        barycenter += vertex.position();
    }
    barycenter /= mesh.vertexNumber();
    
    std::cout << "Mesh barycenter: " << barycenter << std::endl;
    
    // Find bounding box manually
    vcl::Point3d minPoint = mesh.vertex(0).position();
    vcl::Point3d maxPoint = mesh.vertex(0).position();
    
    for (const auto& vertex : mesh.vertices()) {
        const auto& pos = vertex.position();
        minPoint = vcl::Point3d(
            std::min(minPoint.x(), pos.x()),
            std::min(minPoint.y(), pos.y()),
            std::min(minPoint.z(), pos.z())
        );

        // the library provides vcl::min and vcl::max functions which, for
        // Points, return the point containing the minimum or maximum value
        // between each component of the two input points.
        maxPoint = vcl::max(maxPoint, pos);
    }
    
    std::cout << "Bounding box: min=" << minPoint << ", max=" << maxPoint << std::endl;
    
    /****** Save the created mesh ******/
    
    std::cout << "\n=== Saving Mesh ===" << std::endl;
    
    vcl::save(mesh, VCLIB_RESULTS_PATH "/001_mesh-basics_triangle.ply");
    vcl::save(polyMesh, VCLIB_RESULTS_PATH "/001_mesh-basics_polygon.ply");
    
    std::cout << "Saved meshes to results directory" << std::endl;
    
    return 0;
}
