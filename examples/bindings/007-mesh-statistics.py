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
import numpy as np

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = current_file_path + "/../../assets/example_meshes"


def mesh_statistics():
    print("=== VCLib Example 007: Mesh Statistics ===\n")

    # PART 1: BASIC GEOMETRIC PROPERTIES
    print("PART 1: Basic Geometric Properties\n-----------------------------------")
    mesh = vcl.load_tri_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/bimba_simplified.obj")
    print(f"Loaded mesh: {mesh.vertex_number()} vertices, {mesh.face_number()} faces\n")
    vcl.update_bounding_box(mesh)
    bbox = mesh.bounding_box()
    print(f"Bounding Box:\n  Min: {bbox.min()}\n  Max: {bbox.max()}\n  Size: {bbox.size()}\n  Diagonal: {bbox.diagonal()}\n")

    # PART 2: BARYCENTER CALCULATIONS
    print("PART 2: Barycenter Calculations\n--------------------------------")
    geom_barycenter = vcl.barycenter(mesh)
    print(f"Geometric barycenter: {geom_barycenter}")
    shell_barycenter = vcl.shell_barycenter(mesh)
    print(f"Shell barycenter: {shell_barycenter}")
    barycenter_diff = (geom_barycenter - shell_barycenter).norm()
    print(f"Distance between barycenters: {barycenter_diff}")
    if barycenter_diff < 1e-6:
        print("  -> Barycenters are very close (uniform distribution)")
    else:
        print("  -> Barycenters differ (non-uniform distribution)")

    # PART 3: SURFACE AREA AND VOLUME
    print("\n\nPART 3: Surface Area and Volume\n-------------------------------")
    surface_area = vcl.surface_area(mesh)
    print(f"Surface area: {surface_area}")
    volume = vcl.volume(mesh)
    print(f"Volume: {volume}")
    if volume > 1e-10:
        surface_to_volume_ratio = surface_area / volume
        print(f"Surface area to volume ratio: {surface_to_volume_ratio}")
        sphere_radius = (3.0 * volume / (4.0 * math.pi)) ** (1.0 / 3.0)
        sphere_surface_area = 4.0 * math.pi * sphere_radius ** 2
        compactness = sphere_surface_area / surface_area
        print(f"Compactness (sphere/mesh surface ratio): {compactness}")
        if compactness > 0.8:
            print("  -> Shape is relatively compact (sphere-like)")
        else:
            print("  -> Shape has complex surface (elongated or detailed)")

    # PART 4: EDGE AND FACE STATISTICS
    print("\n\nPART 4: Edge and Face Statistics\n--------------------------------")
    edge_lengths = []
    for face in mesh.faces():
        for i in range(3):
            v1 = face.vertex(i).position()
            v2 = face.vertex((i + 1) % 3).position()
            edge_lengths.append((v2 - v1).norm())
    edge_lengths.sort()
    min_edge = edge_lengths[0]
    max_edge = edge_lengths[-1]
    avg_edge = sum(edge_lengths) / len(edge_lengths)
    median_edge = edge_lengths[len(edge_lengths) // 2]
    print(f"Edge length statistics:\n  Minimum: {min_edge}\n  Maximum: {max_edge}\n  Average: {avg_edge}\n  Median: {median_edge}\n  Ratio (max/min): {max_edge / min_edge}")

    face_areas = []
    for face in mesh.faces():
        v0 = face.vertex(0).position()
        v1 = face.vertex(1).position()
        v2 = face.vertex(2).position()
        edge1 = v1 - v0
        edge2 = v2 - v0
        area = 0.5 * edge1.cross(edge2).norm()
        face_areas.append(area)
    face_areas.sort()
    min_area = face_areas[0]
    max_area = face_areas[-1]
    avg_area = sum(face_areas) / len(face_areas)
    median_area = face_areas[len(face_areas) // 2]
    print(f"\nFace area statistics:\n  Minimum: {min_area}\n  Maximum: {max_area}\n  Average: {avg_area}\n  Median: {median_area}\n  Ratio (max/min): {max_area / min_area}")

    # PART 5: MESH QUALITY ANALYSIS
    print("\n\nPART 5: Mesh Quality Analysis\n-----------------------------")
    aspect_ratios = []
    degenerate_triangles = 0
    for face in mesh.faces():
        v0 = face.vertex(0).position()
        v1 = face.vertex(1).position()
        v2 = face.vertex(2).position()
        a = (v1 - v0).norm()
        b = (v2 - v1).norm()
        c = (v0 - v2).norm()
        if a < 1e-10 or b < 1e-10 or c < 1e-10:
            degenerate_triangles += 1
            continue
        max_side = max(a, b, c)
        min_side = min(a, b, c)
        aspect_ratios.append(max_side / min_side)
    if aspect_ratios:
        aspect_ratios.sort()
        min_aspect = aspect_ratios[0]
        max_aspect = aspect_ratios[-1]
        avg_aspect = sum(aspect_ratios) / len(aspect_ratios)
        good_triangles = len([ar for ar in aspect_ratios if ar < 2.0])
        print(f"Triangle aspect ratio statistics:\n  Minimum: {min_aspect} (1.0 = equilateral)\n  Maximum: {max_aspect}\n  Average: {avg_aspect}\n  Good triangles (aspect < 2.0): {good_triangles} ({100.0 * good_triangles / len(aspect_ratios)}%)")
    if degenerate_triangles > 0:
        print(f"  Degenerate triangles found: {degenerate_triangles}")

    # PART 6: COMPARISON WITH DIFFERENT MESH
    print("\n\nPART 6: Comparison with Different Mesh\n--------------------------------------")
    mesh2 = vcl.load_tri_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/cube_tri.ply")
    print(f"Comparison mesh (cube): {mesh2.vertex_number()} vertices, {mesh2.face_number()} faces")
    surface_area2 = vcl.surface_area(mesh2)
    barycenter2 = vcl.barycenter(mesh2)
    print(f"Cube surface area: {surface_area2}")
    print(f"Cube barycenter: {barycenter2}")
    volume2 = vcl.volume(mesh2)
    print(f"Cube volume: {volume2}")
    vcl.update_bounding_box(mesh2)
    cube_size = mesh2.bounding_box().max() - mesh2.bounding_box().min()
    expected_volume = cube_size.x() * cube_size.y() * cube_size.z()
    print(f"Expected cube volume: {expected_volume}")
    print(f"Volume calculation accuracy: {100.0 * (1.0 - abs(volume2 - expected_volume) / expected_volume)}%")

    # SUMMARY
    print("\n\n=== EXAMPLE SUMMARY ===\n")
    print("In this example we learned:\n")
    print("1. GEOMETRIC PROPERTIES:\n   - Bounding box calculation and analysis\n   - barycenter(): geometric center (vertex average)\n   - shellBarycenter(): surface-weighted center\n")
    print("2. VOLUMETRIC MEASUREMENTS:\n   - surfaceArea(): total surface area\n   - volume(): enclosed volume (closed meshes)\n   - Compactness and shape analysis\n")
    print("3. STATISTICAL ANALYSIS:\n   - Edge length distributions\n   - Face area distributions\n   - Triangle quality metrics (aspect ratios)\n")
    print("4. MESH QUALITY ASSESSMENT:\n   - Detection of degenerate triangles\n   - Aspect ratio analysis for triangle quality\n   - Statistical measures for mesh uniformity\n")
    print("These statistics are essential for:\n- Mesh quality assessment and validation\n- Geometric analysis and comparison\n- Preprocessing for simulation and rendering\n- Shape analysis and classification\n")
    print("Example completed successfully!")


if __name__ == "__main__":
    mesh_statistics()
