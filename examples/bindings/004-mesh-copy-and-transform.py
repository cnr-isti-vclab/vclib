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

import copy
import os

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = current_file_path + "/../../assets/example_meshes"
VCLIB_RESULTS_PATH = current_file_path + "/../../assets/results/python"

# This example demonstrates mesh creation in VCLib:
# 1. Creating basic geometric primitives (tetrahedron, cube, sphere, etc.)
# 2. Understanding the difference between TriMesh and PolyMesh
# 3. Different sphere generation algorithms
# 4. Creating meshes with custom parameters
# 5. Saving created meshes to files

def mesh_copy_and_transform():
    print("=== VCLib Example 004: Mesh Copy and Transform ===\n")

    # PART 1: MESH COPYING AND CLONING

    print("PART 1: Mesh Copying and Cloning\n")
    print("---------------------------------\n")

    # Create a starting mesh: an icosahedron

    original_mesh = vcl.create_icosahedron_tri_mesh()

    print(f"Original mesh created: icosahedron with {original_mesh.vertex_number()} vertices and {original_mesh.face_number()} faces\n")

    # Copy via deepcopy

    print("\nCopy via deepcopy:\n")

    copied_mesh = copy.deepcopy(original_mesh)
    print(f"   Mesh copied with {copied_mesh.vertex_number()} vertices\n")

    # Verify that copies are independent
    print("\nVerifying independence of copies:\n")
    print(f"   ID of original mesh: {id(original_mesh)}")
    print(f"   ID of copied mesh: {id(copied_mesh)}")

    original_mesh.clear()
    print(f"   Original mesh cleared: {original_mesh.vertex_number()} vertices\n")
    print(f"   Copied mesh maintains: {copied_mesh.vertex_number()} vertices\n")

    # PART 2: GEOMETRIC TRANSFORMATIONS
    print("PART 2: Geometric Transformations\n")

    transform_mesh = vcl.create_icosahedron_tri_mesh()
    vcl.update_bounding_box(transform_mesh)
    transform_mesh.set_name("Transformed Icosahedron")

    # 2.1 Translation
    print("\n2.1 Translation:\n")
    translation = vcl.Point3(2.0, 1.0, 0.5)

    # Calculate bounding box before translation
    bb_before = transform_mesh.bounding_box()
    print(f"   Bounding box before: {bb_before.min()} to {bb_before.max()}")

    # Apply translation
    vcl.translate(transform_mesh, translation)
    vcl.update_bounding_box(transform_mesh)

    bb_after = transform_mesh.bounding_box()
    print(f"   Bounding box after translation {translation}: {bb_after.min()} to {bb_after.max()}")


if __name__ == "__main__":
    mesh_copy_and_transform()

    print("Example completed successfully!")
