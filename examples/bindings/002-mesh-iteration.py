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

# This example demonstrates various ways to iterate over mesh elements:
# 1. Basic iteration over vertices and faces
# 2. Iterating over adjacent elements (vertex-face, face-vertex relationships)
# 3. Working with polygon meshes and variable topology
# 4. Computing mesh statistics through iteration

def mesh_iteration():
    print("=== VCLib Example 002: Mesh Iteration ===\n")

    # /****** Load a test mesh ******/

    print("=== Loading Test Mesh ===")

    mesh = vcl.load_tri_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/cube_tri.ply")

    print(f"Loaded mesh with {mesh.vertex_number()} vertices and {mesh.face_number()} faces")

    # /****** Basic iteration ******/

    print("\n=== Basic Iteration ===")

    # Iterate with range-based for loop
    print("First 3 vertex positions:")
    count = 0
    for vertex in mesh.vertices():
        if count >= 3:
            break
        print(f"  Vertex {vertex.index()}: {vertex.position()}")
        count += 1

    # Iterate with traditional for loop using indices
    print("\nFirst 3 faces (using indices):")
    for i in range(min(3, mesh.face_number())):
        face = mesh.face(i)
        print(f"  Face {i}: vertices [", end="")
        for j in range(3):
            print(face.vertex(j).index(), end="")
            if j < 2:
                print(", ", end="")
        print("]")

    # /****** Face-vertex iteration ******/

    print("\n=== Face-Vertex Iteration ===")

    # Iterate over vertices of each face
    count = 0
    for face in mesh.faces():
        if count >= 3:
            break
        print(f"Face {face.index()} vertices:")

        for i in range(3):
            vertex = face.vertex(i)
            print(f"    Vertex {vertex.index()}: {vertex.position()}")

        print()
        count += 1

    # /****** Adjacency-based iteration ******/

    print("\n=== Adjacency-Based Iteration ===")

    # Enable adjacency information
    mesh.enable_per_vertex_adjacent_faces()
    vcl.update_per_vertex_adjacent_faces(mesh)

    # Iterate over faces adjacent to each vertex
    count = 0
    for vertex in mesh.vertices():
        if count >= 3:
            break
        print(f"Vertex {vertex.index()} is adjacent to faces: ", end="")
        for face in vertex.adj_faces():
            if face is not None:
                print(f"{face.index()} ", end="")
            else:
                print("null ", end="")
        print()
        count += 1

    # Enable face-to-face adjacency
    mesh.enable_per_face_adjacent_faces()
    vcl.update_per_face_adjacent_faces(mesh)

    print("\nFace adjacencies:")
    count = 0
    for face in mesh.faces():
        if count >= 3:
            break
        print(f"Face {face.index()} is adjacent to faces: ", end="")
        for i in range(3):
            adj_face = face.adj_face(i)
            if adj_face is not None:
                print(f"{adj_face.index()} ", end="")
            else:
                print("null ", end="")
        print()
        count += 1

    # /****** Polygon mesh iteration ******/

    print("\n=== Polygon Mesh Iteration ===")

    # Load a polygon mesh
    poly_mesh = vcl.load_poly_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/cube_poly.ply")

    print(f"Loaded polygon mesh with {poly_mesh.vertex_number()} vertices and {poly_mesh.face_number()} faces")

    # Iterate over polygon faces (variable number of vertices)
    for face in poly_mesh.faces():
        print(f"Polygon face {face.index()} has {face.vertex_number()} vertices: ", end="")
        for i in range(face.vertex_number()):
            print(f"{face.vertex(i).index()} ", end="")
        print()

        # Calculate polygon centroid
        centroid = vcl.Point3(0, 0, 0)
        for i in range(face.vertex_number()):
            centroid += face.vertex(i).position()
        centroid /= face.vertex_number()
        print(f"  Centroid: {centroid}")

    # /****** Computing mesh statistics through iteration ******/

    print("\n=== Mesh Statistics ===")

    # Compute bounding box
    if mesh.vertex_number() > 0:
        min_point = mesh.vertex(0).position()
        max_point = mesh.vertex(0).position()

        for vertex in mesh.vertices():
            min_point = vcl.min(min_point, vertex.position())
            max_point = vcl.max(max_point, vertex.position())

        print(f"Mesh bounding box: [{min_point}] to [{max_point}]")

        # Compute average edge length
        total_edge_length = 0.0
        edge_count = mesh.face_number() * 3

        for face in mesh.faces():
            for i in range(3):
                v1 = face.vertex(i).position()
                v2 = face.vertex((i + 1) % 3).position()
                total_edge_length += (v2 - v1).norm()

        avg_edge_length = total_edge_length / edge_count
        print(f"Average edge length: {avg_edge_length}")

    # /****** Performance considerations ******/

    print("\n=== Performance Notes ===")
    print("- Range-based for loops are generally the most readable")
    print("- Index-based access can be faster for random access patterns")
    print("- Python iteration is optimized for readability and ease of use")


if __name__ == "__main__":
    mesh_iteration()
