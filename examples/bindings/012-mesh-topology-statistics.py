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

# This example demonstrates how to analyze mesh topology and compute statistics:
# 1. Finding connected components in a mesh
# 2. Analyzing component sizes
# 3. Checking if a mesh is water tight
# 4. Counting non-manifold vertices
# 5. Counting holes in a mesh


def mesh_topology_statistics():
    print("=== VCLib Example 012: Mesh Topology Statistics ===\n")

    # Connected components analysis
    print("=== Connected Components Analysis ===")

    # Load a mesh with multiple components
    range_mesh = vcl.TriMesh()
    vcl.load_mesh(range_mesh, f"{VCLIB_EXAMPLE_MESHES_PATH}/rangemap.ply")

    range_mesh.enable_per_face_adjacent_faces()
    vcl.update_per_face_adjacent_faces(range_mesh)

    print(f"Range mesh: {range_mesh.vertex_number()} vertices, "
          f"{range_mesh.face_number()} faces")

    # Find connected components
    components = vcl.connected_components(range_mesh)
    print(f"Number of connected components: {len(components)}")

    # Print component sizes
    for i in range(min(len(components), 5)):
        print(f"Component {i}: {len(components[i])} faces")

    if len(components) > 5:
        print(f"... and {len(components) - 5} more components")

    # Topology analysis
    print("\n=== Topology Analysis ===")

    mesh = vcl.TriMesh()
    vcl.load_mesh(mesh, f"{VCLIB_EXAMPLE_MESHES_PATH}/brain.ply")
    mesh.set_name("Brain Mesh")

    # Enable adjacency information for topology checks
    mesh.enable_per_face_adjacent_faces()
    vcl.update_per_face_adjacent_faces(mesh)

    # Check if mesh is water tight
    water_tight = vcl.is_water_tight(mesh)
    print(f"Is water tight: {'Yes' if water_tight else 'No'}")

    # Count non-manifold vertices
    non_manifold = vcl.number_non_manifold_vertices(mesh)
    print(f"Non-manifold vertices: {non_manifold}")

    # Count holes
    holes = vcl.number_holes(mesh)
    print(f"Number of holes: {holes}")

    print("\nExample completed successfully!")


def main():
    try:
        mesh_topology_statistics()

    except Exception as e:
        print(f"Error in main: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
