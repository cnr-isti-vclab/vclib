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
import numpy as np
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

    print("PART 1: Mesh Copying and Cloning")
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

    # ========================================
    # PART 2: GEOMETRIC TRANSFORMATIONS
    # ========================================

    print("PART 2: Geometric Transformations")
    print("---------------------------------\n")

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
    print("\n    Non-uniform Scaling:\n")
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
    rotation_angle_radians = np.radians(rotation_angle)

    print(f"   Rotating around axis {rotation_axis} by {rotation_angle} degrees\n")

    vcl.rotate(rotate_mesh, rotation_axis, rotation_angle_radians)
    vcl.update_bounding_box(rotate_mesh)

    bb_rotated = rotate_mesh.bounding_box()
    print(f"   Bounding box after rotation: dimensions {bb_rotated.size()}\n")

    # ========================================
    # PART 3: TRANSFORMATION MATRICES
    # ========================================

    print("PART 3: Transformation Matrices")
    print("-------------------------------\n")

    print("Creating a composite transformation matrix:\n")

    matrix_mesh = vcl.create_icosahedron_tri_mesh()
    matrix_mesh.set_name("Matrix Transformed Icosahedron")

    # 4x4 transformation matrix (translation + rotation + scale)
    transform_matrix = np.eye(4)

    rot_angle = np.radians(45)
    cos_angle = np.cos(rot_angle)
    sin_angle = np.sin(rot_angle)

    # 3x3 rotation matrix around Y-axis
    rotation_matrix = np.array([
        [cos_angle, 0, sin_angle],
        [0, 1, 0],
        [-sin_angle, 0, cos_angle]
    ])

    transform_matrix[:3, :3] = rotation_matrix

    # Add translation
    transform_matrix[:3, 3] = [1.5, 1.0, 0.5]

    # Add a scale factor of 1.5
    scale_factor = 1.5
    transform_matrix[0, 0] *= scale_factor
    transform_matrix[1, 1] *= scale_factor
    transform_matrix[2, 2] *= scale_factor

    print(f"   Transformation matrix:\n{transform_matrix}\n")

    # Apply the transformation matrix to the mesh
    bb_matrix_before = matrix_mesh.bounding_box()
    print(f"   Before transformation: center = {bb_matrix_before.center()}\n")
    vcl.apply_transform_matrix(matrix_mesh, transform_matrix)
    vcl.update_bounding_box(matrix_mesh)

    bb_matrix_after = matrix_mesh.bounding_box()
    print(f"   After transformation: center = {bb_matrix_after.center()}\n")

    # ========================================
    # PART 4: MESH COMBINATION (APPEND)
    # ========================================

    print("PART 4: Mesh Combination (Append)")
    print("---------------------------------\n")

    combined_mesh = vcl.create_cube_tri_mesh()
    print (f"   Main Mesh (cube): {combined_mesh.vertex_number()} vertices, {combined_mesh.face_number()} faces\n")
    combined_mesh.set_name("Combined Scene")

    # Create different primitives to add
    sp = vcl.Sphere([0.0, 0.0, 0.0], 1.0)

    sphere1 = vcl.create_sphere_tri_mesh(sp, vcl.ICOSAHEDRON, divisions=3)
    vcl.translate(sphere1, vcl.Point3(3.0, 0.0, 0.0))

    sphere2 = vcl.create_sphere_tri_mesh(sp, vcl.UV, parallels=16, meridians=16)
    vcl.translate(sphere2, vcl.Point3(-3.0, 0.0, 0.0))
    vcl.scale(sphere2, 0.8)

    tetrahedron = vcl.create_tetrahedron_tri_mesh()
    vcl.translate(tetrahedron, vcl.Point3(0.0, 3.0, 0.0))
    vcl.scale(tetrahedron, 1.5)

    print("\nPrimitives to add:\n")
    print(f"- Icosphere (3 subdivisions): {sphere1.vertex_number()} vertices, {sphere1.face_number()} faces")
    print(f"- UV Sphere (16x16): {sphere2.vertex_number()} vertices, {sphere2.face_number()} faces")
    print(f"- Tetrahedron: {tetrahedron.vertex_number()} vertices, {tetrahedron.face_number()} faces")

    print("\nCombination through append:\n")

    original_count = combined_mesh.vertex_number()
    combined_mesh.append(sphere1)
    print(f"After appending sphere 1: {combined_mesh.vertex_number()} vertices (+" \
          f"{combined_mesh.vertex_number() - original_count} from original)\n")

    combined_mesh.append(sphere2)
    print(f"After appending sphere 2: {combined_mesh.vertex_number()} vertices (+" \
          f"{combined_mesh.vertex_number() - original_count} from original)\n")

    combined_mesh.append(tetrahedron)
    print(f"After appending tetrahedron: {combined_mesh.vertex_number()} vertices (+" \
          f"{combined_mesh.vertex_number() - original_count} from original)\n")

    print(f"\nFinal combined mesh: {combined_mesh.vertex_number()} vertices, {combined_mesh.face_number()} faces\n")

    vcl.update_bounding_box(combined_mesh)
    bb_combined = combined_mesh.bounding_box()
    print(f"Bounding box of combined mesh:\n")
    print(f"Min: ({bb_combined.min().x()}, "
          f"{bb_combined.min().y()}, {bb_combined.min().z()})\n")
    print(f"Max: ({bb_combined.max().x()}, "
          f"{bb_combined.max().y()}, {bb_combined.max().z()})\n")

    return (transform_mesh, scale_mesh, rotate_mesh, matrix_mesh, combined_mesh)

if __name__ == "__main__":
    transform_mesh, scale_mesh, rotate_mesh, matrix_mesh, combined_mesh = mesh_copy_and_transform()

    try:
        # Save the meshes to files
        vcl.save_mesh(transform_mesh, VCLIB_RESULTS_PATH + "/004_transformed_icosahedron.ply")
        vcl.save_mesh(scale_mesh, VCLIB_RESULTS_PATH + "/004_scaled_cube.ply")
        vcl.save_mesh(rotate_mesh, VCLIB_RESULTS_PATH + "/004_rotated_cube.ply")
        vcl.save_mesh(matrix_mesh, VCLIB_RESULTS_PATH + "/004_matrix_transformed.ply")
        vcl.save_mesh(combined_mesh, VCLIB_RESULTS_PATH + "/004_combined_scene.ply")
        print("Meshes saved successfully.")
    except Exception as e:
        print(f"Error saving meshes: {e}")

    print("Example completed successfully!")
