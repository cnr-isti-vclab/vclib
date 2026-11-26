# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl
import math
import numpy as np
import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from paths import VCLIB_EXAMPLE_MESHES_PATH, VCLIB_PYTHON_RESULTS_PATH

# This example demonstrates mesh filtering and conditional deletion algorithms:
# 1. Creating new meshes by filtering vertices (predicate and selection)
# 2. Creating new meshes by filtering faces (predicate, boolean mask, and
#    selection)
# 3. Creating new EdgeMeshes by filtering edges (predicate and selection)
# 4. Deleting faces, vertices, and edges in-place with delete_*_if


def mesh_filter_and_delete():
    print("=== VCLib Example 023: Mesh Filter and Delete ===\n")

    # /****** Loading input mesh ******/

    bunny = vcl.load_tri_mesh(f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny.obj")
    bunny.set_name("Original Bunny")
    vcl.update_vertex_and_face_normals(bunny)

    print(
        f"Loaded bunny: {bunny.vertex_count()} vertices, "
        f"{bunny.face_count()} faces"
    )

    # /****** 1. Vertex Filtering ******/

    print("\n=== 1. Vertex Filtering ===")

    # Filter vertices using a predicate function
    upper_vertices = vcl.vertex_mesh_filter(
        bunny, lambda v: v.position().y() > 0.1
    )
    upper_vertices.set_name("Upper Vertices")

    print(
        f"Upper vertices (y > 0.1): {upper_vertices.vertex_count()} vertices, "
        f"{upper_vertices.face_count()} faces"
    )

    # Filter vertices by selection
    for v in bunny.vertices():
        v.set_selected(v.position().x() > 0.3)
    right_selected_vertices = vcl.vertex_selection_mesh_filter(bunny)
    right_selected_vertices.set_name("Right Selected Vertices")

    print(
        f"Selected vertices (x > 0.3): "
        f"{right_selected_vertices.vertex_count()} vertices"
    )

    # /****** 2. Face Filtering ******/

    print("\n=== 2. Face Filtering ===")

    # Filter faces using a predicate function
    upward_faces = vcl.face_mesh_filter(
        bunny, lambda f: f.normal().y() > 0.0
    )
    upward_faces.set_name("Upward Faces")

    print(
        f"Upward-facing mesh (normal.y > 0): {upward_faces.vertex_count()} "
        f"vertices, {upward_faces.face_count()} faces"
    )

    # Filter faces using a boolean mask (NumPy array)
    face_mask = np.array(
        [
            (
                f.vertex(0).position().y()
                + f.vertex(1).position().y()
                + f.vertex(2).position().y()
            )
            / 3.0
            > 0.1
            for f in bunny.faces()
        ],
        dtype=bool,
    )

    upper_faces = vcl.face_mesh_filter(bunny, face_mask)
    upper_faces.set_name("Upper Faces")

    print(
        f"Upper faces (barycenter.y > 0.1): {upper_faces.vertex_count()} "
        f"vertices, {upper_faces.face_count()} faces"
    )

    # Filter faces by selection
    vcl.clear_face_selection(bunny)
    for f in bunny.faces():
        if f.normal().y() > 0.5:
            f.set_selected(True)

    selected_faces = vcl.face_selection_mesh_filter(bunny)
    selected_faces.set_name("Selected Faces")

    print(
        f"Selected faces (normal.y > 0.5): {selected_faces.vertex_count()} "
        f"vertices, {selected_faces.face_count()} faces"
    )

    # /****** 3. Edge Filtering ******/

    print("\n=== 3. Edge Filtering ===")

    # Build an EdgeMesh from the crease edges of a cylinder
    cylinder = vcl.create_cylinder_tri_mesh(1.0, 2.0, 24)
    cylinder.enable_face_adjacent_faces()
    vcl.update_face_adjacent_faces(cylinder)
    vcl.select_crease_face_edges(
        cylinder, -math.radians(60.0), math.radians(60.0)
    )

    crease_edges = vcl.face_edge_selection_mesh_filter(cylinder)
    print(
        f"Extracted crease EdgeMesh: {crease_edges.vertex_count()} vertices, "
        f"{crease_edges.edge_count()} edges"
    )

    # Filter edges with a predicate (only edges in the upper cap)
    top_cap_edges = vcl.edge_mesh_filter(
        crease_edges,
        lambda e: e.vertex(0).position().y() > 0.0
        and e.vertex(1).position().y() > 0.0,
    )
    top_cap_edges.set_name("Top Cap Edges")

    print(
        f"Filtered top cap edges: {top_cap_edges.vertex_count()} vertices, "
        f"{top_cap_edges.edge_count()} edges"
    )

    # Filter edges by selection
    for e in crease_edges.edges():
        e.set_selected(e.vertex(0).position().x() > 0.0)
    selected_edges = vcl.edge_selection_mesh_filter(crease_edges)
    selected_edges.set_name("Selected Edges")

    print(
        f"Selected edges: {selected_edges.vertex_count()} vertices, "
        f"{selected_edges.edge_count()} edges"
    )

    # /****** 4. In-place Conditional Deletion (delete_*_if) ******/

    print("\n=== 4. In-place Conditional Deletion ===")

    # 4a. Delete faces in-place using a predicate
    deleted_faces_mesh = vcl.TriMesh(bunny)
    deleted_faces_mesh.set_name("Deleted Faces Bunny")

    n_del_faces = vcl.delete_faces_if(
        deleted_faces_mesh, lambda f: f.normal().y() < 0.0
    )
    vcl.remove_unreferenced_vertices(deleted_faces_mesh)
    deleted_faces_mesh.compact()

    print(
        f"Deleted {n_del_faces} downward faces -> remaining: "
        f"{deleted_faces_mesh.vertex_count()} vertices, "
        f"{deleted_faces_mesh.face_count()} faces"
    )

    # 4b. Delete vertices in-place and clean invalidated faces
    deleted_verts_mesh = vcl.TriMesh(bunny)
    deleted_verts_mesh.set_name("Deleted Vertices Bunny")

    n_del_verts = vcl.delete_vertices_if(
        deleted_verts_mesh, lambda v: v.position().y() < 0.1
    )
    # Remove faces whose vertex references were invalidated (UINT_NULL)
    n_cleaned_faces = vcl.delete_faces_if(
        deleted_verts_mesh,
        lambda f: any(vi == vcl.UINT_NULL for vi in f.vertex_indices()),
    )
    deleted_verts_mesh.compact()

    print(
        f"Deleted {n_del_verts} vertices and {n_cleaned_faces} invalidated "
        f"faces -> remaining: {deleted_verts_mesh.vertex_count()} vertices, "
        f"{deleted_verts_mesh.face_count()} faces"
    )

    # 4c. Delete edges in-place using a Python list mask
    deleted_edges_mesh = vcl.EdgeMesh(crease_edges)
    deleted_edges_mesh.set_name("Deleted Edges")

    edge_delete_mask = [
        e.vertex(0).position().y() < 0.0 for e in deleted_edges_mesh.edges()
    ]

    n_del_edges = vcl.delete_edges_if(deleted_edges_mesh, edge_delete_mask)
    vcl.remove_unreferenced_vertices(deleted_edges_mesh)
    deleted_edges_mesh.compact()

    print(
        f"Deleted {n_del_edges} bottom edges -> remaining: "
        f"{deleted_edges_mesh.vertex_count()} vertices, "
        f"{deleted_edges_mesh.edge_count()} edges"
    )

    return (
        upper_vertices,
        upper_faces,
        selected_faces,
        top_cap_edges,
        deleted_faces_mesh,
        deleted_verts_mesh,
        deleted_edges_mesh,
    )


def main():
    try:
        (
            upper_vertices,
            upper_faces,
            selected_faces,
            top_cap_edges,
            deleted_faces_mesh,
            deleted_verts_mesh,
            deleted_edges_mesh,
        ) = mesh_filter_and_delete()

        # /****** Save the created meshes ******/

        print("\n=== Saving Meshes ===")

        os.makedirs(VCLIB_PYTHON_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(
            upper_vertices,
            f"{VCLIB_PYTHON_RESULTS_PATH}/023_upper_vertices_bunny.ply",
        )
        vcl.save_mesh(
            upper_faces,
            f"{VCLIB_PYTHON_RESULTS_PATH}/023_upper_faces_bunny.ply",
        )
        vcl.save_mesh(
            selected_faces,
            f"{VCLIB_PYTHON_RESULTS_PATH}/023_selected_faces_bunny.ply",
        )
        vcl.save_mesh(
            top_cap_edges,
            f"{VCLIB_PYTHON_RESULTS_PATH}/023_top_cap_edges.ply",
        )
        vcl.save_mesh(
            deleted_faces_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/023_deleted_faces_bunny.ply",
        )
        vcl.save_mesh(
            deleted_verts_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/023_deleted_vertices_bunny.ply",
        )
        vcl.save_mesh(
            deleted_edges_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/023_deleted_edges.ply",
        )

        print(f"\nAll files have been saved to: {VCLIB_PYTHON_RESULTS_PATH}")
        print("\n=== Example 023 completed successfully! ===")

    except Exception as e:
        print(f"Error in saving: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
