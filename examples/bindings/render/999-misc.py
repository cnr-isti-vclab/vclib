# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl

import numpy as np
import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from paths import VCLIB_EXAMPLE_MESHES_PATH, VCLIB_PYTHON_RESULTS_PATH

if __name__ == "__main__":
    print("=== VCLib Example 999: Miscellaneous ===\n")

    app = vcl.Application(sys.argv)

    m = vcl.load_poly_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/spot/spot_quadrangulated.obj")

    viewer = vcl.MeshViewer()

    dm = vcl.make_drawable(m)

    viewer.push_drawable_object(dm)

    vcl.update_per_vertex_and_face_normals(dm)

    settings = dm.render_settings

    # Set points to visible and change their size
    settings.set_points(vcl.MeshRenderInfo.Points.VISIBLE)
    settings.set_point_width(5.0)

    # Set wireframe to visible, change its size and color (magenta)
    settings.set_wireframe(vcl.MeshRenderInfo.Wireframe.VISIBLE)
    settings.set_wireframe_width(2)
    settings.set_wireframe_user_color(1.0, 0.0, 1.0, 1.0)

    dm.render_settings = settings

    dm.update_buffers()

    # need to call this to update the render settings gui after modification   
    viewer.update_gui()

    print(f"Number of drawable objects: {len(viewer)}")
    for obj in viewer:
        print(f"Object: {obj.name()}")

    viewer.fit_scene()
    viewer.show_maximized()

    ret = app.exec()

    # Explicitly delete viewer before QApplication to avoid annoying warning messages
    del viewer

    sys.exit(ret)
