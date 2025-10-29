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
import sys

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = current_file_path + "/../../assets/example_meshes"
VCLIB_RESULTS_PATH = current_file_path + "/../../assets/results/python"

# This example demonstrates mesh cleaning and repairing algorithms:
# 1. Removing unreferenced vertices
# 2. Removing duplicated vertices
# 3. Removing duplicated faces
# 4. Removing degenerate vertices and faces
# 5. Checking mesh topology properties
# 6. Connected components analysis


def mesh_clean():
    print("=== VCLib Example 013: Mesh Cleaning ===\n")

    # Load a mesh and perform basic cleaning
    print("=== Loading and Cleaning Brain Mesh ===")

    original_mesh = vcl.TriMesh()
    vcl.load_mesh(original_mesh, f"{VCLIB_EXAMPLE_MESHES_PATH}/brain.ply")
    original_mesh.set_name("Original Brain Mesh")

    mesh = vcl.TriMesh(original_mesh)
    mesh.set_name("Cleaned Brain Mesh")

    print(f"Original mesh: {mesh.vertex_number()} vertices, "
          f"{mesh.face_number()} faces")

    # Remove unreferenced vertices
    removed_unref = vcl.remove_unreferenced_vertices(mesh)
    print(f"Removed {removed_unref} unreferenced vertices")

    # Remove duplicated vertices
    removed_dup_verts = vcl.remove_duplicate_vertices(mesh)
    print(f"Removed {removed_dup_verts} duplicate vertices")

    # Remove duplicated faces
    removed_dup_faces = vcl.remove_duplicate_faces(mesh)
    print(f"Removed {removed_dup_faces} duplicate faces")

    # Remove degenerate faces
    removed_deg_faces = vcl.remove_degenerate_faces(mesh)
    print(f"Removed {removed_deg_faces} degenerate faces")

    # Compact the mesh to remove deleted elements
    mesh.compact()

    print(f"Cleaned mesh: {mesh.vertex_number()} vertices, "
          f"{mesh.face_number()} faces\n")

    # Removing degenerate vertices with NaN positions
    print("\n=== Handling Degenerate Vertices ===")

    # Create a test mesh with some problematic vertices
    test_mesh = vcl.TriMesh()
    test_mesh.set_name("Test Mesh")

    # Add normal vertices
    v0 = test_mesh.add_vertex(vcl.Point3(0, 0, 0))
    v1 = test_mesh.add_vertex(vcl.Point3(1, 0, 0))
    v2 = test_mesh.add_vertex(vcl.Point3(0, 1, 0))

    # Add a vertex with NaN position
    v3 = test_mesh.add_vertex(vcl.Point3(float('nan'), 0, 0))

    # Add faces
    test_mesh.add_face([v0, v1, v2])
    test_mesh.add_face([v0, v1, v3])  # This face will be affected

    print(f"Test mesh before cleaning: {test_mesh.vertex_number()} vertices, "
          f"{test_mesh.face_number()} faces")

    # Remove degenerate vertices and associated faces
    removed_deg_verts = vcl.remove_degenerate_vertices(test_mesh, True)
    print(f"Removed {removed_deg_verts} degenerate vertices")

    test_mesh.compact()
    print(f"Test mesh after cleaning: {test_mesh.vertex_number()} vertices, "
          f"{test_mesh.face_number()} faces")

    print("\nExample completed successfully!")

    return original_mesh, mesh, test_mesh


def main():
    try:
        original_mesh, mesh, test_mesh = mesh_clean()

        # Save the created meshes
        print("\n=== Saving Meshes ===")

        # Create results directory if it doesn't exist
        os.makedirs(VCLIB_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(mesh, f"{VCLIB_RESULTS_PATH}/013_cleaned_brain.ply")
        vcl.save_mesh(test_mesh, f"{VCLIB_RESULTS_PATH}/013_test_clean.ply")

        print(f"\nAll files have been saved to: {VCLIB_RESULTS_PATH}")

    except Exception as e:
        print(f"Error in main: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
