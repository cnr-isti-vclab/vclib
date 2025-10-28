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
import time

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = current_file_path + "/../../assets/example_meshes"
VCLIB_RESULTS_PATH = current_file_path + "/../../assets/results/python"

# This example demonstrates mesh smoothing algorithms:
# 1. Laplacian smoothing with uniform weights
# 2. Laplacian smoothing with cotangent weights
# 3. Taubin smoothing (low-pass filter)
# 4. Per-vertex normal smoothing for point clouds


def mesh_smoothing():
    print("=== VCLib Example 014: Mesh Smoothing ===\n")

    # Load a mesh for smoothing
    print("=== Loading Bunny Mesh ===")

    original_mesh = vcl.TriMesh()
    vcl.load_mesh(original_mesh, f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny.obj")
    original_mesh.set_name("Original Bunny Mesh")

    print("Original mesh loaded:")
    print(f"  Vertices: {original_mesh.vertex_number()}")
    print(f"  Faces: {original_mesh.face_number()}")

    vcl.update_per_vertex_and_face_normals(original_mesh)

    # Laplacian Smoothing with Uniform Weights
    print("\n=== Laplacian Smoothing (Uniform Weights) ===")

    laplacian_mesh = vcl.TriMesh(original_mesh)
    laplacian_mesh.set_name("Laplacian Smoothed (Uniform)")

    start_time = time.time()
    vcl.laplacian_smoothing(laplacian_mesh, 10, False, False)
    elapsed_time = time.time() - start_time
    print(f"Laplacian smoothing (uniform) took: {elapsed_time*1000:.2f} ms")

    print("Laplacian smoothing (10 iterations) completed.")

    vcl.update_per_vertex_and_face_normals(laplacian_mesh)

    # Laplacian Smoothing with Cotangent Weights
    print("\n=== Laplacian Smoothing (Cotangent Weights) ===")

    cotangent_mesh = vcl.TriMesh(original_mesh)
    cotangent_mesh.set_name("Laplacian Smoothed (Cotangent)")

    start_time = time.time()
    vcl.laplacian_smoothing(cotangent_mesh, 10, False, True)
    elapsed_time = time.time() - start_time
    print(f"Laplacian smoothing (cotangent) took: {elapsed_time*1000:.2f} ms")

    print("Cotangent Laplacian smoothing (10 iterations) completed.")

    vcl.update_per_vertex_and_face_normals(cotangent_mesh)

    # Taubin Smoothing
    print("\n=== Taubin Smoothing ===")

    taubin_mesh = vcl.TriMesh(original_mesh)
    taubin_mesh.set_name("Taubin Smoothed")

    start_time = time.time()
    vcl.taubin_smoothing(taubin_mesh, 50, 0.5, -0.53, False)
    elapsed_time = time.time() - start_time
    print(f"Taubin smoothing took: {elapsed_time*1000:.2f} ms")

    print("Taubin smoothing (50 iterations, λ=0.5, μ=-0.53) completed.")

    vcl.update_per_vertex_and_face_normals(taubin_mesh)

    # Normal Smoothing for Point Cloud
    print("\n=== Normal Smoothing ===")

    normal_mesh = vcl.TriMesh(original_mesh)
    normal_mesh.set_name("Normal Smoothed")

    start_time = time.time()
    vcl.smooth_per_vertex_normals_point_cloud(normal_mesh, 10, 3)
    elapsed_time = time.time() - start_time
    print(f"Normal smoothing took: {elapsed_time*1000:.2f} ms")

    print("Per-vertex normal smoothing (10 neighbors, 3 iterations) completed.")

    # Display mesh statistics
    print("\n=== Mesh Statistics ===")
    print("All meshes have:")
    print(f"  Vertices: {original_mesh.vertex_number()}")
    print(f"  Faces: {original_mesh.face_number()}")

    return original_mesh, laplacian_mesh, cotangent_mesh, taubin_mesh, normal_mesh


def main():
    try:
        original_mesh, laplacian_mesh, cotangent_mesh, taubin_mesh, normal_mesh = mesh_smoothing()

        # Save the smoothed meshes
        print("\n=== Saving Meshes ===")

        # Create results directory if it doesn't exist
        os.makedirs(VCLIB_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(laplacian_mesh, f"{VCLIB_RESULTS_PATH}/013_laplacian_smoothed.ply")
        vcl.save_mesh(cotangent_mesh, f"{VCLIB_RESULTS_PATH}/013_cotangent_smoothed.ply")
        vcl.save_mesh(taubin_mesh, f"{VCLIB_RESULTS_PATH}/013_taubin_smoothed.ply")
        vcl.save_mesh(normal_mesh, f"{VCLIB_RESULTS_PATH}/013_normal_smoothed.ply")

        print(f"\nAll files have been saved to: {VCLIB_RESULTS_PATH}")

    except Exception as e:
        print(f"Error in saving: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
