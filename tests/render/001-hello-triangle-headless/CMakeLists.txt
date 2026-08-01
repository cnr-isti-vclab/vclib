# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(SHADERS
    shaders/vs_vertex_shader.sc
    shaders/fs_fragment_shader.sc
    shaders/varying.def.sc
)

vclib_add_test(
    001-hello-triangle-headless
    MODULE render
    SOURCES main.cpp
)

vclib_target_add_shaders(vclib-render-test-001-hello-triangle-headless ${SHADERS})
