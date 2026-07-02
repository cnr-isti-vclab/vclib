# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

#############################################################################
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
project(standalone-vclib-core-template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable FetchContent to download VCLib at configure time
include(FetchContent)

# Alteratively, if VCLib is installed on your system, you can use find_package:
# find_package(vclib REQUIRED)

FetchContent_Declare(
    vclib
    GIT_REPOSITORY https://github.com/cnr-isti-vclab/vclib.git
    GIT_TAG origin/main
)

FetchContent_MakeAvailable(vclib)

# Create the standalone executable
add_executable(${PROJECT_NAME} main.cpp)

# Link with VCLib - this will automatically pull in all dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE vclib::vclib)

# Print configuration info
message(STATUS "VCLib Standalone Template Project")
