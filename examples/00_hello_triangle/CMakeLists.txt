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
#* it under the terms of the GNU General Public License as published by      *
#* the Free Software Foundation; either version 3 of the License, or         *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
#* for more details.                                                         *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)

set(EXAMPLE_NAME 00-hello-triangle)
project(vclib-render-${EXAMPLE_NAME}-example)

### Build settings
set(CMAKE_CXX_STANDARD 20)

set(HEADERS
    common.h)

set(SOURCES
    main.cpp)

set(SHADERS
    shaders/vs_vertex_shader.sc
    shaders/fs_fragment_shader.sc
    shaders/varying.def.sc)

if (TARGET vclib-external-qt)
    add_compile_definitions(USE_QT)
    list(APPEND HEADERS hello_triangle_qt.h)
    list(APPEND SOURCES hello_triangle_qt.cpp)
elseif (TARGET vclib-external-glfw)
    add_compile_definitions(USE_GLFW)
    list(APPEND HEADERS hello_triangle_glfw.h)
    list(APPEND SOURCES hello_triangle_glfw.cpp)
endif()

vclib_render_add_example(${EXAMPLE_NAME} TRUE ${HEADERS} ${SOURCES})

target_add_bgfx_shaders(${PROJECT_NAME} ${SHADERS})
