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
# 1. Calculate face and vertex normals
# 2. Use different weighting algorithms for vertex normals
# 3. Manipulate normals with transformations

def mesh_normals():
    print(f"=== VCLib Example 005: Mesh Normals ===\n\n")

    mesh = vcl.load_tri_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/bimba_simplified.obj")
    print(f"Loaded mesh with {mesh.vertex_number()} vertices and {mesh.face_number()} faces.")

    # PART 1: FACE NORMALS
    print("\n\nPART 1: Face Normals")
    print("------------------")

    # Calculate face normals
    vcl.update_per_face_normals(mesh)
    print("Face normals calculated.")

    # Show normals for first few faces
    print(f"First 3 face normals:\n")
    for i in range(3):
        normal = mesh.face(i).normal()
        print(f"Face {i}: {normal}")

    # PART 2: VERTEX NORMALS - DIFFERENT ALGORITHMS
    print("\n\nPART 2: Vertex Normals - Different Algorithms")
    print("----------------------------------------------")

    # 2.1 Basic vertex normals (area weighted)
    print("\n2.1 Basic Vertex Normals (area weighted):")
    vcl.update_per_vertex_normals(mesh)
    print("Basic vertex normals calculated.")

    # Show normal for first vertex
    # Store this result for comparison
    basic_normal = mesh.vertex(0).normal()
    print(f"First vertex normal: {basic_normal}")

    # 2.2 Vertex normals from face normals (uniform weight)
    print("\n2.2 Vertex Normals from Face Normals (uniform weight):")
    vcl.update_per_vertex_normals_from_face_normals(mesh)
    uniform_normal = mesh.vertex(0).normal()
    print(f"First vertex normal: {uniform_normal}")

    # 2.3 Angle weighted vertex normals
    print("\n2.3 Angle Weighted Vertex Normals:")
    vcl.update_per_vertex_normals_angle_weighted(mesh)
    angle_normal = mesh.vertex(0).normal()
    print(f"First vertex normal: {angle_normal}")

    # 2.4 Nelson-Max weighted vertex normals
    print("\n2.4 Nelson-Max Weighted Vertex Normals:")
    vcl.update_per_vertex_normals_nelson_max_weighted(mesh)
    nelson_max_normal = mesh.vertex(0).normal()
    print(f"First vertex normal: {nelson_max_normal}")

    # Compare the different methods
    print("\nComparison of different weighting methods for vertex 0:")
    print(f"  Basic (area):     {basic_normal.norm()} (magnitude)")
    print(f"  Uniform:          {uniform_normal.norm()} (magnitude)")
    print(f"  Angle weighted:   {angle_normal.norm()} (magnitude)")
    print(f"  Nelson-Max:       {nelson_max_normal.norm()} (magnitude)")

    # PART 3: NORMAL MANIPULATION

    print(f"\nPART 3: Normal Manipulation\n")
    print(f"----------------------------\n")

    # Create a transformation matrix to flip normals
    print(f"3.1 Flipping Normals:\n")

    flip_matrix = np.eye(4)
    # flip X, Y, Z components
    flip_matrix[0, 0] = -1
    flip_matrix[1, 1] = -1
    flip_matrix[2, 2] = -1

    # Store original normal for comparison
    original_face_normal = mesh.face(0).normal()
    print(f"Original face 0 normal: {original_face_normal}")

    # Apply transformation to face normals
    vcl.multiply_per_face_normals_by_matrix(mesh, flip_matrix)

    flipped_face_normal = mesh.face(0).normal()
    print(f"Flipped face 0 normal: {flipped_face_normal}")

    # Apply transformation to vertex normals
    original_vertex_normal = mesh.vertex(0).normal()
    print(f"Original vertex 0 normal: {original_vertex_normal}")

    vcl.multiply_per_vertex_normals_by_matrix(mesh, flip_matrix)

    flipped_vertex_normal = mesh.vertex(0).normal()
    print(f"Flipped vertex 0 normal: {flipped_vertex_normal}")

    # PART 4: WORKING WITH DIFFERENT MESH TYPES
    print(f"\nPART 4: Working with Different Mesh Types\n")
    print(f"------------------------------------------\n")

    # Load and process a polygon mesh
    poly_mesh = vcl.PolyMesh()
    vcl.load_mesh(poly_mesh, VCLIB_EXAMPLE_MESHES_PATH + "/cube_poly.ply")

    print(f"Loaded polygon mesh with {poly_mesh.vertex_number()} vertices and {poly_mesh.face_number()} faces.")

    # Calculate normals for polygon mesh
    vcl.update_per_face_normals(poly_mesh)
    vcl.update_per_vertex_normals(poly_mesh)

    print("Calculated normals for polygon mesh.")
    print(f"First polygon face normal: {poly_mesh.face(0).normal()}")
    print(f"First polygon vertex normal: {poly_mesh.vertex(0).normal()}")

    # Restore proper normals before returning
    vcl.update_per_face_normals(mesh)
    vcl.update_per_vertex_normals(mesh)

    return mesh, poly_mesh


if __name__ == "__main__":
    mesh, poly_mesh = mesh_normals()

    try:
        # Save the meshes to files
        vcl.save_mesh(mesh, VCLIB_RESULTS_PATH + "/005_cube_with_normals.ply")
        vcl.save_mesh(poly_mesh, VCLIB_RESULTS_PATH + "/005_poly_cube_with_normals.ply")
        print("Meshes saved successfully.")
    except Exception as e:
        print(f"Error saving meshes: {e}")

    print("Example completed successfully!")