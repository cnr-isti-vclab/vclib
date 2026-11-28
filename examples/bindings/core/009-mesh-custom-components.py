# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

import vclib as vcl
import numpy as np
import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from paths import VCLIB_EXAMPLE_MESHES_PATH, VCLIB_PYTHON_RESULTS_PATH


# This example demonstrates how to:
# 1. Add custom components to mesh elements (vertices, faces)
# 2. Access element custom components individually and in bulk via NumPy/lists
# 3. Add custom components to the mesh itself (typed and Python object fallback)
# 4. Use custom components in algorithms and inspect C++-generated ones (birthVertex)


def mesh_custom_components():
    print("=== VCLib Example 009: Mesh Custom Components ===\n")

    # ========================================
    # PART 1: LOADING MESH AND BASIC SETUP
    # ========================================

    print("PART 1: Loading Mesh and Basic Setup")
    print("-------------------------------------")

    mesh = vcl.TriMesh()
    vcl.load_mesh(mesh, VCLIB_EXAMPLE_MESHES_PATH + "/bimba_simplified.obj")

    print(
        f"Loaded mesh: {mesh.vertex_count()} vertices, "
        f"{mesh.face_count()} faces\n"
    )

    # ========================================
    # PART 2: VERTEX CUSTOM COMPONENTS
    # ========================================

    print("PART 2: Vertex Custom Components")
    print("---------------------------------")

    # Add custom integer component for vertex processing flags
    mesh.add_vertex_custom_component("processingFlag", int)
    print("Added vertex custom component 'processingFlag'")
    print(
        "Has processingFlag component: "
        f"{mesh.has_vertex_custom_component('processingFlag')}"
    )

    # Set values per vertex
    for vertex in mesh.vertices():
        is_boundary = vertex.index() % 10 == 0  # simplified candidate flag
        vertex.set_custom_component("processingFlag", 1 if is_boundary else 0)

    # Bulk read via NumPy array (much faster than looping in Python)
    flags_array = mesh.vertex_custom_component_array("processingFlag")
    boundary_count = int(np.sum(flags_array == 1))
    print(f"Marked {boundary_count} vertices as boundary candidates")

    # Add custom Point3 component and populate in bulk from vertex positions
    mesh.add_vertex_custom_component("originalPosition", vcl.Point3)
    print("Added vertex custom component 'originalPosition'")
    mesh.set_vertex_custom_component_array(
        "originalPosition", np.array(mesh.vertices().positions())
    )

    # Add custom float (64-bit double) component for vertex quality metrics
    mesh.add_vertex_custom_component("quality", float)
    print("Added vertex custom component 'quality'")

    centroid = vcl.barycenter(mesh)
    for vertex in mesh.vertices():
        dist_from_center = (vertex.position() - centroid).norm()
        vertex.set_custom_component("quality", dist_from_center)

    quality_array = mesh.vertex_custom_component_array("quality")
    print(
        f"Quality range: [{quality_array.min()}, {quality_array.max()}]\n"
    )

    # ========================================
    # PART 3: FACE CUSTOM COMPONENTS
    # ========================================

    print("PART 3: Face Custom Components")
    print("------------------------------")

    # Add float component for face areas
    mesh.add_face_custom_component("area", float)
    print("Added face custom component 'area'")

    for face in mesh.faces():
        v0 = face.vertex(0).position()
        v1 = face.vertex(1).position()
        v2 = face.vertex(2).position()
        area = 0.5 * (v1 - v0).cross(v2 - v0).norm()
        face.set_custom_component("area", area)

    face_areas = mesh.face_custom_component_array("area")
    total_surface_area = float(np.sum(face_areas))
    avg_area = total_surface_area / mesh.face_count()

    # Add int component for material IDs (computed in bulk via NumPy)
    mesh.add_face_custom_component("materialID", int)
    material_ids = (face_areas > avg_area).astype(np.int32)
    mesh.set_face_custom_component_array("materialID", material_ids)
    print("Added face custom component 'materialID'")

    material0_count = int(np.sum(material_ids == 0))
    material1_count = int(np.sum(material_ids == 1))
    print(
        f"Material assignment: {material0_count} small faces, "
        f"{material1_count} large faces"
    )

    # Add str component for face labels
    mesh.add_face_custom_component("label", str)
    print("Added face custom component 'label'")

    for face in mesh.faces():
        mat_id = face.custom_component("materialID")
        prefix = "large_face" if mat_id == 1 else "small_face"
        face.set_custom_component("label", f"{prefix}_{face.index()}")

    first_labels = [
        f'"{mesh.face(i).custom_component("label")}"'
        for i in range(min(3, mesh.face_count()))
    ]
    print("First 3 face labels: " + " ".join(first_labels))

    # Add Color component (reference semantics allow in-place mutation)
    mesh.add_face_custom_component("highlightColor", vcl.Color)
    if mesh.face_count() > 0:
        mesh.face(0).set_custom_component(
            "highlightColor", vcl.Color(255, 128, 0, 255)
        )
        mesh.face(0).custom_component("highlightColor").set_alpha(200)
        print(
            "Face 0 highlightColor alpha after in-place update: "
            f"{mesh.face(0).custom_component('highlightColor').alpha()}\n"
        )

    # ========================================
    # PART 4: MESH-LEVEL CUSTOM COMPONENTS
    # ========================================

    print("PART 4: Mesh-Level Custom Components")
    print("------------------------------------")

    # Types are inferred automatically from Python values
    mesh.add_custom_component("modelName", "Bimba Model")
    mesh.add_custom_component("processingVersion", 1)
    mesh.add_custom_component("meshQuality", 0.85)
    mesh.add_custom_component("totalSurfaceArea", total_surface_area)

    vcl.update_bounding_box(mesh)
    bbox = mesh.bounding_box()
    mesh.add_custom_component("boundingBoxSize", bbox.size())
    mesh.add_custom_component("meshCentroid", centroid)

    # Arbitrary Python objects fall back to the GIL-safe 'object' wrapper
    mesh.add_custom_component(
        "pipelineMetadata",
        {
            "source": "bimba_simplified.obj",
            "tags": ["demo", "custom_components"],
            "verified": True,
        },
    )

    print("Added mesh-level custom components:")
    print(f"  Model name: {mesh.custom_component('modelName')}")
    print(f"  Processing version: {mesh.custom_component('processingVersion')}")
    print(f"  Mesh quality: {mesh.custom_component('meshQuality')}")
    print(f"  Total surface area: {mesh.custom_component('totalSurfaceArea')}")
    print(f"  Bounding box size: {mesh.custom_component('boundingBoxSize')}")
    print(f"  Mesh centroid: {mesh.custom_component('meshCentroid')}")
    print(f"  Pipeline metadata (object fallback): {mesh.custom_component('pipelineMetadata')}\n")

    # ========================================
    # PART 5: USING CUSTOM COMPONENTS IN ALGORITHMS
    # ========================================

    print("PART 5: Using Custom Components in Algorithms")
    print("---------------------------------------------")

    print("Applying selective smoothing based on processing flags...")

    new_positions = [vertex.position().copy() for vertex in mesh.vertices()]

    mesh.enable_vertex_adjacent_vertices()
    vcl.update_vertex_adjacent_vertices(mesh)

    for vertex in mesh.vertices():
        if vertex.custom_component("processingFlag") == 1:
            avg_pos = vcl.Point3(0.0, 0.0, 0.0)
            adj_count = 0
            for adj_vertex in vertex.adj_vertices():
                avg_pos += adj_vertex.position()
                adj_count += 1
            if adj_count > 0:
                avg_pos /= adj_count
                new_positions[vertex.index()] = (
                    0.7 * vertex.position() + 0.3 * avg_pos
                )

    for vertex in mesh.vertices():
        vertex.set_position(new_positions[vertex.index()])

    cur_positions = np.array(mesh.vertices().positions())
    orig_positions = mesh.vertex_custom_component_array("originalPosition")
    avg_displacement = float(
        np.mean(np.linalg.norm(cur_positions - orig_positions, axis=1))
    )

    print(f"Average vertex displacement: {avg_displacement}")
    mesh.add_custom_component("averageDisplacement", avg_displacement)

    # Interoperability with C++ algorithms that produce custom components:
    # vertex_mesh_filter stores source vertex indices in uint 'birthVertex'.
    filtered_mesh = vcl.vertex_mesh_filter(
        mesh, lambda v: v.custom_component("processingFlag") == 1
    )
    birth_vertices = filtered_mesh.vertex_custom_component_array("birthVertex")
    print(
        f"Filtered mesh vertices: {filtered_mesh.vertex_count()}, "
        f"birthVertex type: {filtered_mesh.vertex_custom_component_type('birthVertex')}, "
        f"first 5 birth indices: {birth_vertices[:5]}\n"
    )

    # ========================================
    # PART 6: COMPONENT MANAGEMENT
    # ========================================

    print("PART 6: Component Management")
    print("----------------------------")

    print(f"Vertex components: {mesh.vertex_custom_component_names()}")
    print(f"Face components:   {mesh.face_custom_component_names()}")
    print(f"Mesh components:   {mesh.custom_component_names()}")

    print("\nType checking examples:")
    print(
        "  processingFlag is int: "
        f"{mesh.is_vertex_custom_component_of_type('processingFlag', int)}"
    )
    print(
        "  processingFlag is float: "
        f"{mesh.is_vertex_custom_component_of_type('processingFlag', float)}"
    )
    print(
        "  area is float: "
        f"{mesh.is_face_custom_component_of_type('area', float)}"
    )
    print(
        "  pipelineMetadata is object: "
        f"{mesh.is_custom_component_of_type('pipelineMetadata', object)}"
    )
    print(
        "  filtered birthVertex is UINT (MeshInfo.DataType): "
        f"{filtered_mesh.is_vertex_custom_component_of_type('birthVertex', vcl.MeshInfo.DataType.UINT)}"
    )

    # Remove a component
    mesh.delete_vertex_custom_component("processingFlag")
    print("\nRemoved processingFlag component")
    print(
        "Has processingFlag component: "
        f"{mesh.has_vertex_custom_component('processingFlag')}"
    )

    print("\nExample completed successfully!")

    return mesh


if __name__ == "__main__":
    mesh = mesh_custom_components()

    print("\n=== Saving Meshes ===")
    try:
        os.makedirs(VCLIB_PYTHON_RESULTS_PATH, exist_ok=True)
        # Note: PLY exports primitive scalar custom components (e.g. quality, area, materialID)
        out_path = VCLIB_PYTHON_RESULTS_PATH + "/009_processed_bimba.ply"
        vcl.save_mesh(mesh, out_path)
        print(f"Saved: {out_path}")
    except Exception as e:
        print(f"Error in saving: {e}")
