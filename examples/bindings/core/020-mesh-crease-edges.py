# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl
import math
import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from paths import VCLIB_PYTHON_RESULTS_PATH

# This example demonstrates various approaches to compute and handle crease
# edges in meshes using:
# 1. Crease edge selection based on dihedral angles
# 2. Face edge filtering and extraction
# 3. Creating edge meshes from selected crease edges


def mesh_crease_edges():
    print("=== VCLib Example 020: Mesh Crease Edges ===\n")

    # /****** Creating test meshes ******/

    print("=== Creating Test Meshes ===")

    # Create a cylinder that will have clear crease edges
    cylinder = vcl.create_cylinder_tri_mesh(1.0, 2.0, 24)
    cylinder.set_name("Cylinder")
    vcl.update_vertex_and_face_normals(cylinder)

    # Create an icosahedron for comparison
    icosahedron = vcl.create_icosahedron_tri_mesh()
    icosahedron.set_name("Icosahedron")
    vcl.update_vertex_and_face_normals(icosahedron)

    print(
        f"Created cylinder: {cylinder.vertex_count()} vertices, "
        f"{cylinder.face_count()} faces"
    )
    print(
        f"Created icosahedron: {icosahedron.vertex_count()} vertices, "
        f"{icosahedron.face_count()} faces"
    )

    # /****** Method 1: Crease Edge Selection ******/

    print("\n=== Method 1: Crease Edge Selection ===")

    # Enable face adjacency for crease edge detection
    cylinder.enable_face_adjacent_faces()
    icosahedron.enable_face_adjacent_faces()

    vcl.update_face_adjacent_faces(cylinder)
    vcl.update_face_adjacent_faces(icosahedron)

    # Select crease edges with different angle thresholds
    vcl.select_crease_face_edges(
        cylinder, -math.radians(60.0), math.radians(60.0)
    )
    cylinder_crease_edges = vcl.face_edges_selection_count(cylinder)

    vcl.select_crease_face_edges(
        icosahedron, -math.radians(30.0), math.radians(30.0)
    )
    icosahedron_crease_edges = vcl.face_edges_selection_count(icosahedron)

    print(f"Cylinder crease edges (±60°): {cylinder_crease_edges}")
    print(f"Icosahedron crease edges (±30°): {icosahedron_crease_edges}")

    # /****** Method 2: Direct Crease Edge Computation ******/

    print("\n=== Method 2: Direct Crease Edge Computation ===")

    # Compute crease edges directly without selection
    cylinder_crease_list = vcl.crease_face_edges(
        cylinder, -math.radians(60.0), math.radians(60.0)
    )
    icosa_crease_list = vcl.crease_face_edges(
        icosahedron, -math.radians(30.0), math.radians(30.0), True
    )

    print(f"Cylinder crease edges (direct): {len(cylinder_crease_list)}")
    print(
        f"Icosahedron crease edges (with borders): {len(icosa_crease_list)}"
    )

    # Display some crease edge details
    print("First few cylinder crease edges:")
    for face_idx, edge_idx in cylinder_crease_list[:5]:
        face = cylinder.face(face_idx)
        angle = vcl.face_dihedral_angle_on_edge(face, edge_idx)
        print(
            f"  Face {face_idx}, edge {edge_idx} "
            f"(angle: {math.degrees(angle):.2f}°)"
        )

    # /****** Method 3: Edge Mesh Creation ******/

    print("\n=== Method 3: Edge Mesh Creation ===")

    # Create edge meshes from selected crease edges
    cylinder_crease_mesh = vcl.face_edge_selection_mesh_filter(cylinder)
    cylinder_crease_mesh.set_name("Cylinder Crease Edge Mesh")

    # Re-select for icosahedron and create edge mesh
    vcl.select_crease_face_edges(
        icosahedron, -math.radians(30.0), math.radians(30.0), True
    )
    icosa_crease_mesh = vcl.face_edge_selection_mesh_filter(icosahedron)
    icosa_crease_mesh.set_name("Icosahedron Crease Edge Mesh")

    print(
        f"Cylinder crease edge mesh: {cylinder_crease_mesh.vertex_count()} "
        f"vertices, {cylinder_crease_mesh.edge_count()} edges"
    )
    print(
        f"Icosahedron crease edge mesh: {icosa_crease_mesh.vertex_count()} "
        f"vertices, {icosa_crease_mesh.edge_count()} edges"
    )

    # Color edges of cylinder_crease_mesh
    cylinder_crease_mesh.enable_edge_color()

    for e in cylinder_crease_mesh.edges():
        if e.vertex(0).position().y() > 0:
            e.set_color(vcl.Color.RED)
        else:
            e.set_color(vcl.Color.BLUE)

    # /****** Method 4: Custom Filtering ******/

    print("\n=== Method 4: Custom Filtering ===")

    # Create a custom filter for very sharp edges only
    def sharp_edge_filter(face, edge_idx):
        if face.adj_face(edge_idx) is None:
            return False  # Skip border edges

        angle = vcl.face_dihedral_angle_on_edge(face, edge_idx)
        return abs(angle) > math.radians(75.0)  # Very sharp edges only

    sharp_edges_mesh = vcl.face_edge_mesh_filter(cylinder, sharp_edge_filter)
    sharp_edges_mesh.set_name("Sharp Edges Mesh")

    print(
        f"Sharp edges only (>75°): {sharp_edges_mesh.edge_count()} edges"
    )

    # /****** Method 5: Different Angle Thresholds ******/

    print("\n=== Method 5: Angle Threshold Comparison ===")

    thresholds = [30.0, 45.0, 60.0, 75.0, 90.0]
    print("Crease edges count by angle threshold (cylinder):")

    for thresh in thresholds:
        vcl.select_crease_face_edges(
            cylinder, -math.radians(thresh), math.radians(thresh)
        )
        count = vcl.face_edges_selection_count(cylinder)
        print(f"  ±{thresh}°: {count} edges")

    return (
        cylinder,
        icosahedron,
        cylinder_crease_mesh,
        icosa_crease_mesh,
        sharp_edges_mesh,
    )


def main():
    try:
        (
            cylinder,
            icosahedron,
            cylinder_crease_mesh,
            icosa_crease_mesh,
            sharp_edges_mesh,
        ) = mesh_crease_edges()

        # /****** Save the created meshes ******/

        print("\n=== Saving Meshes ===")

        os.makedirs(VCLIB_PYTHON_RESULTS_PATH, exist_ok=True)

        # Save original meshes
        vcl.save_mesh(
            cylinder,
            f"{VCLIB_PYTHON_RESULTS_PATH}/020_cylinder.ply",
            binary=False,
        )
        vcl.save_mesh(
            icosahedron,
            f"{VCLIB_PYTHON_RESULTS_PATH}/020_icosahedron.ply",
            binary=False,
        )

        # Save crease edge meshes
        vcl.save_mesh(
            cylinder_crease_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/020_cylinder_crease_edges.ply",
            binary=False,
        )
        vcl.save_mesh(
            icosa_crease_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/020_icosahedron_crease_edges.ply",
            binary=False,
        )
        vcl.save_mesh(
            sharp_edges_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/020_sharp_edges_only.ply",
            binary=False,
        )

        print(f"\nAll files have been saved to: {VCLIB_PYTHON_RESULTS_PATH}")
        print("\n=== Example 020 completed successfully! ===")

    except Exception as e:
        print(f"Error in saving: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
