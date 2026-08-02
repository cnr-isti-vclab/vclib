# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

import os

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = os.path.normpath(os.path.join(current_file_path, "../../assets/example_meshes"))
VCLIB_PYTHON_RESULTS_PATH = os.path.normpath(os.path.join(current_file_path, "../../assets/results/python"))
