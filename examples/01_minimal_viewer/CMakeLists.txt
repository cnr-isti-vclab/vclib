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

set(EXAMPLE_NAME 01-minimal-viewer)
project(vclib-render-${EXAMPLE_NAME}-example)

### Build settings
set(CMAKE_CXX_STANDARD 20)

set(HEADERS
    common.h)

set(SOURCES
    main.cpp)

if (TARGET vclib-external-qt)
    add_compile_definitions(USE_QT)
    list(APPEND HEADERS minimal_viewer_qt.h)
    list(APPEND SOURCES minimal_viewer_qt.cpp)
elseif (TARGET vclib-external-glfw)
    add_compile_definitions(USE_GLFW)
    list(APPEND HEADERS minimal_viewer_glfw.h)
    list(APPEND SOURCES minimal_viewer_glfw.cpp)
endif()

# will create a target called 'vclib-render-${EXAMPLE_NAME}-example'
vclib_render_add_example(${EXAMPLE_NAME} TRUE ${HEADERS} ${SOURCES})

# this target will use the vclib_render bgfx shaders, therefore we need to
# expose them to the target
target_expose_vclib_bgfx_shaders(${PROJECT_NAME})
