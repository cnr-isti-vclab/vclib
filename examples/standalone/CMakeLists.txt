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

cmake_minimum_required(VERSION 3.24)
project(vclib-standalone-examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(VCLIB_USE_FETCHCONTENT "If true, VCLib is downloaded via FetchContent instead of using find_package" OFF)

if (VCLIB_USE_FETCHCONTENT)
    message(STATUS "VCLib Standalone: Using FetchContent")
    include(FetchContent)
    
    # We use the local repository root for testing local changes
    set(VCLIB_LOCAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    
    set(VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS ON CACHE STRING "" FORCE)
    
    # We configure CMake to build only the core module to keep things fast during our tests
    set(VCLIB_BUILD_MODULE_EXTERNAL OFF CACHE STRING "" FORCE)
    set(VCLIB_BUILD_MODULE_RENDER OFF CACHE STRING "" FORCE)
    set(VCLIB_BUILD_MODULE_BINDINGS OFF CACHE STRING "" FORCE)

    FetchContent_Declare(
        vclib
        SOURCE_DIR ${VCLIB_LOCAL_DIR}
    )
    FetchContent_MakeAvailable(vclib)
else()
    message(STATUS "VCLib Standalone: Using find_package")
    find_package(vclib REQUIRED)
endif()

enable_testing()

# Define data paths macros (mirroring cmake/examples_and_tests.cmake)
set(VCLIB_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../assets")

add_library(vclib-standalone-examples-common INTERFACE)
target_compile_definitions(vclib-standalone-examples-common INTERFACE
    VCLIB_ASSETS_PATH="${VCLIB_ASSETS_PATH}"
    VCLIB_EXAMPLE_MESHES_PATH="${VCLIB_ASSETS_PATH}/example_meshes"
    VCLIB_CORE_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results/core"
    VCLIB_EXTERNAL_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results/external"
)


# Function to add a standalone example linking to the original source code
function(add_standalone_example module name)
    set(TARGET_NAME "standalone-${module}-${name}")
    set(SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../${module}/${name}/main.cpp")
    
    if (NOT EXISTS "${SOURCE_FILE}")
        message(FATAL_ERROR "Source file not found: ${SOURCE_FILE}")
    endif()

    add_executable(${TARGET_NAME} "${SOURCE_FILE}")
    
    # Link to vclib and the common definitions
    target_link_libraries(${TARGET_NAME} PRIVATE vclib::vclib vclib-standalone-examples-common)
    
    # Add the example directory to include path in case it uses local headers
    target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../${module}/${name}")
    
    # Register the test for CTest
    add_test(NAME run-${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction()

# Add all core examples automatically
file(GLOB CORE_EXAMPLE_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/../core" "${CMAKE_CURRENT_SOURCE_DIR}/../core/*")
foreach(dir IN LISTS CORE_EXAMPLE_DIRS)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../core/${dir}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../core/${dir}/main.cpp")
            add_standalone_example(core "${dir}")
        endif()
    endif()
endforeach()
