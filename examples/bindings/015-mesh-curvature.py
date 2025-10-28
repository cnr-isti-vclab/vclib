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

# This example demonstrates mesh curvature computation algorithms:
# 1. Principal curvature using Taubin95 algorithm
# 2. Principal curvature using PCA method
# 3. Color mapping based on mean and Gaussian curvature


def mesh_curvature():
    print("=== VCLib Example 015: Mesh Curvature ===\n")

    # Load a mesh for curvature computation
    print("=== Loading Bunny Mesh ===")

    original_mesh = vcl.TriMesh()
    vcl.load_mesh(original_mesh, f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny.obj")
    original_mesh.set_name("Original Bunny Mesh")

    print("Original mesh loaded:")
    print(f"  Vertices: {original_mesh.vertex_number()}")
    print(f"  Faces: {original_mesh.face_number()}")

    # Enable required components
    original_mesh.enable_per_vertex_adjacent_faces()
    original_mesh.enable_per_face_adjacent_faces()
    original_mesh.enable_per_vertex_principal_curvature()
    original_mesh.enable_per_vertex_color()
    original_mesh.enable_per_vertex_quality()

    vcl.update_per_vertex_and_face_normals(original_mesh)
    vcl.update_per_face_adjacent_faces(original_mesh)
    vcl.update_per_vertex_adjacent_faces(original_mesh)

    # Principal Curvature with Taubin95 Algorithm
    print("\n=== Principal Curvature (Taubin95) ===")

    taubin_mesh = vcl.TriMesh(original_mesh)
    taubin_mesh.set_name("Taubin95 Curvature")

    start_time = time.time()
    vcl.update_per_vertex_principal_curvature(taubin_mesh, algorithm=vcl.PrincipalCurvatureAlgorithm.TAUBIN95)
    elapsed_time = time.time() - start_time
    print(f"Taubin95 principal curvature took: {elapsed_time*1000:.2f} ms")

    # Color mapping based on mean curvature
    vcl.set_per_vertex_quality_from_principal_curvature_mean(taubin_mesh)
    h = vcl.vertex_quality_histogram(taubin_mesh)

    vcl.set_per_vertex_color_from_quality(
        taubin_mesh,
        vcl.Color.ColorMap.RED_BLUE,
        h.value_at_percentile(0.1),
        h.value_at_percentile(0.9))

    print("Taubin95 curvature computed.")
    print(f"  Mean curvature range: {h.min_range_value()} to {h.max_range_value()}")

    # Principal Curvature with PCA Method
    print("\n=== Principal Curvature (PCA) ===")

    pca_mesh = vcl.TriMesh(original_mesh)
    pca_mesh.set_name("PCA Curvature")

    radius = vcl.bounding_box(pca_mesh).diagonal() * 0.05

    start_time = time.time()
    vcl.update_per_vertex_principal_curvature(pca_mesh, algorithm=vcl.PrincipalCurvatureAlgorithm.PCA, radius=radius)
    elapsed_time = time.time() - start_time
    print(f"PCA principal curvature took: {elapsed_time*1000:.2f} ms")

    # Color mapping based on Gaussian curvature
    vcl.set_per_vertex_quality_from_principal_curvature_gaussian(pca_mesh)
    h2 = vcl.vertex_quality_histogram(pca_mesh)

    vcl.set_per_vertex_color_from_quality(
        pca_mesh,
        vcl.Color.ColorMap.RED_BLUE,
        h2.value_at_percentile(0.05),
        h2.value_at_percentile(0.95))

    print(f"PCA curvature computed (radius: {radius}).")
    print(f"  Gaussian curvature range: {h2.min_range_value()} to {h2.max_range_value()}")

    # General Principal Curvature Function
    print("\n=== Principal Curvature (General Function) ===")

    general_mesh = vcl.TriMesh(original_mesh)
    general_mesh.set_name("General Curvature")

    start_time = time.time()
    vcl.update_per_vertex_principal_curvature(general_mesh, vcl.PrincipalCurvatureAlgorithm.TAUBIN95)
    elapsed_time = time.time() - start_time
    print(f"General principal curvature took: {elapsed_time*1000:.2f} ms")

    # Color mapping with maximum curvature
    for v in general_mesh.vertices():
        v.set_quality(max(
            abs(v.principal_curvature().max_value()),
            abs(v.principal_curvature().min_value())))

    h3 = vcl.vertex_quality_histogram(general_mesh)

    vcl.set_per_vertex_color_from_quality(
        general_mesh,
        vcl.Color.ColorMap.RED_BLUE,
        h3.value_at_percentile(0.1),
        h3.value_at_percentile(0.9))

    print("General curvature computed.")
    print(f"  Maximum curvature range: {h3.min_range_value()} to {h3.max_range_value()}")

    # Display statistics
    print("\n=== Curvature Statistics ===")

    # Show curvature info for first vertex
    v = taubin_mesh.vertex(0)
    print("Sample vertex curvature (Taubin95):")
    print(f"  Max value (k1): {v.principal_curvature().max_value()}")
    print(f"  Min value (k2): {v.principal_curvature().min_value()}")
    print(f"  Mean curvature: {(v.principal_curvature().max_value() + v.principal_curvature().min_value()) * 0.5}")
    print(f"  Gaussian curvature: {v.principal_curvature().max_value() * v.principal_curvature().min_value()}")

    return original_mesh, taubin_mesh, pca_mesh, general_mesh


def main():
    try:
        original_mesh, taubin_mesh, pca_mesh, general_mesh = mesh_curvature()

        # Save the curvature meshes
        print("\n=== Saving Meshes ===")

        # Create results directory if it doesn't exist
        os.makedirs(VCLIB_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(taubin_mesh, f"{VCLIB_RESULTS_PATH}/015_taubin_curvature.ply")
        vcl.save_mesh(pca_mesh, f"{VCLIB_RESULTS_PATH}/015_pca_curvature.ply")
        vcl.save_mesh(general_mesh, f"{VCLIB_RESULTS_PATH}/015_general_curvature.ply")

        print(f"\nAll files have been saved to: {VCLIB_RESULTS_PATH}")
        print("Files saved:")
        print("  - 015_taubin_curvature.ply (colored by mean curvature)")
        print("  - 015_pca_curvature.ply (colored by Gaussian curvature)")
        print("  - 015_general_curvature.ply (colored by maximum curvature)")

        print("\n=== Example 015 completed successfully! ===")

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
