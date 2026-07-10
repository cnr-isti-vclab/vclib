# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl
import os
import numpy as np

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = current_file_path + "/../../../assets/example_meshes"
VCLIB_PYTHON_RESULTS_PATH = current_file_path + "/../../../assets/results/python"

if __name__ == "__main__":
    print("=== VCLib Example 999: Miscellaneous ===\n")

    t = vcl.TriMesh()

    t.add_vertices(3)

    # create a numpy array of 3x4 floats between 0 and 1
    colors_1 = np.random.rand(3, 4).astype(np.float64)

    print(colors_1)

    vcl.vertex_colors_from_matrix(t, colors_1)

    for v in t.vertices():
        print(f"Vertex {v.index()} color: {v.color()}")

    # create a numpy array of 3x4 integers between 0 and 255
    colors_2 = np.random.randint(0, 256, (3, 4), dtype=np.uint8)

    print(colors_2)

    vcl.vertex_colors_from_matrix(t, colors_2)

    for v in t.vertices():
        print(f"Vertex {v.index()} color: {v.color()}")

    t.add_faces(4)

    t.enable_per_face_adjacent_faces()

    for f in t.faces():
        for af in f.adj_face_indices():
            if af != vcl.UINT_NULL:
                print(f"Face {f.index()} adjacent face: {af}")
            else:
                print(f"Face {f.index()} adjacent face: border")
