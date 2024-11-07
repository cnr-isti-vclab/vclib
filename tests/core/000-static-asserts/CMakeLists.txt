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

set(TEST_NAME 000-static-asserts)
project(vclib-test-${TEST_NAME})

set(HEADERS
    meshes.h
    loggers.h
    space.h
    meshes/edgemesh.h
    meshes/mesh_views.h
    meshes/pointcloud.h
    meshes/polymesh.h
    meshes/trimesh.h
    space/array.h
    space/box.h
    space/color.h
    space/image.h
    space/matrix.h
    space/plane.h
    space/point.h
)

set(SOURCES
    main.cpp
)

vclib_add_test(
    ${TEST_NAME}
    SOURCES ${HEADERS} ${SOURCES}
    ${HEADER_ONLY_OPTION})
