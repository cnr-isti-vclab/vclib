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

print("=== Creating a Triangle Mesh ===")

mesh = vcl.TriMesh()

# Meshes can be named for easier identification
mesh.set_name("My Triangle Mesh")

# Add vertices to the mesh
mesh.add_vertex([0, 0, 0])
mesh.add_vertex([1, 0, 0])
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