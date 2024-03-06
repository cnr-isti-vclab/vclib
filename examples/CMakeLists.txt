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

option(VCLIB_RENDER_COMPILE_WARNINGS_AS_ERRORS "Compile warnings as errors" ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_RENDER_COMPILE_WARNINGS_AS_ERRORS})

set(CAN_BUILD_EXAMPLES false)

option(VCLIB_RENDER_BUILD_EXAMPLES_WITH_QT_AND_BGFX "Force building examples using qt and bgfx libraries" ON)
option(VCLIB_RENDER_BUILD_EXAMPLES_WITH_GLFW_AND_BGFX "Force building examples using glfw and bgfx libraries" ON)
option(VCLIB_RENDER_BUILD_EXAMPLES_WITH_QGLVIEWER "Force building examples using qglviewer library" ON)

if (TARGET vclib-external-qt)
    if (TARGET vclib-external-bgfx)
        if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_QT_AND_BGFX)
            add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_QT)
            add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_QT_AND_BGFX)
            set(CAN_BUILD_EXAMPLES true)
        endif()
    elseif(TARGET vclib-external-qglviewer)
        if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_QGLVIEWER)
            add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_QT)
            add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER)
            set(CAN_BUILD_EXAMPLES true)
        endif()
    endif()
endif()
if(TARGET vclib-external-glfw AND TARGET vclib-external-bgfx)
    if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_GLFW_AND_BGFX)
        add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_GLFW_AND_BGFX)
        set(CAN_BUILD_EXAMPLES true)
    endif()
endif()

if (CAN_BUILD_EXAMPLES)
    # examples
    if (VCLIB_BUILD_EXAMPLES)
        add_subdirectory(00_hello_triangle)
        add_subdirectory(01_minimal_viewer)
        add_subdirectory(02_qt_main_window)

        if (TARGET vclib-external-qglviewer)
            add_subdirectory(qglviewer)
        endif()

        # the following examples are the same of the ones in the vclib library,
        # but they add a viewer at the end in order to visualize the results
        add_subdirectory(vclib)
    endif()
endif()
