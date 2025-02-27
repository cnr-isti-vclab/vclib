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
project(vclib-bindings)

# TODO: rename
set(MODULE_NAME vcl)

# 3rdparty libraries
message(STATUS "VCLib-Bindings: 3rd party libraries")
add_subdirectory(3rdparty)

### vclib-bindings target
set(VCLIB_BINDINGS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "bindings/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "bindings/*.cpp")

pybind11_add_module(${MODULE_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${MODULE_NAME} PRIVATE ${VCLIB_BINDINGS_INCLUDE_DIR})
target_link_libraries(${MODULE_NAME} PRIVATE vclib-core)

