#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the Mozilla Public License Version 2.0 as published *
#* by the Mozilla Foundation; either version 2 of the License, or            *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* Mozilla Public License Version 2.0                                        *
#* (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
#****************************************************************************/

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl
import numpy as np
import os

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = current_file_path + "/../../assets/example_meshes"
VCLIB_RESULTS_PATH = current_file_path + "/../../assets/results/python"

# This example demonstrates the basic concepts and operations with VCLib
# meshes:
# 1. Creating meshes from scratch
# 2. Adding vertices and faces
# 3. Accessing and modifying mesh elements
# 4. Working with optional components
# 5. Basic mesh information and iteration

# /****** Create a mesh ******/

def mesh_basics():
    print("=== Creating a Triangle Mesh ===")

    mesh = vcl.TriMesh()

    # Meshes can be named for easier identification
    mesh.set_name("My Triangle Mesh")

    # Add vertices to the mesh
    mesh.add_vertex([0, 0, 0])
    mesh.add_vertex(np.array([1., 0., 0.])) # example using np array (when using numpy, numbers must be floats)
    mesh.add_vertex([0, 1, 0])
    mesh.add_vertex([1, 1, 0])

    print(f"Added {mesh.vertex_number()} vertices")

    # Add faces (triangles) using vertex indices
    mesh.add_face([0, 1, 2])  # first triangle
    mesh.add_face([1, 3, 2])  # second triangle

    print(f"Added {mesh.face_number()} faces")

    # /****** Accessing mesh elements ******/

    print("=== Accessing Mesh Elements ===")

    # Access vertices by index

    print (f"Vertex 0: {mesh.vertex(0).position()}")
    print (f"Vertex 1: {mesh.vertex(1).position()}")

    # Access faces and their vertices
    print(f"Face 0 vertices:", end=" ")
    for i in range(0, 3):
        print(mesh.face(0).vertex(i).index(), end=" ")
    print()

    # Modify vertex position
    mesh.vertex(2).set_position([0.5, 1.5, 0.2])
    print(f"Modified Vertex 2 position: {mesh.vertex(2).position()}")

    # /****** Iterating over mesh elements ******/

    print("\n=== Iterating Over Elements ===")

    # Iterate over all vertices
    print("All vertex positions:")
    for vertex in mesh.vertices():
        print(f"  Vertex {vertex.index()}: {vertex.position()}")

    # Iterate over all faces
    print("All faces:")
    for face in mesh.faces():
        print(f"  Face {face.index()}: vertices [", end="")
        for i in range(0, 3):
            print(face.vertex(i).index(), end="")
            if i < 2:
                print(", ", end="")
        print("]")

    # /****** Working with optional components ******/

    print("\n=== Optional Components ===")

    # Check if color component is available
    print(f"Per-vertex color enabled: {mesh.is_per_vertex_color_enabled()}")

    # Enable per-vertex color component
    mesh.enable_per_vertex_color()
    print(f"Enabled per-vertex color")
    print(f"Per-vertex color enabled: {mesh.is_per_vertex_color_enabled()}")

    # Set vertex colors
    mesh.vertex(0).set_color(vcl.Color.RED)
    mesh.vertex(1).set_color(vcl.Color.GREEN)
    mesh.vertex(2).set_color(vcl.Color.BLUE)
    mesh.vertex(3).set_color(vcl.Color.YELLOW)

    print(f"Set colors for all vertices")

    # Enable and set face colors
    mesh.enable_per_face_color()
    mesh.face(0).set_color(vcl.Color.CYAN)
    mesh.face(1).set_color(vcl.Color.MAGENTA)

    print(f"Set colors for all faces")

    # /****** Polygon Mesh example ******/

    print("=== Creating a Polygon Mesh ===")

    poly_mesh = vcl.PolyMesh()

    # Polygon meshes can also be named
    poly_mesh.set_name("My Polygon Mesh")

    # Add vertices for a square
    poly_mesh.add_vertex([0, 0, 0])
    poly_mesh.add_vertex([2, 0, 0])
    poly_mesh.add_vertex([2, 2, 0])
    poly_mesh.add_vertex([0, 2, 0])

    # Add a quadrilateral face
    # Polygon faces can have variable number of vertices

    # easy way:
    # poly_mesh.add_face([0, 1, 2, 3])  # single quad face

    # or:
    poly_mesh.add_face()
    face = poly_mesh.face(0)
    face.push_vertex(poly_mesh.vertex(0)) # with the pointer of the vertex
    face.push_vertex(1) # or with the index of the vertex
    face.push_vertex(poly_mesh.vertex(2))
    face.push_vertex(poly_mesh.vertex(3))

    print(f"Created polygon with {face.vertex_number()} vertices")

    # Iterate over vertices of the polygon face
    print("Polygon vertices:", end=" ")
    for i in range(face.vertex_number()):
        print(f"{face.vertex(i).index()}", end=" ")
    print()

    # /****** Mesh statistics ******/

    print("\n=== Mesh Statistics ===")

    print("TriMesh:")
    print(f"  Vertices: {mesh.vertex_number()}")
    print(f"  Faces: {mesh.face_number()}")
    print(f"  Container sizes - Vertices: {mesh.vertex_container_size()}, Faces: {mesh.face_container_size()}")

    print("PolyMesh:")
    print(f"  Vertices: {poly_mesh.vertex_number()}")
    print(f"  Faces: {poly_mesh.face_number()}")

    # /****** Computing and displaying barycenter and bounding box ******/

    print("\n=== Barycenter and Bounding Box ===")

    # Compute barycenter manually
    barycenter = vcl.Point3(0, 0, 0)
    for vertex in mesh.vertices():
        barycenter += vertex.position()
    barycenter /= mesh.vertex_number()

    print(f"Mesh barycenter: {barycenter}")

    # Find bounding box manually
    min_point = vcl.Point3(float("inf"), float("inf"), float("inf"))
    max_point = vcl.Point3(float("-inf"), float("-inf"), float("-inf"))
    for vertex in mesh.vertices():
        min_point = vcl.min(min_point, vertex.position())
        max_point = vcl.max(max_point, vertex.position())

    print(f"Bounding Box: Min {min_point}, Max {max_point}")

    return mesh, poly_mesh


if __name__ == "__main__":
    mesh, poly_mesh = mesh_basics()

    try:
        # Save the meshes to files
        vcl.save_mesh(mesh, VCLIB_RESULTS_PATH + "/001_mesh-basics_triangle.ply")
        vcl.save_mesh(poly_mesh, VCLIB_RESULTS_PATH + "/001_mesh-basics_polygon.ply")
        print("Meshes saved successfully.")
    except Exception as e:
        print(f"Error saving meshes: {e}")
        
    print("Example completed successfully!")