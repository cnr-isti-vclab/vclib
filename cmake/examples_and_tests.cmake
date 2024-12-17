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

set(VCLIB_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_library(vclib-tests-examples-common INTERFACE)

target_compile_definitions(vclib-tests-examples-common INTERFACE
    VCLIB_ASSETS_PATH="${VCLIB_ASSETS_PATH}")

target_compile_definitions(vclib-tests-examples-common INTERFACE
    VCLIB_EXAMPLE_MESHES_PATH="${VCLIB_ASSETS_PATH}/example_meshes")

target_compile_definitions(vclib-tests-examples-common INTERFACE
    VCLIB_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results")

function(_vclib_add_test_example name)
    set(options TEST HEADER_ONLY)
    set(oneValueArgs VCLIB_MODULE VCLIB_CORE_EXAMPLE)
    set(multiValueArgs SOURCES)

    cmake_parse_arguments(ARG
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT ARG_VCLIB_MODULE)
        set(ARG_VCLIB_MODULE "core")
    endif()

    if (${ARG_TEST})
        set(TARGET_NAME "vclib-${ARG_VCLIB_MODULE}-test-${name}")
    else()
        set(TARGET_NAME "vclib-${ARG_VCLIB_MODULE}-example-${name}")
    endif()

    add_executable(${TARGET_NAME} ${ARG_SOURCES})
    target_link_libraries(${TARGET_NAME} PRIVATE vclib-tests-examples-common)

    # if ARG_VCLIB_MODULE is "render"
    if (ARG_VCLIB_MODULE STREQUAL "render")
        target_link_libraries(${TARGET_NAME} PRIVATE vclib-render-examples-common)
    endif()

    if (ARG_VCLIB_CORE_EXAMPLE)
        set(VCLIB_INCLUDE_EXAMPLES_DIR ${VCLIB_EXAMPLES_DIR}/core/${ARG_VCLIB_CORE_EXAMPLE})
        target_include_directories(${TARGET_NAME} PUBLIC
                ${VCLIB_INCLUDE_EXAMPLES_DIR})
    endif()

    if (NOT ${ARG_HEADER_ONLY})
        target_link_libraries(${TARGET_NAME} PRIVATE vclib-${ARG_VCLIB_MODULE})
    else()
        # additional include and links required when using header only:

        # vclib-core
        target_include_directories(${TARGET_NAME} PRIVATE
            ${VCLIB_CORE_INCLUDE_DIR})

        # eigen
        set(EIGEN_DIR ${VCLIB_CORE_INCLUDE_DIR}/../external/eigen-3.4.0/)
        target_include_directories(${TARGET_NAME} PRIVATE ${EIGEN_DIR})

        # Tbb and Threads if available - needed for gcc
        if (TARGET vclib-3rd-tbb)
            target_link_libraries(${TARGET_NAME} PRIVATE vclib-3rd-tbb)
        endif()

        if (${test})
            # Catch2
            target_link_libraries(${TARGET_NAME} PRIVATE vclib-3rd-catch2)
        endif()
    endif()

    if (${ARG_TEST})
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "tests")
        enable_testing()
        add_test(NAME ${name} COMMAND ${TARGET_NAME})
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")
    endif()
endfunction()

# example of call of this function
# vclib_add_example(
#     000-mesh-basic # name of the example
#     [HEADER_ONLY] # optional - to build the example with includepath
#                     (no cmake targets)
#     [VCLIB_MODULE core] # optional - to specify the module of the example
#                           (default is core)
#     [VCLIB_CORE_EXAMPLE 000-mesh-basic] # optional - to specify the example of
#                                     the core module from which reuse some code
#     SOURCES main.cpp # sources of the example
# )
function(vclib_add_example name)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    _vclib_add_test_example(${name} ${ARG_UNPARSED_ARGUMENTS})
endfunction()

# example of call of this function
# vclib_add_test(
#     000-mesh-basic # name of the test
#     [HEADER_ONLY] # optional - to build the test with includepath
#                     (no cmake targets)
#     [VCLIB_MODULE core] # optional - to specify the module of the test
#                           (default is core)
#     [VCLIB_CORE_EXAMPLE 000-mesh-basic] # optional - to specify the example of
#                                     the core module from which reuse some code
#     SOURCES main.cpp # sources of the test
# )
function(vclib_add_test name)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    _vclib_add_test_example(${name} TEST ${ARG_UNPARSED_ARGUMENTS})
endfunction()
