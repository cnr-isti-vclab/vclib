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

set(TEST_NAME 000-vcg-mesh-import)
project(vclib-test-${TEST_NAME})

# vcg generates a lot of warnings
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

set(SOURCES
    mesh.h
    main.cpp)

vclib_add_test(
    ${TEST_NAME}
    VCLIB_MODULE external
    SOURCES ${SOURCES}
    ${HEADER_ONLY_OPTION})

get_property(VCG_INCLUDE_DIRS
    TARGET vclib-3rd-vcg
    PROPERTY VCG_INCLUDE_DIRS)

# add plylib.cpp source from vcg
target_sources(vclib-external-test-${TEST_NAME} PRIVATE
    ${VCG_INCLUDE_DIRS}/wrap/ply/plylib.cpp)
