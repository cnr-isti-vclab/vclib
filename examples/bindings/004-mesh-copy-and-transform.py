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

    # 2.2 Scaling
    print("\n2.2 Scaling:\n")
    scale_mesh = vcl.create_cube_tri_mesh()
    vcl.update_bounding_box(scale_mesh)
    scale_mesh.set_name("Scaled Cube")

    # Calculate bounding box before scaling
    bb_before_scale = scale_mesh.bounding_box()
    diagonal_before = bb_before_scale.diagonal()
    print(f"   Diagonal before scaling: {diagonal_before}\n")

    # Apply uniform scaling
    scale_factor = 2.5
    vcl.scale(scale_mesh, scale_factor)
    vcl.update_bounding_box(scale_mesh)

    bb_after_scale = scale_mesh.bounding_box()
    diagonal_after = bb_after_scale.diagonal()
    print(f"   Diagonal after scaling by {scale_factor}: {diagonal_after}\n")
    print(f"   Diagonal ratio: {diagonal_after / diagonal_before} (should be ~{scale_factor})\n")

    # Non-uniform scaling
    print("\n2.3 Non-uniform Scaling:\n")
    non_uniform_scale_mesh = vcl.create_cube_tri_mesh()
    vcl.update_bounding_box(non_uniform_scale_mesh)
    non_uniform_scale_mesh.set_name("Non-uniform Scaled Cube")

    scale_factors = vcl.Point3(1.0, 2.0, 0.5)
    vcl.scale(non_uniform_scale_mesh, scale_factors)
    vcl.update_bounding_box(non_uniform_scale_mesh)

    bb_non_uniform = non_uniform_scale_mesh.bounding_box()
    size = bb_non_uniform.size()
    print(f"   Dimensions after non-uniform scaling {scale_factors}: {size}")

    # 2.3 Rotation
    print("\n2.3 Rotation:\n")
    rotate_mesh = vcl.create_cube_tri_mesh()
    vcl.update_bounding_box(rotate_mesh)
    rotate_mesh.set_name("Rotated Cube")

    # Rotation around Z-axis by 45 degrees
    rotation_axis = vcl.Point3(0.0, 0.0, 1.0)
    rotation_angle = 45.0

    print(f"   Rotating around axis {rotation_axis} by {rotation_angle} degrees\n")

    vcl.rotate(rotate_mesh, rotation_axis, rotation_angle)
    vcl.update_bounding_box(rotate_mesh)

    bb_rotated = rotate_mesh.bounding_box()
    print(f"   Bounding box after rotation: {bb_rotated.min()} to {bb_rotated.max()}\n")

    # ========================================
    # PART 3: TRANSFORMATION MATRICES
    # ========================================

    print("PART 3: Transformation Matrices\n")

    print("Creating a composite transformation matrix:\n")

    matrix_mesh = vcl.create_icosahedron_tri_mesh()
    matrix_mesh.set_name("Matrix Transformed Icosahedron")

    # TODO: Define a transformation matrix

if __name__ == "__main__":
    mesh_copy_and_transform()

    print("Example completed successfully!")
