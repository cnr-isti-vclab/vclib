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
#include <vclib/views.h>

// This example demonstrates various ways to iterate over mesh elements:
// 1. Basic iteration over vertices and faces
// 2. Iterating over adjacent elements (vertex-face, face-vertex relationships)
// 3. Using VCLib views for advanced iteration patterns
// 4. Filtering and transforming during iteration
// 5. Working with polygon meshes and variable topology

int main()
{
    std::cout << "=== VCLib Example 002: Mesh Iteration ===\n\n";

    /****** Load a test mesh ******/
    
    std::cout << "=== Loading Test Mesh ===" << std::endl;
    
    vcl::TriMesh mesh = vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");
    
    std::cout << "Loaded mesh with " << mesh.vertexNumber() 
              << " vertices and " << mesh.faceNumber() << " faces" << std::endl;
    
    /****** Basic iteration ******/
    
    std::cout << "\n=== Basic Iteration ===" << std::endl;
    
    // Iterate with range-based for loop
    std::cout << "First 3 vertex positions:" << std::endl;
    for (int count = 0; const auto& vertex : mesh.vertices()) {
        if (count >= 3) break;
        std::cout << "  Vertex " << vertex.index() 
                  << ": " << vertex.position() << std::endl;
        count++;
    }
    
    // Iterate with traditional for loop using indices
    std::cout << "\nFirst 3 faces (using indices):" << std::endl;
    for (vcl::uint i = 0; i < std::min(3u, mesh.faceNumber()); ++i) {
        const auto& face = mesh.face(i);
        std::cout << "  Face " << i << ": vertices [";
        for (int j = 0; j < 3; ++j) {
            std::cout << face.vertexIndex(j);
            if (j < 2) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }
    
    // Iterate with iterators
    std::cout << "\nUsing iterators:" << std::endl;
    auto it = mesh.vertexBegin();
    for (int i = 0; i < 3 && it != mesh.vertexEnd(); ++it, ++i) {
        std::cout << "  Vertex " << it->index() 
                  << ": " << it->position() << std::endl;
    }
    
    /****** Face-vertex iteration ******/
    
    std::cout << "\n=== Face-Vertex Iteration ===" << std::endl;
    
    // Iterate over vertices of each face
    for (int count = 0; const auto& face : mesh.faces()) {
        if (count >= 3) break;
        std::cout << "Face " << face.index() << " vertices:" << std::endl;
        
        // Method 1: Using indices
        for (int i = 0; i < 3; ++i) {
            const auto& vertex = mesh.vertex(face.vertexIndex(i));
            std::cout << "  Vertex " << face.vertexIndex(i) 
                      << ": " << vertex.position() << std::endl;
        }
        
        // Method 2: Using pointers
        std::cout << "  Using pointers:" << std::endl;
        for (int i = 0; i < 3; ++i) {
            const auto* vertex = face.vertex(i);
            std::cout << "    Vertex " << vertex->index() 
                      << ": " << vertex->position() << std::endl;
        }
        
        std::cout << std::endl;
        count++;
    }
    
    /****** Adjacency-based iteration ******/
    
    std::cout << "\n=== Adjacency-Based Iteration ===" << std::endl;
    
    // Enable adjacency information
    mesh.enablePerVertexAdjacentFaces();
    vcl::updatePerVertexAdjacentFaces(mesh);
    
    // Iterate over faces adjacent to each vertex
    for (int count = 0; const auto& vertex : mesh.vertices()) {
        if (count >= 3) break;
        std::cout << "Vertex " << vertex.index() << " is adjacent to faces: ";
        for (const auto* face : vertex.adjFaces()) {
            if (face != nullptr) {
                std::cout << face->index() << " ";
            } else {
                std::cout << "null ";
            }
        }
        std::cout << std::endl;
        count++;
    }
    
    // Enable face-to-face adjacency
    mesh.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(mesh);
    
    std::cout << "\nFace adjacencies:" << std::endl;
    for (int count = 0; const auto& face : mesh.faces()) {
        if (count >= 3) break;
        std::cout << "Face " << face.index() << " is adjacent to faces: ";
        for (int i = 0; i < 3; ++i) {
            const auto* adjFace = face.adjFace(i);
            if (adjFace != nullptr) {
                std::cout << adjFace->index() << " ";
            } else {
                std::cout << "null ";
            }
        }
        std::cout << std::endl;
        count++;
    }
    
    /****** Using VCLib views ******/
    
    std::cout << "\n=== Using VCLib Views ===" << std::endl;
    
    // Iterate over vertex positions using views
    std::cout << "Vertex positions using views:" << std::endl;
    for (int count = 0; const auto& position : mesh | vcl::views::vertices | vcl::views::positions) {
        if (count >= 3) break;
        std::cout << "  Position " << count << ": " << position << std::endl;
        count++;
    }
    
    // Iterate over face vertices using views
    std::cout << "\nFace vertices using views:" << std::endl;
    for (int count = 0; const auto& face : mesh | vcl::views::faces) {
        if (count >= 2) break;
        std::cout << "Face " << face.index() << " vertex positions:" << std::endl;
        for (const auto& position : face | vcl::views::vertices | vcl::views::positions) {
            std::cout << "  " << position << std::endl;
        }
        count++;
    }
    
    // Using views with adjacent faces (filter out null adjacencies)
    std::cout << "\nValid adjacent faces using views:" << std::endl;
    for (int count = 0; const auto& face : mesh | vcl::views::faces) {
        if (count >= 2) break;
        std::cout << "Face " << face.index() << " valid adjacent faces: ";
        for (const auto* adjFace : face | vcl::views::adjFaces | vcl::views::notNull) {
            std::cout << adjFace->index() << " ";
        }
        std::cout << std::endl;
        count++;
    }
    
    /****** Polygon mesh iteration ******/
    
    std::cout << "\n=== Polygon Mesh Iteration ===" << std::endl;
    
    // Load a polygon mesh
    vcl::PolyMesh polyMesh = vcl::load<vcl::PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");
    
    std::cout << "Loaded polygon mesh with " << polyMesh.vertexNumber() 
              << " vertices and " << polyMesh.faceNumber() << " faces" << std::endl;
    
    // Iterate over polygon faces (variable number of vertices)
    for (const auto& face : polyMesh.faces()) {
        std::cout << "Polygon face " << face.index() 
                  << " has " << face.vertexNumber() << " vertices: ";
        for (vcl::uint i = 0; i < face.vertexNumber(); ++i) {
            std::cout << face.vertexIndex(i) << " ";
        }
        std::cout << std::endl;
        
        // Calculate polygon centroid
        vcl::Point3d centroid(0, 0, 0);
        for (vcl::uint i = 0; i < face.vertexNumber(); ++i) {
            centroid += face.vertex(i)->position();
        }
        centroid /= face.vertexNumber();
        std::cout << "  Centroid: " << centroid << std::endl;
    }
    
    /****** Computing mesh statistics through iteration ******/
    
    std::cout << "\n=== Mesh Statistics ===" << std::endl;
    
    // Compute bounding box
    vcl::Point3d minPoint = mesh.vertex(0).position();
    vcl::Point3d maxPoint = mesh.vertex(0).position();
    
    for (const auto& vertex : mesh.vertices()) {
        minPoint = vcl::min(minPoint, vertex.position());
        maxPoint = vcl::max(maxPoint, vertex.position());
    }
    
    std::cout << "Mesh bounding box: [" << minPoint << "] to [" << maxPoint << "]" << std::endl;
    
    // Compute average edge length
    double totalEdgeLength = 0.0;
    vcl::uint edgeCount = mesh.faceNumber() * 3;
    
    for (const auto& face : mesh.faces()) {
        for (int i = 0; i < 3; ++i) {
            const auto& v1 = face.vertex(i)->position();
            const auto& v2 = face.vertex((i + 1) % 3)->position();
            totalEdgeLength += (v2 - v1).norm();
        }
    }
    
    double avgEdgeLength = totalEdgeLength / edgeCount;
    std::cout << "Average edge length: " << avgEdgeLength << std::endl;
    
    /****** Performance considerations ******/
    
    std::cout << "\n=== Performance Notes ===" << std::endl;
    std::cout << "- Range-based for loops are generally the most readable" << std::endl;
    std::cout << "- Index-based access can be faster for random access patterns" << std::endl;
    std::cout << "- Views provide composable iteration patterns" << std::endl;
    
    return 0;
}
