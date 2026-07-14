# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl
import os
import sys
import time

from paths import VCLIB_EXAMPLE_MESHES_PATH, VCLIB_PYTHON_RESULTS_PATH

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
    print(f"  Vertices: {original_mesh.vertex_count()}")
    print(f"  Faces: {original_mesh.face_count()}")

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

    print("Taubin smoothing (50 iterations, lambda=0.5, mu=-0.53) completed.")

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
    print(f"  Vertices: {original_mesh.vertex_count()}")
    print(f"  Faces: {original_mesh.face_count()}")

    return original_mesh, laplacian_mesh, cotangent_mesh, taubin_mesh, normal_mesh


def main():
    try:
        original_mesh, laplacian_mesh, cotangent_mesh, taubin_mesh, normal_mesh = mesh_smoothing()

        # Save the smoothed meshes
        print("\n=== Saving Meshes ===")

        # Create results directory if it doesn't exist
        os.makedirs(VCLIB_PYTHON_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(laplacian_mesh, f"{VCLIB_PYTHON_RESULTS_PATH}/014_laplacian_smoothed.ply")
        vcl.save_mesh(cotangent_mesh, f"{VCLIB_PYTHON_RESULTS_PATH}/014_cotangent_smoothed.ply")
        vcl.save_mesh(taubin_mesh, f"{VCLIB_PYTHON_RESULTS_PATH}/014_taubin_smoothed.ply")
        vcl.save_mesh(normal_mesh, f"{VCLIB_PYTHON_RESULTS_PATH}/014_normal_smoothed.ply")

        print(f"\nAll files have been saved to: {VCLIB_PYTHON_RESULTS_PATH}")

    except Exception as e:
        print(f"Error in saving: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
