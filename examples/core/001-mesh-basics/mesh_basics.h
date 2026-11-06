// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef MESH_BASICS_H
#define MESH_BASICS_H

#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates the basic concepts and operations with VCLib
// meshes:
// 1. Creating meshes from scratch
// 2. Adding vertices and faces
// 3. Accessing and modifying mesh elements
// 4. Working with optional components
// 5. Basic mesh information and iteration

auto meshBasics()
{
    std::println("=== VCLib Example 001: Mesh Basics ===\n");

    /****** Creating a mesh from scratch ******/

    std::println("=== Creating a Triangle Mesh ===");

    vcl::TriMesh mesh;

    // Meshes can be named for easier identification
    mesh.name() = "My Triangle Mesh";

    // Add vertices to the mesh
    mesh.addVertex(vcl::Point3d(0, 0, 0)); // vertex 0
    mesh.addVertex(vcl::Point3d(1, 0, 0)); // vertex 1
    mesh.addVertex(vcl::Point3d(0, 1, 0)); // vertex 2
    mesh.addVertex(vcl::Point3d(1, 1, 0)); // vertex 3

    std::println("Added {} vertices", mesh.vertexCount());

    // Add faces (triangles) using vertex indices
    mesh.addFace(0, 1, 2); // first triangle
    mesh.addFace(1, 3, 2); // second triangle

    std::println("Added {} faces", mesh.faceCount());

    /****** Accessing mesh elements ******/

    std::println("\n=== Accessing Mesh Elements ===");

    // Access vertices by index
    std::println("Vertex 0 position: {}", mesh.vertex(0).position());
    std::println("Vertex 1 position: {}", mesh.vertex(1).position());

    // Access faces and their vertices
    std::print("Face 0 vertices: ");
    for (int i = 0; i < 3; ++i) {
        std::print("{} ", mesh.face(0).vertex(i)->index());
        // or mesh.face(0).vertexIndex(i)
    }
    std::println();

    // Modify vertex positions
    mesh.vertex(2).position() = vcl::Point3d(0.5, 1.5, 0.2);
    std::println("Modified vertex 2 position: {}", mesh.vertex(2).position());

    /****** Iterating over mesh elements ******/

    std::println("\n=== Iterating Over Elements ===");

    // Iterate over all vertices
    std::println("All vertex positions:");
    for (const auto& vertex : mesh.vertices()) {
        std::println("  Vertex {}: {}", vertex.index(), vertex.position());
    }

    // Iterate over all faces
    std::println("All faces:");
    for (const auto& face : mesh.faces()) {
        std::print("  Face {}: vertices [", face.index());
        for (int i = 0; i < 3; ++i) {
            std::print("{}", face.vertexIndex(i));
            if (i < 2)
                std::print(", ");
        }
        std::println("]");
    }

    /****** Working with optional components ******/

    std::println("\n=== Optional Components ===");

    // Check if color component is available
    std::println(
        "Per-vertex color enabled: {}", mesh.isPerVertexColorEnabled());

    // Enable per-vertex color component
    mesh.enablePerVertexColor();
    std::println("Enabled per-vertex color");
    std::println(
        "Per-vertex color enabled: {}", mesh.isPerVertexColorEnabled());

    // Set vertex colors
    mesh.vertex(0).color() = vcl::Color::Red;
    mesh.vertex(1).color() = vcl::Color::Green;
    mesh.vertex(2).color() = vcl::Color::Blue;
    mesh.vertex(3).color() = vcl::Color::Yellow;

    std::println("Set colors for all vertices");

    // Enable and set face colors
    mesh.enablePerFaceColor();
    mesh.face(0).color() = vcl::Color::Cyan;
    mesh.face(1).color() = vcl::Color::Magenta;

    std::println("Set colors for all faces");

    /******** Mesh Components ******/

    // set the color associated with the mesh
    mesh.color() = vcl::Color::DarkYellow;

    /****** Polygon Mesh example ******/

    std::println("\n=== Creating a Polygon Mesh ===");

    vcl::PolyMesh polyMesh;

    // Polygon meshes can also be named
    polyMesh.name() = "My Polygon Mesh";

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
    face.pushVertex(1u);                  // or with the index of the vertex
    face.pushVertex(&polyMesh.vertex(2));
    face.pushVertex(&polyMesh.vertex(3));

    std::println("Created polygon with {} vertices", face.vertexCount());

    // Iterate over vertices of the polygon face
    std::print("Polygon vertices: ");
    for (vcl::uint i = 0; i < face.vertexCount(); ++i) {
        std::print("{} ", face.vertexIndex(i));
    }
    std::println();

    /****** Mesh statistics ******/

    std::println("\n=== Mesh Statistics ===");

    std::println("TriMesh:");
    std::println("  Vertices: {}", mesh.vertexCount());
    std::println("  Faces: {}", mesh.faceCount());
    std::println(
        "  Container sizes - Vertices: {}, Faces: {}",
        mesh.vertexContainerSize(),
        mesh.faceContainerSize());

    std::println("PolyMesh:");
    std::println("  Vertices: {}", polyMesh.vertexCount());
    std::println("  Faces: {}", polyMesh.faceCount());

    /****** Computing and displaying barycenter and bounding box ******/

    std::println("\n=== Barycenter and Bounding Box ===");

    // Compute barycenter manually
    vcl::Point3d barycenter(0, 0, 0);
    for (const auto& vertex : mesh.vertices()) {
        barycenter += vertex.position();
    }
    barycenter /= mesh.vertexCount();

    std::println("Mesh barycenter: {}", barycenter);

    // Find bounding box manually
    vcl::Point3d minPoint = mesh.vertex(0).position();
    vcl::Point3d maxPoint = mesh.vertex(0).position();

    for (const auto& vertex : mesh.vertices()) {
        const auto& pos = vertex.position();
        minPoint        = vcl::Point3d(
            std::min(minPoint.x(), pos.x()),
            std::min(minPoint.y(), pos.y()),
            std::min(minPoint.z(), pos.z()));

        // the library provides vcl::min and vcl::max functions which, for
        // Points, return the point containing the minimum or maximum value
        // between each component of the two input points.
        maxPoint = vcl::max(maxPoint, pos);
    }

    std::println("Bounding box: min={}, max={}", minPoint, maxPoint);

    return std::make_tuple(mesh, polyMesh);
}

#endif // MESH_BASICS_H
