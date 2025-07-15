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

cmake_minimum_required(VERSION 3.24)
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

    add_subdirectory(801-camera-viewer)

    if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
        add_subdirectory(984-mesh-viewer-imgui-test-split-shaders)
    endif()

    add_subdirectory(999-misc)

    # the following examples are the same of the ones in the core module,
    # but they add a viewer at the end in order to visualize the results
    add_subdirectory(9000-mesh-io)
    add_subdirectory(9001-mesh-basics)
    add_subdirectory(9003-mesh-creation)
    add_subdirectory(9004-mesh-copy-and-transform)
    add_subdirectory(9008-polymesh-specifics)
    add_subdirectory(9009-mesh-custom-components)
    add_subdirectory(9010-mesh-matrix-conversion)
    add_subdirectory(9011-mesh-texture-and-texcoords)
    add_subdirectory(9013-mesh-clean)
    add_subdirectory(9014-mesh-smoothing)
    add_subdirectory(9015-mesh-curvature)
    add_subdirectory(9016-face-flooding)
    add_subdirectory(9017-mesh-convex-hull)
    add_subdirectory(9019-mesh-point-sampling)
    add_subdirectory(9020-mesh-crease-edges)

    add_subdirectory(9821-mesh-sphere-intersection)
endif()
