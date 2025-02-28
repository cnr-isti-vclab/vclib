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
project(vclib)

### Options

option(VCLIB_COMPILE_WARNINGS_AS_ERRORS "Compile warnings as errors" ON)

# Optional modules build options
option(VCLIB_BUILD_MODULE_EXTERNAL "Build the external module" OFF)
option(VCLIB_BUILD_MODULE_PROCESSING "Build the processing module" OFF)
option(VCLIB_BUILD_MODULE_RENDER "Build the render module" OFF)

# Build examples
option(VCLIB_BUILD_EXAMPLES "Build examples" ON)

# Build tests
option(VCLIB_BUILD_TESTS "Build tests" ON)

# If true, the examples and tests of VCLib will be built just setting
# the INCLUDE_PATH of the core module (no cmake targets).
option(VCLIB_TESTS_AND_EXAMPLES_USE_CORE_HEADER_ONLY
    "If true, the examples and tests of VCLib-core will be built just setting
    the INCLUDE_PATH of the library (no cmake targets)." OFF)

### Common settings
include(cmake/vclib_common_settings.cmake)

### Examples and Tests
include(cmake/examples_and_tests.cmake)

add_library(${PROJECT_NAME} INTERFACE)
add_library(vclib::vclib ALIAS ${PROJECT_NAME})

add_subdirectory(vclib/core)
target_link_libraries(vclib INTERFACE vclib::core)

if (VCLIB_BUILD_MODULE_EXTERNAL)
    add_subdirectory(vclib/external)
    target_link_libraries(vclib INTERFACE vclib::external)
endif()

if (VCLIB_BUILD_MODULE_PROCESSING)
    add_subdirectory(vclib/processing)
    target_link_libraries(vclib INTERFACE vclib::processing)
endif()

if (VCLIB_BUILD_MODULE_RENDER)
    add_subdirectory(vclib/render)
    target_link_libraries(vclib INTERFACE vclib::render)
endif()

set(VCLIB_EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(VCLIB_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

if (VCLIB_TESTS_AND_EXAMPLES_USE_CORE_HEADER_ONLY)
    message(STATUS
        "VCLib tests and examples will use core module in header only mode")
    set(HEADER_ONLY_OPTION HEADER_ONLY)
    set(VCLIB_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vclib/core/include)
endif()

if (VCLIB_BUILD_EXAMPLES)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VCLIB_EXAMPLES_DIR ${VCLIB_EXAMPLES_DIR})

    add_subdirectory(${VCLIB_EXAMPLES_DIR})
endif()

if (VCLIB_BUILD_TESTS)
    include(CTest)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VCLIB_TESTS_DIR ${VCLIB_TESTS_DIR})

    add_subdirectory(${VCLIB_TESTS_DIR})
endif()
