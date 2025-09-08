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

# This example demonstrates how to:
# 1. Work with adjacency relationships in meshes
# 2. Navigate vertex-face, vertex-vertex, and face-face adjacencies
# 3. Explore mesh topology and connectivity

def mesh_topology():
    print(f"=== VCLib Example 006: Mesh Topology ===\n\n")

    # PART 1: LOADING AND BASIC INFO

    print(f"PART 1: Loading Mesh and Basic Information\n")
    print(f"-------------------------------------------\n")

    # Load a simple mesh for topology exploration
    mesh = vcl.load_tri_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/cube_tri.ply")
    print(f"Loaded triangular mesh: {mesh.vertex_number()} vertices, {mesh.face_number()} faces")
    print(f"Expected edges: ~{(mesh.face_number() * 3 / 2)} (Euler's formula approximation)\n")

    # PART 2: VERTEX-FACE ADJACENCY
    print(f"PART 2: Vertex-Face Adjacency\n")
    print(f"-----------------------------\n")

    # Enable and compute vertex-face adjacency
    mesh.enable_per_vertex_adjacent_faces()
    vcl.update_per_vertex_adjacent_faces(mesh)

    print(f"Vertex-face adjacency enabled and computed\n\n")

    # Show adjacency for first few vertices
    print(f"Adjacent faces for first 3 vertices:\n")
    for i in range(3):
        vertex = mesh.vertex(i)
        print(f"Vertex {i} at {vertex.position()} is adjacent to faces: ", end="")
        for face in vertex.adj_faces():
            print(f"{face.index()} ", end="")
        print(f"\n(Total: {vertex.adj_faces_number()} faces)\n")

    # Find vertex with most adjacent faces
    max_adj_faces = 0
    max_vertex = 0
    for v in mesh.vertices():
        if v.adj_faces_number() > max_adj_faces:
            max_adj_faces = v.adj_faces_number()
            max_vertex = v.index()
    print(f"Vertex with most adjacent faces: {max_vertex} ({max_adj_faces} faces)\n")

    # PART 3: VERTEX-VERTEX ADJACENCY
    print(f"PART 3: Vertex-Vertex Adjacency\n")
    print(f"-------------------------------\n")

    # Enable and compute vertex-vertex adjacency
    mesh.enable_per_vertex_adjacent_vertices()
    vcl.update_per_vertex_adjacent_vertices(mesh)

    print(f"Vertex-vertex adjacency enabled and computed\n\n")

    # Show adjacency for first few vertices
    print(f"Adjacent vertices for first 3 vertices:\n")
    for i in range(3):
        vertex = mesh.vertex(i)
        print(f"Vertex {i} at {vertex.position()} is adjacent to vertices: ", end="")
        for adj_vertex in vertex.adj_vertices():
            print(f"{adj_vertex.index()} ", end="")
        print(f"\n(Total: {vertex.adj_vertices_number()} vertices)\n")

    # Analyze vertex degrees (number of adjacent vertices)
    min_degree = float('inf')
    max_degree = 0
    avg_degree = 0.0
    for v in mesh.vertices():
        degree = v.adj_vertices_number()
        min_degree = min(min_degree, degree)
        max_degree = max(max_degree, degree)
        avg_degree += degree
    avg_degree /= mesh.vertex_number()
    print(f"Vertex degree statistics:\n")
    print(f"  Minimum degree: {min_degree}\n")
    print(f"  Maximum degree: {max_degree}\n")
    print(f"  Average degree: {avg_degree}\n")

    # PART 4: FACE-FACE ADJACENCY
    print(f"\n\nPART 4: Face-Face Adjacency\n")
    print(f"----------------------------\n")

    # Enable and compute face-face adjacency
    mesh.enable_per_face_adjacent_faces()
    vcl.update_per_face_adjacent_faces(mesh)

    print(f"Face-face adjacency enabled and computed\n\n")

    # Show adjacency for first few faces
    print(f"Adjacent faces for first 3 faces:\n")
    for i in range(3):
        face = mesh.face(i)
        print(f"Face {i} is adjacent to faces: ", end="")
        for adj_face in face.adj_faces():
            if adj_face != None:
                print(f"{adj_face.index()} ", end="")
            else:
                print("border ", end="")
        print(f"\n(Total: {face.adj_faces_number()} faces)\n")

    # Count border edges (faces with null adjacent faces)
    border_edges = 0
    for f in mesh.faces():
        for adj_face in f.adj_faces():
            if adj_face == None:
                border_edges += 1
    print(f"Border edges found: {border_edges}\n")

    # PART 5: MESH NAVIGATION
    print(f"\n\nPART 5: Mesh Navigation\n")
    print(f"-----------------------\n")

    # Navigate around a vertex using adjacency information
    if (mesh.vertex_number() > 0):
        start_vertex = 0
        vertex = mesh.vertex(start_vertex)

        print(f"Navigation example starting from vertex {start_vertex}:")

        print(f"  1-ring neighborhood (direct neighbors): ", end="")
        for adj_vertex in vertex.adj_vertices():
            print(f"{adj_vertex.index()} ", end="")
        print(f"\n")

        # 2-ring neighborhood (neighbors of neighbors)
        print(f"  2-ring neighborhood (neighbors of neighbors): ", end="")
        for adj_vertex in vertex.adj_vertices():
            for adj_adj_vertex in adj_vertex.adj_vertices():
                if adj_adj_vertex != vertex:
                    print(f"{adj_adj_vertex.index()} ", end="")
        print(f"\n")

        # Star of faces around the vertex
        print(f"  Star of faces around vertex {start_vertex}: ", end="")
        for face in vertex.adj_faces():
            print(f"{face.index()} ", end="")
        print(f"\n")


if __name__ == "__main__":
    mesh_topology()

    print("Example completed successfully!")
