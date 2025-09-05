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
#*****************************************************************************

import vclib as vcl
import os
import math

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = current_file_path + "/../../assets/example_meshes"
VCLIB_RESULTS_PATH = current_file_path + "/../../assets/results"


def polymesh_specifics():
    print("=== VCLib Example 008: PolyMesh Specifics ===\n")

    # PART 1: CREATING A POLYMESH FROM SCRATCH
    print("PART 1: Creating a PolyMesh from Scratch\n----------------------------------------")
    poly_mesh = vcl.PolyMesh()
    poly_mesh.set_name("Hexagon Pyramid PolyMesh")

    # Add 6 vertices in a circle (hexagon base)
    for i in range(6):
        angle = i * 2.0 * math.pi / 6.0
        x = math.cos(angle)
        y = math.sin(angle)
        poly_mesh.add_vertex(vcl.Point3(x, y, 0.0))
    print(f"Added {poly_mesh.vertex_number()} vertices")

    # Create a hexagonal face
    fid = poly_mesh.add_face()
    hex_face = poly_mesh.face(fid)
    for i in range(6):
        hex_face.push_vertex(i)
    print(f"Created hexagonal face with {hex_face.vertex_number()} vertices")

    # Add a center vertex (shifted on z) and create triangular faces
    center_vertex = poly_mesh.add_vertex(vcl.Point3(0.0, 0.0, -1.0))
    for i in range(6):
        poly_mesh.add_face([center_vertex, (i + 1) % 6, i])
    print(f"Added {poly_mesh.face_number() - 1} triangular faces around the center")
    print(f"Total faces: {poly_mesh.face_number()}\n")

    # Convert to TriMesh (triangulates polygonal faces)
    tri_mesh = vcl.TriMesh()
    tri_mesh.import_from(poly_mesh)
    tri_mesh.set_name("Hexagon Pyramid TriMesh")
    print(f"Converted to TriMesh: {tri_mesh.vertex_number()} vertices, {tri_mesh.face_number()} faces\n")

    # PART 2: POLYMESH VS TRIMESH COMPARISON
    print("PART 2: PolyMesh vs TriMesh Comparison\n--------------------------------------")
    poly_mesh_loaded = vcl.PolyMesh()
    tri_mesh_loaded = vcl.TriMesh()
    vcl.load_mesh(poly_mesh_loaded, VCLIB_EXAMPLE_MESHES_PATH + "/cube_poly.ply")
    vcl.load_mesh(tri_mesh_loaded, VCLIB_EXAMPLE_MESHES_PATH + "/cube_poly.ply")
    print(f"Loaded cube as PolyMesh: {poly_mesh_loaded.vertex_number()} vertices, {poly_mesh_loaded.face_number()} faces")
    print(f"Loaded cube as TriMesh:  {tri_mesh_loaded.vertex_number()} vertices, {tri_mesh_loaded.face_number()} faces")

    # Analyze face structure
    poly_face_sizes = {}
    for face in poly_mesh_loaded.faces():
        size = face.vertex_number()
        poly_face_sizes[size] = poly_face_sizes.get(size, 0) + 1
    print("\nFace structure analysis:")
    print("PolyMesh face distribution:")
    for size, count in poly_face_sizes.items():
        print(f"  {count} faces with {size} vertices")
    print("TriMesh face distribution:")
    print(f"  {tri_mesh_loaded.face_number()} faces with 3 vertices each\n")

    # PART 3: DYNAMIC FACE MANIPULATION
    print("PART 3: Dynamic Face Manipulation\n----------------------------------")
    dynamic_mesh = vcl.PolyMesh()
    dynamic_mesh.set_name("Dynamic Square PolyMesh")
    for i in range(4):
        dynamic_mesh.add_vertex(vcl.Point3(-1.0 if i % 2 == 0 else 1.0, -1.0 if i < 2 else 1.0, 0.0))
    sfi = dynamic_mesh.add_face()
    square_face = dynamic_mesh.face(sfi)
    print(f"Created empty face, vertices: {square_face.vertex_number()}")
    square_face.push_vertex(0)
    print(f"After adding 1st vertex: {square_face.vertex_number()} vertices")
    square_face.push_vertex(1)
    square_face.push_vertex(3)
    square_face.push_vertex(2)
    print(f"After adding all vertices: {square_face.vertex_number()} vertices")
    square_face.clear_vertices()
    print(f"After clearing: {square_face.vertex_number()} vertices")
    square_face.set_vertices([0, 1, 3, 2])
    print(f"After set_vertices: {square_face.vertex_number()} vertices")

    # PART 4: ADJACENCY IN POLYMESH
    print("\n\nPART 4: Adjacency in PolyMesh\n-----------------------------")
    poly_mesh_loaded.enable_per_face_adjacent_faces()
    vcl.update_per_face_adjacent_faces(poly_mesh_loaded)
    print("Face-face adjacency enabled for PolyMesh")
    if poly_mesh_loaded.face_number() > 0:
        face = poly_mesh_loaded.face(0)
        print(f"Face 0 has {face.vertex_number()} vertices and {face.adj_faces_number()} adjacent faces")
        print("Adjacent faces: ", end="")
        for i in range(face.adj_faces_number()):
            adj_face = face.adj_face(i)
            if adj_face is not None:
                print(f"{adj_face.index()} ", end="")
            else:
                print("border ", end="")
        print("")
        print("Adjacency per edge:")
        for i in range(face.vertex_number()):
            adj_face = face.adj_face(i)
            print(f"  Edge {i} (v{face.vertex(i).index()}-v{face.vertex_mod(i + 1).index()}): ", end="")
            if adj_face is not None:
                print(f"face {adj_face.index()}")
            else:
                print("border")

    # PART 5: POLYGONAL FACE OPERATIONS
    print("\n\nPART 5: Polygonal Face Operations\n---------------------------------")
    if poly_mesh_loaded.face_number() > 0:
        face = poly_mesh_loaded.face(0)
        print("Face 0 analysis:")
        print(f"  Vertex count: {face.vertex_number()}")
        print("  Vertices: ", end="")
        for i in range(face.vertex_number()):
            print(f"{face.vertex(i).index()} ", end="")
        print("")
        centroid = vcl.Point3(0, 0, 0)
        for i in range(face.vertex_number()):
            centroid += face.vertex(i).position()
        centroid /= face.vertex_number()
        print(f"  Centroid: ({centroid.x()}, {centroid.y()}, {centroid.z()})")
        area = 0.0
        for i in range(1, face.vertex_number() - 1):
            v0 = face.vertex(0).position()
            v1 = face.vertex(i).position()
            v2 = face.vertex(i + 1).position()
            edge1 = v1 - v0
            edge2 = v2 - v0
            area += 0.5 * edge1.cross(edge2).norm()
        print(f"  Approximate area: {area}")

    # PART 6: CONSIDERATIONS
    print("\n\nPART 6: Considerations\n---------------------------------")
    print("Memory and performance considerations:")
    print("TriMesh characteristics:")
    print("  - Fixed 3 vertices per face")
    print("  - Predictable memory layout")
    print("  - Optimal for triangle-based algorithms")
    print("  - Standard format for graphics pipelines\n")
    print("PolyMesh characteristics:")
    print("  - Variable vertices per face")
    print("  - Dynamic memory allocation")
    print("  - Preserves original face structure")
    print("  - Better for CAD and architectural models\n")
    tri_mesh_face_memory = tri_mesh_loaded.face_number() * 3
    poly_mesh_face_memory = sum(face.vertex_number() for face in poly_mesh_loaded.faces())
    print("Storage comparison for cube:")
    print(f"  TriMesh face indices: {tri_mesh_face_memory}")
    print(f"  PolyMesh face indices: {poly_mesh_face_memory}")
    print(f"  Ratio: {tri_mesh_face_memory / poly_mesh_face_memory if poly_mesh_face_memory else 0}\n")

    # SUMMARY
    print("\n\n=== EXAMPLE SUMMARY ===\n")
    print("In this example we learned:\n")
    print("1. POLYMESH CREATION:")
    print("   - add_face() creates empty polygonal faces")
    print("   - push_vertex() adds vertices to faces dynamically")
    print("   - clear_vertices() and set_vertices() for face manipulation\n")
    print("2. TRIMESH VS POLYMESH:")
    print("   - TriMesh: fixed 3 vertices per face, optimized for triangles")
    print("   - PolyMesh: variable vertices per face, preserves original geometry")
    print("   - Different memory and performance characteristics\n")
    print("3. DYNAMIC FACE MANIPULATION:")
    print("   - Faces can be modified after creation")
    print("   - Vertices can be added/removed dynamically")
    print("   - Adjacency information updates automatically\n")
    print("4. POLYMESH-SPECIFIC FEATURES:")
    print("   - vertex_number() varies per face")
    print("   - adj_faces_number() matches vertex count")
    print("   - Face operations work with arbitrary polygon sizes\n")
    print("PolyMesh is ideal for:")
    print("- CAD and architectural models")
    print("- Preserving original geometric structure")
    print("- Working with quad-dominant or mixed meshes")
    print("- Applications requiring flexible face definitions\n")
    print("Example completed successfully!")

    return poly_mesh, tri_mesh, dynamic_mesh


if __name__ == "__main__":
    poly_mesh, tri_mesh, dynamic_mesh = polymesh_specifics()
    print("\n=== Saving Meshes ===")
    try:
        vcl.save_mesh(poly_mesh, VCLIB_RESULTS_PATH + "/008_hexagon_pyramid_polymesh.ply")
        print("Saved: 008_hexagon_pyramid_polymesh.ply")
        vcl.save_mesh(tri_mesh, VCLIB_RESULTS_PATH + "/008_hexagon_pyramid_trimesh.ply")
        print("Saved: 008_hexagon_pyramid_trimesh.ply")
        vcl.save_mesh(dynamic_mesh, VCLIB_RESULTS_PATH + "/008_square_polymesh.ply")
        print("Saved: 008_square_polymesh.ply")
        print(f"\nAll files have been saved to: {VCLIB_RESULTS_PATH}")
    except Exception as e:
        print(f"Error in saving: {e}")
