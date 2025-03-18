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

set(MODULE_NAME vclib_bindings)

# 3rdparty libraries
message(STATUS "VCLib-Bindings: 3rd party libraries")
add_subdirectory(3rdparty)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/vclib/bindings/*.cpp")

pybind11_add_module(${MODULE_NAME} ${HEADERS} ${SOURCES})
target_compile_definitions(${MODULE_NAME} PRIVATE
    VCLIB_MAIN_MODULE_NAME=${MODULE_NAME})

set(VCLIB_BINDINGS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${MODULE_NAME} PRIVATE ${VCLIB_BINDINGS_INCLUDE_DIR})
target_link_libraries(${MODULE_NAME} PRIVATE vclib::core)

# put the generated binary in a dir called vclib
set_target_properties(${MODULE_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/vclib)

# after build module, copy the file 'assets/__init__.py' to the build dir
add_custom_command(TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/__init__.py"
    "${CMAKE_CURRENT_BINARY_DIR}/vclib/__init__.py")

