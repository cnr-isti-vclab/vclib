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

option(VCLIB_RENDER_BUILD_EXAMPLES_WITH_QT "Force building examples using qt libraries" ON)
option(VCLIB_RENDER_BUILD_EXAMPLES_WITH_GLFW "Force building examples using glfw libraries" ON)
option(VCLIB_RENDER_BUILD_EXAMPLES_WITH_QGLVIEWER "Force building examples using qglviewer library" ON)

if (TARGET vclib-external-qt)
    if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_QT)
        add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_QT)
        set(CAN_BUILD_EXAMPLES true)
    endif()
endif()
if(TARGET vclib-external-glfw)
    if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_GLFW AND NOT CAN_BUILD_EXAMPLES)
        add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_GLFW)
        set(CAN_BUILD_EXAMPLES true)
    endif()
endif()
if(TARGET vclib-external-qglviewer)
    if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_QGLVIEWER AND NOT CAN_BUILD_EXAMPLES)
        add_compile_definitions(VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER)
        set(CAN_BUILD_EXAMPLES true)
    endif()
endif()

if (CAN_BUILD_EXAMPLES)
    # examples
    if (VCLIB_BUILD_EXAMPLES)
        add_subdirectory(common)

        # examples with bgfx and a window manager (qt or glfw)
        if ((VCLIB_RENDER_BUILD_EXAMPLES_WITH_QT OR VCLIB_RENDER_BUILD_EXAMPLES_WITH_GLFW) AND
                (TARGET vclib-external-qt OR TARGET vclib-external-glfw))
            add_subdirectory(00-hello-triangle)
            add_subdirectory(01-viewer)

            if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_QT AND TARGET vclib-external-qt)
                add_subdirectory(02-qt-main-window)
                add_subdirectory(03-qt-viewer-with-text)
            endif()

            if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_GLFW AND TARGET vclib-external-glfw)
                add_subdirectory(04-glfw-viewer-with-text)
            endif()
        endif()

        add_subdirectory(999-misc)

        if (TARGET vclib-external-qglviewer)
            add_subdirectory(899-qglviewer)
        endif()

        # the following examples are the same of the ones in the vclib library,
        # but they add a viewer at the end in order to visualize the results
        add_subdirectory(vclib)
    endif()
endif()
