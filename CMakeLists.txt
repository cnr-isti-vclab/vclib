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
project(vclib)

### Common settings
include(cmake/vclib_common_settings.cmake)

### Options
option(VCLIB_BUILD_EXAMPLES "Build VCLib examples" ON)
option(VCLIB_BUILD_TESTS "Build VCLib tests" ON)

option(VCLIB_COMPILE_WARNINGS_AS_ERRORS "Compile warnings as errors" ON)

option(VCLIB_TESTS_AND_EXAMPLES_HEADER_ONLY
    "If true, the examples and tests of VCLib will be built just setting the
    INCLUDE_PATH of the library (no cmake targets)." OFF)

# external libraries
message(STATUS "VCLib: external libraries")
add_subdirectory(external)

### vclib target
set(VCLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/*.h")

add_library(vclib INTERFACE ${HEADERS})
target_include_directories(vclib INTERFACE ${VCLIB_INCLUDE_DIR})

set_target_properties(vclib PROPERTIES
    VCLIB_INCLUDE_DIR ${VCLIB_INCLUDE_DIR})

target_link_libraries(vclib INTERFACE ${VCLIB_EXTERNAL_LIBRARIES})

target_sources(vclib PRIVATE ${HEADERS})

if (MSVC)
    target_compile_options(vclib INTERFACE "/bigobj")
endif()

### Examples and Tests
include(cmake/examples_and_tests.cmake)

set(VCLIB_EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(VCLIB_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

if (VCLIB_BUILD_EXAMPLES)
    set_target_properties(vclib PROPERTIES
        VCLIB_EXAMPLES_DIR ${VCLIB_EXAMPLES_DIR})

    add_subdirectory(${VCLIB_EXAMPLES_DIR})
endif()

if (VCLIB_BUILD_TESTS)
    include(CTest)
    set_target_properties(vclib PROPERTIES
        VCLIB_TESTS_DIR ${VCLIB_TESTS_DIR})

    add_subdirectory(${VCLIB_TESTS_DIR})
endif()

