#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2024                                                    *
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
    set(CAN_BUILD_VCLIB_EXAMPLES true)
endif()
if(TARGET vclib-3rd-glfw)
    if (NOT CAN_BUILD_VCLIB_EXAMPLES)
        add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_GLFW)
        set(CAN_BUILD_VCLIB_EXAMPLES true)
    endif()
endif()

add_subdirectory(common)

add_subdirectory(00-hello-triangle)
add_subdirectory(01-viewer)
add_subdirectory(02-mesh-viewer)
add_subdirectory(03-viewer-with-text)

if (TARGET vclib-processing)
    add_subdirectory(800-processing-main-window)
endif()

if(TARGET vclib-3rd-imgui AND TARGET vclib-3rd-glfw)
    add_subdirectory(910-viewer-imgui)
endif()

add_subdirectory(999-misc)

if (CAN_BUILD_VCLIB_EXAMPLES)
    # the following examples are the same of the ones in the core module,
    # but they add a viewer at the end in order to visualize the results
    add_subdirectory(9015-mesh-io-stl)
    add_subdirectory(9018-mesh-principal-curvature)
    add_subdirectory(9019-mesh-sampling)
    add_subdirectory(9021-mesh-sphere-intersection)
    add_subdirectory(9026-crease-edges-mesh)
endif()
