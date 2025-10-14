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

# This example demonstrates how to export meshes to matrices and import
# matrices back to meshes, including:
# 1. Basic vertex positions and face indices
# 2. Additional mesh components (normals, colors)
# 3. Working with numpy arrays
# 4. Creating meshes from matrices


def random_color():
    """Generate random color using numpy random"""
    return vcl.Color(
        np.random.randint(0, 256),
        np.random.randint(0, 256), 
        np.random.randint(0, 256),
        255
    )


def mesh_matrix_conversion():
    print("=== VCLib Example 010: Mesh Matrix Conversion ===\n")

    # ****** Export: Mesh to Matrix ******
    
    print("=== Export: Mesh to Matrix ===")

    # Load a textured mesh with normals and colors
    mesh = vcl.load_tri_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/bunny_textured.ply")
    vcl.update_per_vertex_normals(mesh)

    # Enable and set some colors for demonstration
    mesh.enable_per_vertex_color()
    mesh.enable_per_face_color()
    for v in mesh.vertices():
        v.set_color(random_color())
    for f in mesh.faces():
        f.set_color(random_color())

    print(f"Loaded mesh: {mesh.vertex_number()} vertices, {mesh.face_number()} faces")

    # Export basic geometry
    vertices = vcl.vertex_positions_matrix(mesh)
    faces = vcl.face_vertex_indices_matrix(mesh)

    # Export additional components
    vertex_normals = vcl.vertex_normals_matrix(mesh)
    vertex_colors = vcl.vertex_colors_matrix(mesh)
    face_colors = vcl.face_colors_matrix(mesh)

    print("Exported matrices:")
    print(f"- Vertices: {vertices.shape[0]}x{vertices.shape[1]}")
    print(f"- Faces: {faces.shape[0]}x{faces.shape[1]}")
    print(f"- Vertex normals: {vertex_normals.shape[0]}x{vertex_normals.shape[1]}")
    print(f"- Vertex colors: {vertex_colors.shape[0]}x{vertex_colors.shape[1]}")
    print(f"- Face colors: {face_colors.shape[0]}x{face_colors.shape[1]}")

    # ****** Import: Matrix to Mesh ******

    print("\n=== Import: Matrix to Mesh ===")

    # Create a new mesh from the exported matrices
    imported_mesh = vcl.tri_mesh_from_matrices(vertices, faces)
    imported_mesh.set_name("Imported Mesh")

    # Import additional components separately
    vcl.vertex_normals_from_matrix(imported_mesh, vertex_normals)
    vcl.vertex_colors_from_matrix(imported_mesh, vertex_colors)
    vcl.face_colors_from_matrix(imported_mesh, face_colors)

    print(f"Imported mesh: {imported_mesh.vertex_number()} vertices, {imported_mesh.face_number()} faces")

    # Verify data integrity
    print("Data verification:")
    print(f"- First vertex position: {imported_mesh.vertex(0).position()}")
    print(f"- First vertex normal: {imported_mesh.vertex(0).normal()}")
    print(f"- First vertex color: {imported_mesh.vertex(0).color()}")

    # ****** Working with polygon meshes ******

    print("\n=== Working with Polygon Meshes ===")

    # Load a polygon mesh
    poly_mesh = vcl.load_poly_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/cube_poly.ply")

    poly_vertices = vcl.vertex_positions_matrix(poly_mesh)
    poly_faces = vcl.face_vertex_indices_matrix(poly_mesh)

    print("Polygon mesh:")
    print(f"- Vertices: {poly_vertices.shape[0]}x{poly_vertices.shape[1]}")
    print(f"- Faces: {poly_faces.shape[0]}x{poly_faces.shape[1]}")

    # ****** Creating meshes from scratch with matrices ******

    print("\n=== Creating Mesh from Scratch ===")

    # Create simple cube data as numpy arrays (faces in counterclockwise order)
    cube_vertices = np.array([
        [-1, -1, -1],  # 0
        [ 1, -1, -1],  # 1
        [ 1,  1, -1],  # 2
        [-1,  1, -1],  # 3
        [-1, -1,  1],  # 4
        [ 1, -1,  1],  # 5
        [ 1,  1,  1],  # 6
        [-1,  1,  1]   # 7
    ], dtype=np.float64)

    cube_quads = np.array([
        [0, 1, 2, 3],  # bottom (z = -1)
        [4, 7, 6, 5],  # top    (z = +1)
        [0, 4, 5, 1],  # front  (y = -1)
        [2, 6, 7, 3],  # back   (y = +1)
        [0, 3, 7, 4],  # left   (x = -1)
        [1, 5, 6, 2]   # right  (x = +1)
    ], dtype=np.int32)

    try:
        # Create tri mesh from polygonal matrices is not allowed
        fail_mesh = vcl.tri_mesh_from_matrices(cube_vertices, cube_quads)
        # You should first call mesh_from_matrices with PolyMesh, then
        # convert to TriMesh if needed using tri_mesh.import_from(poly_mesh)
    except Exception as e:
        print(f"Error creating tri mesh: {e}\n")

    # Create triangular faces for the cube
    cube_triangles = np.array([
        [0, 2, 1], [0, 3, 2],  # bottom (z = -1)
        [4, 5, 6], [4, 6, 7],  # top    (z = +1)
        [0, 1, 5], [0, 5, 4],  # front  (y = -1)
        [2, 3, 7], [2, 7, 6],  # back   (y = +1)
        [0, 4, 7], [0, 7, 3],  # left   (x = -1)
        [1, 2, 6], [1, 6, 5]   # right  (x = +1)
    ], dtype=np.int32)

    # Create mesh from these matrices
    cube_mesh = vcl.tri_mesh_from_matrices(cube_vertices, cube_triangles)
    cube_mesh.set_name("Cube Mesh")

    print(f"Created cube mesh: {cube_mesh.vertex_number()} vertices, {cube_mesh.face_number()} faces")

    vcl.update_per_vertex_and_face_normals(cube_mesh)

    return imported_mesh, cube_mesh


def save_results(imported_mesh, cube_mesh):
    """Save the resulting meshes"""
    # Create results directory if it doesn't exist
    os.makedirs(VCLIB_RESULTS_PATH, exist_ok=True)

    # Save the imported mesh
    vcl.save_mesh(imported_mesh, VCLIB_RESULTS_PATH + "/010_imported_mesh.ply")
    print(f"Saved imported mesh to: {VCLIB_RESULTS_PATH}/010_imported_mesh.ply")

    # Save the created cube mesh
    vcl.save_mesh(cube_mesh, VCLIB_RESULTS_PATH + "/010_created_cube.ply")
    print(f"Saved created cube to: {VCLIB_RESULTS_PATH}/010_created_cube.ply")


if __name__ == "__main__":
    # Run mesh matrix conversion
    imported_mesh, cube_mesh = mesh_matrix_conversion()

    # Save results
    save_results(imported_mesh, cube_mesh)
