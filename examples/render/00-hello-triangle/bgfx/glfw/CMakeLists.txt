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

set(EXAMPLE_NAME 00-hello-triangle-glfw)
project(vclib-render-example-${EXAMPLE_NAME})

### Build settings
set(CMAKE_CXX_STANDARD 20)

set(HEADERS
    ../common.h
    ../hello_triangle_drawer.h)

set(SOURCES
    main.cpp)

set(SHADERS
    shanders/vs_vertex_shader.sc
    shaders/fs_fragment_shader.sc
    shaders/varyig.def.sc)

vclib_add_example(${EXAMPLE_NAME}
    VCLIB_MODULE render
    SOURCES ${HEADERS} ${SOURCES})

target_add_bgfx_shaders(${PROJECT_NAME} ${SHADERS})
