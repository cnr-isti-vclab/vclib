# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.24)
project(vclib-standalone-examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(
    VCLIB_USE_FETCHCONTENT
    "If true, VCLib is downloaded via FetchContent instead of using find_package"
    ON
)

option(VCLIB_STANDALONE_BUILD_CORE "Build standalone core examples" ON)
option(VCLIB_STANDALONE_BUILD_EXTERNAL "Build standalone external examples" OFF)
option(VCLIB_STANDALONE_BUILD_RENDER "Build standalone render examples" OFF)

if(VCLIB_STANDALONE_BUILD_EXTERNAL AND NOT VCLIB_USE_FETCHCONTENT)
    message(
        FATAL_ERROR
        "Building standalone external examples is currently only supported with FetchContent. Please set VCLIB_USE_FETCHCONTENT=ON"
    )
endif()

if(VCLIB_STANDALONE_BUILD_RENDER AND NOT VCLIB_USE_FETCHCONTENT)
    message(
        FATAL_ERROR
        "Building standalone render examples is currently only supported with FetchContent. Please set VCLIB_USE_FETCHCONTENT=ON"
    )
endif()

if(VCLIB_USE_FETCHCONTENT)
    message(STATUS "VCLib Standalone: Using FetchContent")
    include(FetchContent)

    # We use the local repository root for testing local changes
    set(VCLIB_LOCAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

    set(VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS ON CACHE STRING "" FORCE)

    # We configure CMake to build only the required modules to keep things fast during our tests
    if(VCLIB_STANDALONE_BUILD_EXTERNAL)
        set(VCLIB_BUILD_MODULE_EXTERNAL ON CACHE STRING "" FORCE)
    else()
        set(VCLIB_BUILD_MODULE_EXTERNAL OFF CACHE STRING "" FORCE)
    endif()

    if(VCLIB_STANDALONE_BUILD_RENDER)
        set(VCLIB_BUILD_MODULE_RENDER ON CACHE STRING "" FORCE)
    else()
        set(VCLIB_BUILD_MODULE_RENDER OFF CACHE STRING "" FORCE)
    endif()

    set(VCLIB_BUILD_MODULE_BINDINGS OFF CACHE STRING "" FORCE)

    FetchContent_Declare(vclib SOURCE_DIR ${VCLIB_LOCAL_DIR})
    FetchContent_MakeAvailable(vclib)
else()
    message(STATUS "VCLib Standalone: Using find_package")
    find_package(vclib REQUIRED)
endif()

enable_testing()

# Define data paths macros (mirroring cmake/examples_and_tests.cmake)
set(VCLIB_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../assets")

add_library(vclib-standalone-examples-common INTERFACE)
target_compile_definitions(
    vclib-standalone-examples-common
    INTERFACE
        VCLIB_ASSETS_PATH="${VCLIB_ASSETS_PATH}"
        VCLIB_EXAMPLE_MESHES_PATH="${VCLIB_ASSETS_PATH}/example_meshes"
        VCLIB_CORE_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results/core"
        VCLIB_EXTERNAL_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results/external"
)

# example of call of this function
# add_standalone_example(
#     core # module name (e.g., core, external)
#     000-mesh-basic # name of the example directory
# )
#
# Function to add a standalone example linking to the original source code.
# It sets up the executable, links it against vclib and common example properties,
# adds the required include paths, and registers a CTest to run the example.
function(add_standalone_example module name)
    set(TARGET_NAME "standalone-${module}-${name}")
    string(REPLACE "/" "-" TARGET_NAME "${TARGET_NAME}")
    set(SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../${module}/${name}/main.cpp")

    if(NOT EXISTS "${SOURCE_FILE}")
        message(FATAL_ERROR "Source file not found: ${SOURCE_FILE}")
    endif()

    add_executable(${TARGET_NAME} "${SOURCE_FILE}")

    # Link to vclib and the common definitions
    target_link_libraries(
        ${TARGET_NAME}
        PRIVATE vclib::vclib vclib-standalone-examples-common
    )

    # Add the example directory to include path in case it uses local headers
    target_include_directories(
        ${TARGET_NAME}
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../${module}/${name}"
    )

    # Add any extra includes passed to the function
    foreach(extra_inc IN LISTS ARGN)
        target_include_directories(
            ${TARGET_NAME}
            PRIVATE
                "${CMAKE_CURRENT_SOURCE_DIR}/../${module}/${name}/${extra_inc}"
        )
    endforeach()

    # Register the test for CTest
    add_test(NAME run-${TARGET_NAME} COMMAND ${TARGET_NAME})
endfunction()

if(VCLIB_STANDALONE_BUILD_CORE)
    include(core.cmake)
endif()

if(VCLIB_STANDALONE_BUILD_EXTERNAL)
    include(external.cmake)
endif()

if(VCLIB_STANDALONE_BUILD_RENDER)
    if(TARGET vclib-3rd-qt OR TARGET vclib::vclib-3rd-qt)
        target_compile_definitions(
            vclib-standalone-examples-common
            INTERFACE VCLIB_RENDER_EXAMPLES_WITH_QT
        )
    elseif(TARGET vclib-3rd-glfw OR TARGET vclib::vclib-3rd-glfw)
        target_compile_definitions(
            vclib-standalone-examples-common
            INTERFACE VCLIB_RENDER_EXAMPLES_WITH_GLFW
        )
    else()
        message(
            FATAL_ERROR
            "Render standalone examples require either vclib-3rd-qt or vclib-3rd-glfw to be built."
        )
    endif()
    target_include_directories(
        vclib-standalone-examples-common
        INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/../render/common"
    )
    include(render.cmake)
endif()
