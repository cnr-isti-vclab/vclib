# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

vclib_add_test(
    007-glfw-window-resize
    MODULE render
    SOURCES main.cpp
)

# This test requires an interactive desktop; opt in with
# VCLIB_RUN_GLFW_WINDOW_TESTS=1.
if(TEST 007-glfw-window-resize)
    set_tests_properties(007-glfw-window-resize PROPERTIES SKIP_RETURN_CODE 77)
endif()
