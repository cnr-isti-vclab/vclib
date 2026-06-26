#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2026                                                    *
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
#
# VCLib Template Project
#
# This is a standalone template CMake project that demonstrates how to use VCLib
# via FetchContent.
#
# To build this example independently:
#   mkdir build && cd build
#   cmake .. -DCMAKE_BUILD_TYPE=Release
#   cmake --build . --config Release
#############################################################################

cmake_minimum_required(VERSION 3.24)
project(standalone-vclib-template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable FetchContent to download VCLib at configure time
include(FetchContent)

# Alteratively, if VCLib is installed on your system, you can use find_package:
# find_package(vclib REQUIRED)

# Set VCLib options BEFORE calling FetchContent_Declare
set(VCLIB_BUILD_MODULE_EXTERNAL OFF CACHE STRING "" FORCE)
set(VCLIB_BUILD_MODULE_RENDER OFF CACHE STRING "" FORCE)
set(VCLIB_BUILD_MODULE_BINDINGS OFF CACHE STRING "" FORCE)
set(VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS ON CACHE STRING "" FORCE)

FetchContent_Declare(
    vclib
    GIT_REPOSITORY https://github.com/cnr-isti-vclab/vclib.git
    GIT_TAG        origin/main
)

FetchContent_MakeAvailable(vclib)

# Create the standalone executable
add_executable(${PROJECT_NAME} main.cpp)

# Link with VCLib - this will automatically pull in all dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE vclib::vclib)

# Print configuration info
message(STATUS "VCLib Standalone Template Project")
