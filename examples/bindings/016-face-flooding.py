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

# This example demonstrates how to flood a mesh starting from a seed face
# using a custom selection criterion. It uses the face flooding algorithm
# to find all connected faces that satisfy the selection condition, using a
# depth-first search approach.


def face_flooding():
    print("=== VCLib Example 016: Face Flooding ===\n")

    # Load mesh
    print("=== Loading Bunny Mesh ===")
    
    m = vcl.TriMesh()
    vcl.load_mesh(m, f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny.obj")

    # Enable required components
    m.enable_per_face_adjacent_faces()
    m.enable_per_face_color()

    # Update topology and normals
    vcl.update_per_face_adjacent_faces(m)
    vcl.update_per_vertex_and_face_normals(m)
    vcl.update_bounding_box(m)

    # Initialize face colors
    vcl.set_per_face_color(m, vcl.Color.LIGHT_GRAY)

    print(f"Mesh loaded: {m.face_number()} faces")

    # Custom flooding function: select faces with normal pointing upward
    def upward_facing_selector(f):
        return f.normal().y() > 0.5  # faces pointing significantly upward

    # Find a seed face that satisfies our condition
    seed_face_id = None
    for f in m.faces():
        if upward_facing_selector(f):
            seed_face_id = f.index()
            break

    if seed_face_id is None:
        print("No suitable seed face found!")
        return m

    # Mark seed face in red
    m.face(seed_face_id).set_color(vcl.Color.RED)
    print(f"Seed face: {seed_face_id}")

    # Perform flooding from seed face
    print("\n=== Performing Face Flooding ===")
    flooded_faces = vcl.flood_face_patch(m.face(seed_face_id), upward_facing_selector)

    # Color flooded faces in green
    for f_ptr in flooded_faces:
        if f_ptr.index() != seed_face_id:
            m.face(f_ptr.index()).set_color(vcl.Color.GREEN)

    print(f"Flooded {len(flooded_faces)} faces")
    
    print("\n=== Flooding Complete ===")
    print("Color coding:")
    print("  - Red face: seed for flooding")
    print("  - Green faces: upward-facing connected faces")
    print("  - Gray faces: other faces")

    return m


def main():
    try:
        m = face_flooding()

        # Save the result
        print("\n=== Saving Mesh ===")

        # Create results directory if it doesn't exist
        os.makedirs(VCLIB_RESULTS_PATH, exist_ok=True)

        vcl.save_mesh(m, f"{VCLIB_RESULTS_PATH}/016_flooded_faces.ply")

        print(f"\nFile saved to: {VCLIB_RESULTS_PATH}/016_flooded_faces.ply")
        print("- Red face: seed for flooding")
        print("- Green faces: upward-facing connected faces")
        print("- Gray faces: other faces")

        print("\n=== Example 016 completed successfully! ===")

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
