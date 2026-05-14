#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2026                                                    *
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
        add_subdirectory(950-pbr)
        add_subdirectory(984-mesh-viewer-imgui-test-split-shaders)
        add_subdirectory(979-selection)
    endif()

    add_subdirectory(999-misc)

    add_subdirectory(core)

    if (VCLIB_BUILD_MODULE_EXTERNAL)
        add_subdirectory(external)
    endif()
endif()
