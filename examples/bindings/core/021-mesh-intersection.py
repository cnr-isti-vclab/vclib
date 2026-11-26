# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl
import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from paths import VCLIB_EXAMPLE_MESHES_PATH, VCLIB_PYTHON_RESULTS_PATH

# This example demonstrates mesh intersections with spheres and planes:
# 1. Mesh-sphere intersection using different sphere sizes and positions
# 2. Mesh-plane intersection producing an EdgeMesh section contour


def mesh_intersection():
    print("=== VCLib Example 021: Mesh Intersection ===\n")

    # /****** Load base mesh ******/

    print("=== Loading Base Mesh ===")

    original_mesh = vcl.load_tri_mesh(f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny.obj")
    original_mesh.set_name("Original Bunny")

    vcl.update_face_normals(original_mesh)
    vcl.update_vertex_normals(original_mesh)

    print(
        f"Loaded bunny mesh: {original_mesh.vertex_count()} vertices, "
        f"{original_mesh.face_count()} faces"
    )

    # /****** Define spheres for intersection ******/

    print("\n=== Defining Intersection Spheres ===")

    # Center sphere that intersects the bunny's body
    center_sphere = vcl.Sphere(vcl.Point3(0.28, 0.18, 0.49), 0.08)

    # Smaller sphere that intersects only part of the ears
    ear_sphere = vcl.Sphere(vcl.Point3(0.25, 0.47, 0.27), 0.04)

    # Larger sphere that encompasses most of the mesh
    large_sphere = vcl.Sphere(vcl.Point3(0.28, 0.18, 0.49), 0.15)

    print(
        f"Center sphere: radius {center_sphere.radius()} at ("
        f"{center_sphere.center().x()}, {center_sphere.center().y()}, "
        f"{center_sphere.center().z()})"
    )
    print(
        f"Ear sphere: radius {ear_sphere.radius()} at ("
        f"{ear_sphere.center().x()}, {ear_sphere.center().y()}, "
        f"{ear_sphere.center().z()})"
    )
    print(
        f"Large sphere: radius {large_sphere.radius()} at ("
        f"{large_sphere.center().x()}, {large_sphere.center().y()}, "
        f"{large_sphere.center().z()})"
    )

    # /****** Compute intersections ******/

    print("\n=== Computing Intersections ===")

    # Center intersection
    center_sphere_intersection = vcl.intersection(original_mesh, center_sphere)
    center_sphere_intersection.set_name("Bunny-Center Sphere Intersection")

    # Ear intersection
    ear_sphere_intersection = vcl.intersection(original_mesh, ear_sphere)
    ear_sphere_intersection.set_name("Bunny-Ear Sphere Intersection")

    # Large intersection
    large_sphere_intersection = vcl.intersection(original_mesh, large_sphere)
    large_sphere_intersection.set_name("Bunny-Large Sphere Intersection")

    print(
        f"Center sphere intersection: "
        f"{center_sphere_intersection.vertex_count()} vertices, "
        f"{center_sphere_intersection.face_count()} faces"
    )
    print(
        f"Ear sphere intersection: "
        f"{ear_sphere_intersection.vertex_count()} vertices, "
        f"{ear_sphere_intersection.face_count()} faces"
    )
    print(
        f"Large sphere intersection: "
        f"{large_sphere_intersection.vertex_count()} vertices, "
        f"{large_sphere_intersection.face_count()} faces"
    )

    # /****** Optional: Create sphere meshes for visualization ******/

    center_sphere_mesh = vcl.create_sphere_tri_mesh(center_sphere)
    center_sphere_mesh.set_name("Center Sphere")

    ear_sphere_mesh = vcl.create_sphere_tri_mesh(ear_sphere)
    ear_sphere_mesh.set_name("Ear Sphere")

    large_sphere_mesh = vcl.create_sphere_tri_mesh(large_sphere)
    large_sphere_mesh.set_name("Large Sphere")

    # /****** Mesh-plane intersection ******/

    print("\n=== Mesh-Plane Intersection ===")

    cut_plane = vcl.Plane(
        vcl.Point3(0.28, 0.18, 0.49), vcl.Point3(0.0, 1.0, 0.0)
    )

    plane_intersection = vcl.intersection(original_mesh, cut_plane)
    plane_intersection.set_name("Bunny-Plane Intersection")

    print(
        f"Plane intersection: {plane_intersection.vertex_count()} vertices, "
        f"{plane_intersection.edge_count()} edges"
    )

    return (
        original_mesh,
        center_sphere_mesh,
        center_sphere_intersection,
        ear_sphere_mesh,
        ear_sphere_intersection,
        large_sphere_mesh,
        large_sphere_intersection,
        plane_intersection,
    )


def main():
    try:
        (
            original_mesh,
            center_sphere_mesh,
            center_sphere_intersection,
            ear_sphere_mesh,
            ear_sphere_intersection,
            large_sphere_mesh,
            large_sphere_intersection,
            plane_intersection,
        ) = mesh_intersection()

        # /****** Save the created meshes ******/

        print("\n=== Saving Meshes ===")

        os.makedirs(VCLIB_PYTHON_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(
            original_mesh, f"{VCLIB_PYTHON_RESULTS_PATH}/021_original_bunny.ply"
        )
        vcl.save_mesh(
            center_sphere_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/021_center_sphere.ply",
            binary=False,
        )
        vcl.save_mesh(
            center_sphere_intersection,
            f"{VCLIB_PYTHON_RESULTS_PATH}/021_center_sphere_intersection.ply",
            binary=False,
        )
        vcl.save_mesh(
            ear_sphere_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/021_ear_sphere.ply",
            binary=False,
        )
        vcl.save_mesh(
            ear_sphere_intersection,
            f"{VCLIB_PYTHON_RESULTS_PATH}/021_ear_sphere_intersection.ply",
            binary=False,
        )
        vcl.save_mesh(
            large_sphere_mesh,
            f"{VCLIB_PYTHON_RESULTS_PATH}/021_large_sphere.ply",
            binary=False,
        )
        vcl.save_mesh(
            large_sphere_intersection,
            f"{VCLIB_PYTHON_RESULTS_PATH}/021_large_sphere_intersection.ply",
            binary=False,
        )
        vcl.save_mesh(
            plane_intersection,
            f"{VCLIB_PYTHON_RESULTS_PATH}/021_plane_intersection.ply",
            binary=False,
        )

        print(f"\nAll files have been saved to: {VCLIB_PYTHON_RESULTS_PATH}")

    except Exception as e:
        print(f"Error saving files: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
