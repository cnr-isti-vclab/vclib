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

def mesh_creation():
    print("=== VCLib Example 003: Mesh Creation ===\n")

    # /****** Creating Basic Primitives with TriMesh ******/

    print("=== Creating Basic Primitives (TriMesh) ===")

    # 1. Tetrahedron - The simplest 3D primitive
    print("Creating tetrahedron...")
    tetrahedron = vcl.create_tetrahedron_tri_mesh()
    print(f"  Vertices: {tetrahedron.vertex_number()}, Faces: {tetrahedron.face_number()}")
    tetrahedron.set_name("Tetrahedron")

    # 2. Hexahedron (Cube) - Box primitive
    print("Creating hexahedron (cube)...")
    cube = vcl.TriMesh()
    vcl.create_hexahedron(cube) # alternative to vcl.create_hexahedron_tri_mesh()
    print(f"  Vertices: {cube.vertex_number()}, Faces: {cube.face_number()}")
    cube.set_name("Cube")

    # 3. Dodecahedron - More complex polyhedron (triangulated)
    print("Creating dodecahedron (triangulated)...")
    dodecahedron = vcl.create_dodecahedron_tri_mesh()
    print(f"  Vertices: {dodecahedron.vertex_number()}, Faces: {dodecahedron.face_number()}")
    dodecahedron.set_name("Dodecahedron")

    # /****** Creating Parametric Primitives ******/

    print("\n=== Creating Parametric Primitives ===")

    # Custom cube with specific dimensions
    print("Creating custom cube...")
    min_corner = vcl.Point3(-2, -2, -2)
    edge_length = 4.0
    custom_cube = vcl.create_cube_tri_mesh(min_corner, edge_length)
    print(f"  Custom cube (4x4x4) - Vertices: {custom_cube.vertex_number()}, Faces: {custom_cube.face_number()}")
    custom_cube.set_name("Custom Cube")

    # /****** Creating Primitives with PolyMesh ******/

    print("\n=== Creating Primitives with PolyMesh ===")

    # PolyMesh can handle polygonal faces (not just triangles)

    # 1. Hexahedron as quads
    print("Creating hexahedron (PolyMesh - quads)...")
    cube_quads = vcl.create_hexahedron_poly_mesh()
    print(f"  Cube (quads) - Vertices: {cube_quads.vertex_number()}, Faces: {cube_quads.face_number()}")
    cube_quads.set_name("Cube (PolyMesh)")
    # Note: This creates 6 quad faces instead of 12 triangular faces

    # 2. Dodecahedron as pentagons
    print("Creating dodecahedron (PolyMesh - pentagons)...")
    dodecahedron_poly = vcl.create_dodecahedron_poly_mesh()
    print(f"  Dodecahedron (pentagons) - Vertices: {dodecahedron_poly.vertex_number()}, Faces: {dodecahedron_poly.face_number()}")
    dodecahedron_poly.set_name("Dodecahedron (PolyMesh)")
    # Note: This creates 12 pentagonal faces instead of many triangular faces

    # 3. Sphere with PolyMesh (quad faces)
    print("Creating sphere (PolyMesh - quads)...")
    sphere_center = vcl.Point3(0, 0, 0)
    sphere_radius = 1.0
    mode = vcl.SPHERIFIED_CUBE
    sphere_quads = vcl.create_sphere_poly_mesh(sphere_center, sphere_radius, mode, divisions=20)
    print(f"  Sphere (quads) - Vertices: {sphere_quads.vertex_number()}, Faces: {sphere_quads.face_number()}")
    sphere_quads.set_name("Sphere (PolyMesh)")

    # /****** Summary and Comparison ******/

    print("\n=== Summary: TriMesh vs PolyMesh ===")
    print("TriMesh:")
    print("  - All faces are triangles")
    print("  - More faces for complex shapes")
    print("  - Compatible with most graphics pipelines")
    print(f"  - Cube: {cube.face_number()} triangular faces")
    print(f"  - Dodecahedron: {dodecahedron.face_number()} triangular faces")

    print("\nPolyMesh:")
    print("  - Faces can have arbitrary number of vertices")
    print("  - More compact representation for regular shapes")
    print("  - Better preserves original geometry intent")
    print(f"  - Cube: {cube_quads.face_number()} quad faces")
    print(f"  - Dodecahedron: {dodecahedron_poly.face_number()} pentagonal faces")

    return (tetrahedron, cube, dodecahedron, custom_cube, cube_quads, 
            dodecahedron_poly, sphere_quads)


if __name__ == "__main__":
    meshes = mesh_creation()
    
    tetrahedron, cube, dodecahedron, custom_cube, cube_quads, dodecahedron_poly, sphere_quads = meshes

    # /****** Save the created meshes ******/

    # Configure which mesh data to save

    save_info = vcl.MeshInfo()
    save_info.set_vertices()
    save_info.set_faces()
    save_info.set_per_vertex_position(True, vcl.MeshInfo.FLOAT)
    save_info.set_per_face_vertex_references()
    
    print("\n=== Saving Meshes ===")
    
    try:
        vcl.save_mesh(tetrahedron, VCLIB_RESULTS_PATH + "/003_tetrahedron.ply", info = save_info)
        vcl.save_mesh(cube, VCLIB_RESULTS_PATH + "/003_cube_tri.ply", info = save_info)
        vcl.save_mesh(dodecahedron, VCLIB_RESULTS_PATH + "/003_dodecahedron_tri.ply", info = save_info)
        vcl.save_mesh(custom_cube, VCLIB_RESULTS_PATH + "/003_custom_cube.ply", info = save_info)
        vcl.save_mesh(cube_quads, VCLIB_RESULTS_PATH + "/003_cube_poly.ply", info = save_info)
        vcl.save_mesh(dodecahedron_poly, VCLIB_RESULTS_PATH + "/003_dodecahedron_poly.ply", info = save_info)
        vcl.save_mesh(sphere_quads, VCLIB_RESULTS_PATH + "/003_sphere_poly.ply", info = save_info)

        print("\nAll meshes have been saved to the results directory.")
    except Exception as e:
        print(f"Error in saving: {e}")

    print("Example completed successfully!")
