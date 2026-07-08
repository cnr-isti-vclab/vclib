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
project(standalone-vclib-render-template-custom-shader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(
    VCLIB_STANDALONE_USE_FETCHCONTENT
    "If true, VCLib is downloaded via FetchContent instead of using find_package"
    ON
)

if(VCLIB_STANDALONE_USE_FETCHCONTENT)
    # Enable FetchContent to download VCLib at configure time
    include(FetchContent)

    # Set VCLib options BEFORE calling FetchContent_Declare
    set(VCLIB_BUILD_MODULE_RENDER ON CACHE STRING "" FORCE)

    FetchContent_Declare(
        vclib
        GIT_REPOSITORY https://github.com/cnr-isti-vclab/vclib.git
        GIT_TAG origin/main
    )

    FetchContent_MakeAvailable(vclib)
else()
    find_package(vclib REQUIRED)
endif()

# Create the standalone executable
add_executable(${PROJECT_NAME} main.cpp)

# Link with VCLib - this will automatically pull in all dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE vclib::render)

# Add shaders
vclib_target_add_shaders(${PROJECT_NAME}
    shaders/vs_vertex_shader.sc
    shaders/fs_fragment_shader.sc
    shaders/varying.def.sc
)

# Print configuration info
message(STATUS "VCLib Standalone Template Project - Custom Shader")
