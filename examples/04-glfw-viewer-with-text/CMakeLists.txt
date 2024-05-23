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

if (TARGET vclib-external-qt OR TARGET vclib-external-glfw)
    set(EXAMPLE_NAME 04-glfw-viewer-with-text)
    project(vclib-render-example-${EXAMPLE_NAME})

    ### Build settings
    set(CMAKE_CXX_STANDARD 20)

    set(HEADERS
        common.h)

    set(SOURCES
        main.cpp)

    if (VCLIB_RENDER_BUILD_EXAMPLES_WITH_QT_AND_BGFX)
        add_compile_definitions(USE_QT)
    elseif (VCLIB_RENDER_BUILD_EXAMPLES_WITH_GLFW_AND_BGFX)
        add_compile_definitions(USE_GLFW)
    endif()

    # will create a target called 'vclib-render-example-${EXAMPLE_NAME}'
    vclib_render_add_example(${EXAMPLE_NAME} SOURCES ${HEADERS} ${SOURCES})
endif()
