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

# This example demonstrates mesh distance calculation in VCLib:
# 1. Computing Hausdorff distance between two meshes
# 2. Smoothing a mesh and computing distance with original
# 3. Using different distance sampling methods


def mesh_distance():
    print("=== VCLib Example 018: Mesh Distance ===\n")

    # Load a mesh
    print("=== Loading Mesh ===")

    mesh = vcl.TriMesh()
    vcl.load_mesh(mesh, f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny.obj")

    print(f"Loaded mesh with {mesh.vertex_number()} vertices and "
          f"{mesh.face_number()} faces")

    # Prepare mesh for distance computation
    vcl.update_bounding_box(mesh)
    vcl.update_per_face_normals(mesh)

    # Create a smoothed version
    print("\n=== Creating Smoothed Version ===")

    smoothed_mesh = vcl.TriMesh(mesh)  # Copy the original mesh

    # Apply Laplacian smoothing
    vcl.laplacian_smoothing(smoothed_mesh, 5)  # 5 iterations

    print("Applied 5 iterations of Laplacian smoothing")

    # Update normals for the smoothed mesh
    vcl.update_per_face_normals(smoothed_mesh)

    # Compute distance between original and smoothed mesh
    print("\n=== Computing Distance (Original vs Smoothed) ===")

    # Compute Hausdorff distance using vertex uniform sampling
    result = vcl.hausdorff_distance(
        mesh, smoothed_mesh, vcl.HAUSDORFF_VERTEX_UNIFORM)

    print("Hausdorff Distance Results:")
    print(f"  Min distance:  {result.minDist}")
    print(f"  Max distance:  {result.maxDist}")
    print(f"  Mean distance: {result.meanDist}")
    print(f"  RMS distance:  {result.RMSDist}")

    # Compare different smoothing methods
    print("\n=== Comparing Smoothing Methods ===")

    # Create a Taubin smoothed version
    taubin_mesh = vcl.TriMesh(mesh)
    vcl.taubin_smoothing(
        taubin_mesh, 5, 0.5, -0.53)  # 5 iterations, lambda=0.5, mu=-0.53
    vcl.update_per_face_normals(taubin_mesh)

    # Compute distance for Taubin smoothing
    taubin_result = vcl.hausdorff_distance(
        mesh, taubin_mesh, vcl.HAUSDORFF_VERTEX_UNIFORM)

    print("Taubin Smoothing Distance Results:")
    print(f"  Min distance:  {taubin_result.minDist}")
    print(f"  Max distance:  {taubin_result.maxDist}")
    print(f"  Mean distance: {taubin_result.meanDist}")
    print(f"  RMS distance:  {taubin_result.RMSDist}")

    # Compare different sampling methods
    print("\n=== Comparing Sampling Methods ===")

    # Monte Carlo sampling
    mc_result = vcl.hausdorff_distance(
        mesh, smoothed_mesh, vcl.HAUSDORFF_MONTECARLO)

    print("Monte Carlo Sampling Results:")
    print(f"  Min distance:  {mc_result.minDist}")
    print(f"  Max distance:  {mc_result.maxDist}")
    print(f"  Mean distance: {mc_result.meanDist}")
    print(f"  RMS distance:  {mc_result.RMSDist}")

    # Load and compare two different meshes
    print("\n=== Distance Between Different Meshes ===")

    cube = vcl.TriMesh()
    vcl.load_mesh(cube, f"{VCLIB_EXAMPLE_MESHES_PATH}/cube_tri.ply")

    vcl.update_bounding_box(cube)
    vcl.update_per_face_normals(cube)

    cube_result = vcl.hausdorff_distance(
        mesh, cube, vcl.HAUSDORFF_VERTEX_UNIFORM)

    print("Bunny vs Cube Distance Results:")
    print(f"  Min distance:  {cube_result.minDist}")
    print(f"  Max distance:  {cube_result.maxDist}")
    print(f"  Mean distance: {cube_result.meanDist}")
    print(f"  RMS distance:  {cube_result.RMSDist}")

    print("\nExample completed successfully!")

    return mesh, smoothed_mesh, taubin_mesh, cube


def main():
    try:
        mesh, smoothed_mesh, taubin_mesh, cube = mesh_distance()

        # We don't save any meshes in this example
        # as the focus is on computing and displaying distance metrics

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
