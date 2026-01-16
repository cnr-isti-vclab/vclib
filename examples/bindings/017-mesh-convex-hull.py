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

# This example demonstrates how to compute the convex hull of a point set
# using the incremental convex hull algorithm provided by VCLib.


def mesh_convex_hull():
    print("=== VCLib Example 017: Convex Hull ===\n")

    # Load an input mesh
    print("=== Loading Bunny Mesh ===")
    
    input_mesh = vcl.TriMesh()
    vcl.load_mesh(input_mesh, f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny.obj")
    input_mesh.set_name("Input Mesh")

    print(f"Input mesh loaded: {input_mesh.vertex_number()} vertices, "
          f"{input_mesh.face_number()} faces")

    # Compute convex hull from vertex positions
    print("\n=== Computing Convex Hull ===")
    
    # Extract vertex positions
    positions = [v.position() for v in input_mesh.vertices()]
    
    hull_mesh = vcl.convex_hull_tri_mesh(positions)
    hull_mesh.set_name("Convex Hull")

    print(f"Convex hull computed: {hull_mesh.vertex_number()} vertices, "
          f"{hull_mesh.face_number()} faces")

    # Also demonstrate computing hull from a simple point set
    print("\n=== Computing Convex Hull from Point Set ===")

    points = [
        vcl.Point3(0.0, 0.0, 0.0),
        vcl.Point3(1.0, 0.0, 0.0),
        vcl.Point3(0.0, 1.0, 0.0),
        vcl.Point3(0.0, 0.0, 1.0),
        vcl.Point3(1.0, 1.0, 0.0),
        vcl.Point3(1.0, 0.0, 1.0),
        vcl.Point3(0.0, 1.0, 1.0),
        vcl.Point3(1.0, 1.0, 1.0),
        vcl.Point3(0.5, 0.5, 0.5)  # internal point
    ]

    cube_hull = vcl.convex_hull_tri_mesh(points)
    cube_hull.set_name("Cube Convex Hull")

    print(f"Cube convex hull: {cube_hull.vertex_number()} vertices, "
          f"{cube_hull.face_number()} faces")

    return input_mesh, hull_mesh, cube_hull


def main():
    try:
        input_mesh, hull_mesh, cube_hull = mesh_convex_hull()

        # Save the results
        print("\n=== Saving Meshes ===")

        # Create results directory if it doesn't exist
        os.makedirs(VCLIB_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(hull_mesh, f"{VCLIB_RESULTS_PATH}/017_convex_hull.ply")
        vcl.save_mesh(cube_hull, f"{VCLIB_RESULTS_PATH}/017_cube_hull.ply")

        print(f"Files saved to: {VCLIB_RESULTS_PATH}")
        print("- 017_convex_hull.ply (convex hull of bunny mesh)")
        print("- 017_cube_hull.ply (convex hull of cube points)")

        print("\n=== Example 017 completed successfully! ===")

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
