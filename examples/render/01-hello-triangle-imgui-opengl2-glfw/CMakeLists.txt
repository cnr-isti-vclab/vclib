# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(EXAMPLE_NAME 01-hello-triangle-imgui-opengl2-glfw)
project(vclib-render-example-${EXAMPLE_NAME})

set(HEADERS
    ../01-hello-triangle-imgui-opengl2-common/common.h
    ../01-hello-triangle-imgui-opengl2-common/hello_triangle_drawer.h
    ../01-hello-triangle-imgui-common/demo_imgui_drawer.h
)

set(SOURCES main.cpp)

vclib_add_example(${EXAMPLE_NAME}
    MODULE render
    SOURCES ${HEADERS} ${SOURCES}
)
