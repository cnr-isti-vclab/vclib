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

set(EXAMPLE_NAME 02-mesh-viewer-qt)
project(vclib-render-example-${EXAMPLE_NAME})

### Build settings
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    custom_mesh_viewer_render_app.h
    main.cpp)

target_include_directories(vclib-render PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

# will create a target called 'vclib-render-example-${EXAMPLE_NAME}'
vclib_add_example(${EXAMPLE_NAME}
    VCLIB_MODULE render
    SOURCES ${SOURCES})
