# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-render-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

if (TARGET vclib-3rd-qt)
    add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_QT)
    set(CAN_BUILD_VCLIB_RENDER_EXAMPLES true)
endif()
if(TARGET vclib-3rd-glfw)
    if (NOT CAN_BUILD_VCLIB_RENDER_EXAMPLES)
        add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_GLFW)
        set(CAN_BUILD_VCLIB_RENDER_EXAMPLES true)
    endif()
endif()

if (CAN_BUILD_VCLIB_RENDER_EXAMPLES)
    add_subdirectory(common)

    add_subdirectory(00-hello-triangle)
    add_subdirectory(01-viewer)
    add_subdirectory(02-mesh-viewer)
    add_subdirectory(03-viewer-with-text)
    add_subdirectory(04-hello-triangle-imgui)
    add_subdirectory(05-two-window-viewers)
    add_subdirectory(06-viewer-imgui)
    add_subdirectory(07-test-texcoords)
    add_subdirectory(08-test-gltf)

    add_subdirectory(801-camera-viewer)
    add_subdirectory(808-test-lines)

    if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
        add_subdirectory(09-drawable-points)
        add_subdirectory(800-screenspace)
        add_subdirectory(950-pbr)
    endif()

    add_subdirectory(999-misc)

    add_subdirectory(core)

    if (VCLIB_BUILD_MODULE_EXTERNAL)
        add_subdirectory(external)
    endif()
endif()
