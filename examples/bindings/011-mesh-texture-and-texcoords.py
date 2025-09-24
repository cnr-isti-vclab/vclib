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

# This example demonstrates how to work with textures and texture coordinates:
# 1. Loading meshes with vertex texture coordinates
# 2. Loading meshes with wedge texture coordinates
# 3. Accessing and manipulating texture coordinates
# 4. Working with texture images and paths
# 5. Enabling texture coordinate components dynamically


def mesh_texture_and_texcoords():
    print("=== VCLib - Texture and TexCoords Example ===\n")

    # Example 1: Load mesh with vertex texture coordinates
    print("1. Loading mesh with vertex texture coordinates...")
    mesh_vertex_texcoords = vcl.TriMesh()

    # note: by default, the texture images are not loaded when loading a mesh
    # only the texture paths will be available
    # To load the texture images, you can set the 'load_texture_images' option of
    # the LoadSettings object to True, that can be passed to the load function.
    # example of loading also the textures shown below (wedges)
    vcl.load_mesh(mesh_vertex_texcoords, f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny_verttexcoords.ply")

    print(f"   Loaded {mesh_vertex_texcoords.vertex_number()} vertices, "
          f"{mesh_vertex_texcoords.face_number()} faces")

    # Check if mesh has vertex texture coordinates
    if mesh_vertex_texcoords.is_per_vertex_tex_coord_enabled():
        print("   ✓ Mesh has vertex texture coordinates")

        # Access first few vertex texture coordinates
        print("   First 3 vertex texture coordinates:")
        for i in range(min(3, mesh_vertex_texcoords.vertex_number())):
            tex_coord = mesh_vertex_texcoords.vertex(i).tex_coord()
            print(f"     Vertex {i}: ({tex_coord.u()}, {tex_coord.v()}) tex_id {tex_coord.index()}")
    else:
        print("   ✗ Mesh does not have vertex texture coordinates")

    # Example 2: Load mesh with wedge texture coordinates
    print("\n2. Loading mesh with wedge texture coordinates...")
    mesh_wedge_texcoords = vcl.TriMesh()

    vcl.load_mesh(mesh_wedge_texcoords, f"{VCLIB_EXAMPLE_MESHES_PATH}/bunny_textured.ply", load_texture_images = True)

    print(f"   Loaded {mesh_wedge_texcoords.vertex_number()} vertices, "
          f"{mesh_wedge_texcoords.face_number()} faces")

    # Check if mesh has wedge texture coordinates
    if mesh_wedge_texcoords.is_per_face_wedge_tex_coords_enabled():
        print("   ✓ Mesh has wedge texture coordinates")

        # Access first few face wedge texture coordinates
        print("   First face wedge texture coordinates:")
        if mesh_wedge_texcoords.face_number() > 0:
            face = mesh_wedge_texcoords.face(0)
            for i in range(face.vertex_number()):
                tex_coord = face.wedge_tex_coord(i)
                print(f"     Wedge {i}: ({tex_coord.u()}, {tex_coord.v()}) tex_id {face.texture_index()}")
    else:
        print("   ✗ Mesh does not have wedge texture coordinates")

    # Example 3: Working with texture images
    print("\n3. Working with texture images...")
    print(f"   ✓ Mesh has {mesh_wedge_texcoords.texture_number()} textures")

    for i in range(mesh_wedge_texcoords.texture_number()):
        # if the images are not loaded, the texture paths will be available
        # and the image will be empty (width and height will be 0)
        texture = mesh_wedge_texcoords.texture(i)
        print(f"     Texture {i}: {texture.path()}")
        print(f"       Size: {texture.image().width()}x{texture.image().height()}")

    # Example 4: Creating mesh with texture coordinates
    print("\n4. Creating mesh with texture coordinates...")
    custom_mesh = vcl.TriMesh()
    custom_mesh.set_name("Custom Textured Mesh")

    # Enable texture coordinate components
    custom_mesh.enable_per_vertex_tex_coord()
    custom_mesh.enable_per_face_wedge_tex_coords()

    # Add vertices with texture coordinates
    custom_mesh.add_vertices(4)
    custom_mesh.vertex(0).set_position([0.0, 0.0, 0.0])
    custom_mesh.vertex(0).set_tex_coord([0.0, 0.0])
    custom_mesh.vertex(1).set_position([1.0, 0.0, 0.0])
    custom_mesh.vertex(1).set_tex_coord([1.0, 0.0])
    custom_mesh.vertex(2).set_position([1.0, 1.0, 0.0])
    custom_mesh.vertex(2).set_tex_coord([1.0, 1.0])
    custom_mesh.vertex(3).set_position([0.0, 1.0, 0.0])
    custom_mesh.vertex(3).set_tex_coord([0.0, 1.0])

    # Add faces with wedge texture coordinates
    custom_mesh.add_faces(2)
    custom_mesh.face(0).set_vertices([0, 1, 2])
    custom_mesh.face(0).set_wedge_tex_coord(0, [0.0, 0.0])
    custom_mesh.face(0).set_wedge_tex_coord(1, [1.0, 0.0])
    custom_mesh.face(0).set_wedge_tex_coord(2, [1.0, 1.0])

    custom_mesh.face(1).set_vertices([0, 2, 3])
    custom_mesh.face(1).set_wedge_tex_coord(0, [0.0, 0.0])
    custom_mesh.face(1).set_wedge_tex_coord(1, [1.0, 1.0])
    custom_mesh.face(1).set_wedge_tex_coord(2, [0.0, 1.0])

    custom_mesh.push_texture(mesh_wedge_texcoords.texture(0))

    # change the path of the texture to a custom one (will be relative to the
    # mesh file path when saving)
    custom_mesh.texture(0).set_path("custom_texture_path.png")  # just an example path

    print(f"   Created custom mesh with {custom_mesh.vertex_number()} vertices "
          f"and {custom_mesh.face_number()} faces")
    print(f"   ✓ Has vertex texture coordinates: {custom_mesh.is_per_vertex_tex_coord_enabled()}")
    print(f"   ✓ Has wedge texture coordinates: {custom_mesh.is_per_face_wedge_tex_coords_enabled()}")

    # Example 5: Iterating through texture coordinates using views
    print("\n5. Iterating through texture coordinates...")
    if mesh_vertex_texcoords.is_per_vertex_tex_coord_enabled():
        print("   Vertex texture coordinates range:")
        min_tex_coord = vcl.Point2(float('inf'), float('inf'))
        max_tex_coord = vcl.Point2(float('-inf'), float('-inf'))

        # Iterate through vertices to find min/max texture coordinates
        for vertex in mesh_vertex_texcoords.vertices():
            tc = vertex.tex_coord()
            min_tex_coord = vcl.Point2(
                min(min_tex_coord.x(), tc.u()),
                min(min_tex_coord.y(), tc.v())
            )
            max_tex_coord = vcl.Point2(
                max(max_tex_coord.x(), tc.u()),
                max(max_tex_coord.y(), tc.v())
            )

        print(f"     Min: ({min_tex_coord.x()}, {min_tex_coord.y()})")
        print(f"     Max: ({max_tex_coord.x()}, {max_tex_coord.y()})")

    print("\n=== Example completed ===")
    return custom_mesh


def main():
    try:
        custom_mesh = mesh_texture_and_texcoords()

        # Save the created meshes
        print("\n=== Saving Meshes ===")

        vcl.save_mesh(custom_mesh, f"{VCLIB_RESULTS_PATH}/011_custom_textured.ply", save_texture_images = True)

        print(f"\nAll files have been saved to: {VCLIB_RESULTS_PATH}\n")

    except Exception as e:
        print(f"Error in main: {e}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    exit(main())
