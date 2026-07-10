# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(VCLIB_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_library(vclib-tests-examples-common INTERFACE)

target_compile_definitions(
    vclib-tests-examples-common
    INTERFACE VCLIB_ASSETS_PATH="${VCLIB_ASSETS_PATH}"
)

target_compile_definitions(
    vclib-tests-examples-common
    INTERFACE VCLIB_EXAMPLE_MESHES_PATH="${VCLIB_ASSETS_PATH}/example_meshes"
)

target_compile_definitions(
    vclib-tests-examples-common
    INTERFACE VCLIB_CORE_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results/core"
)

target_compile_definitions(
    vclib-tests-examples-common
    INTERFACE
        VCLIB_EXTERNAL_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results/external"
)

function(_vclib_add_test_example name)
    set(options TEST HEADER_ONLY)
    set(oneValueArgs MODULE CORE_EXAMPLE EXTERNAL_EXAMPLE)
    set(multiValueArgs LINK_MODULES SOURCES)

    cmake_parse_arguments(
        ARG
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    if(NOT ARG_MODULE)
        set(ARG_MODULE "core")
    endif()

    if(${ARG_TEST})
        set(TARGET_NAME "vclib-${ARG_MODULE}-test-${name}")
    else()
        if(ARG_CORE_EXAMPLE)
            set(TARGET_NAME "vclib-${ARG_MODULE}-example-core-${name}")
        elseif(ARG_EXTERNAL_EXAMPLE)
            set(TARGET_NAME "vclib-${ARG_MODULE}-example-external-${name}")
        else()
            set(TARGET_NAME "vclib-${ARG_MODULE}-example-${name}")
        endif()
    endif()

    if(${VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS})
        set(TO_EXCLUDE TRUE)
    else()
        set(TO_EXCLUDE FALSE)
    endif()

    add_executable(${TARGET_NAME} ${ARG_SOURCES})

    if(WIN32)
        set(COPY_DLLS_SCRIPT
            "${CMAKE_CURRENT_BINARY_DIR}/copy_dlls_${TARGET_NAME}.cmake"
        )
        file(
            GENERATE OUTPUT "${COPY_DLLS_SCRIPT}"
            CONTENT
                "
set(DLLS \"$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>\")
if(DLLS)
    execute_process(COMMAND \${CMAKE_COMMAND} -E copy_if_different \${DLLS} \"$<TARGET_FILE_DIR:${TARGET_NAME}>\")
    if(\"${VCLIB_WINDEPLOYQT_EXECUTABLE}\" AND DLLS MATCHES \"Qt.*Core\")
        execute_process(COMMAND \"${VCLIB_WINDEPLOYQT_EXECUTABLE}\" \"$<TARGET_FILE:${TARGET_NAME}>\")
    endif()
endif()
"
        )
        add_custom_command(
            TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -P "${COPY_DLLS_SCRIPT}"
            COMMENT "Copying runtime DLLs for ${TARGET_NAME}"
        )
    endif()

    # for each module in LINK_MODULES, link the example/test with it
    foreach(module IN LISTS ARG_LINK_MODULES)
        target_link_libraries(${TARGET_NAME} PRIVATE vclib::${module})
    endforeach()

    target_link_libraries(${TARGET_NAME} PRIVATE vclib-tests-examples-common)

    if(${TO_EXCLUDE})
        set_target_properties(${TARGET_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()

    # if ARG_MODULE is "render"
    if(ARG_MODULE STREQUAL "render")
        target_link_libraries(
            ${TARGET_NAME}
            PRIVATE vclib-render-examples-common
        )
    endif()

    if(ARG_CORE_EXAMPLE)
        set(VCLIB_INCLUDE_EXAMPLES_DIR
            ${VCLIB_EXAMPLES_DIR}/core/${ARG_CORE_EXAMPLE}
        )
        target_include_directories(
            ${TARGET_NAME}
            PUBLIC ${VCLIB_INCLUDE_EXAMPLES_DIR}
        )
    endif()

    if(ARG_EXTERNAL_EXAMPLE)
        set(VCLIB_INCLUDE_EXAMPLES_DIR
            ${VCLIB_EXAMPLES_DIR}/external/${ARG_EXTERNAL_EXAMPLE}
        )
        target_include_directories(
            ${TARGET_NAME}
            PUBLIC ${VCLIB_INCLUDE_EXAMPLES_DIR}
        )
    endif()

    if(NOT ${ARG_HEADER_ONLY})
        target_link_libraries(${TARGET_NAME} PRIVATE vclib::${ARG_MODULE})
    else()
        # additional include and links required when using header only:

        # vclib-core
        target_include_directories(
            ${TARGET_NAME}
            PRIVATE ${VCLIB_CORE_INCLUDE_DIR}
        )

        # eigen
        set(EIGEN_DIR ${VCLIB_CORE_INCLUDE_DIR}/../external/eigen-3.4.0/)
        target_include_directories(${TARGET_NAME} PRIVATE ${EIGEN_DIR})

        # Tbb and Threads if available - needed for gcc
        if(TARGET vclib-3rd-tbb)
            target_link_libraries(${TARGET_NAME} PRIVATE vclib-3rd-tbb)
        endif()

        if(${test})
            # Catch2
            target_link_libraries(${TARGET_NAME} PRIVATE vclib-3rd-catch2)
        endif()
    endif()

    if(${ARG_TEST})
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "tests")
        if(NOT ${TO_EXCLUDE})
            enable_testing()
            add_test(NAME ${name} COMMAND ${TARGET_NAME})
        endif()
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")
    endif()
endfunction()

# example of call of this function
# vclib_add_example(
#     000-mesh-basic # name of the example
#     [HEADER_ONLY] # optional - to build the example with includepath
#                     (no cmake targets)
#     [MODULE core] # optional - to specify the module of the example
#                     (default is core)
#     [CORE_EXAMPLE 000-mesh-basic] # optional - to specify the example of
#                                     the core module from which reuse some code
#     [EXTERNAL_EXAMPLE 000-mesh] # optional - to specify the example of
#                                 the external module from which reuse some code
#     [LINK_MODULES render] # optional - to specify additional modules to link
#                             to the example
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
#     [MODULE core] # optional - to specify the module of the test
#                     (default is core)
#     [CORE_EXAMPLE 000-mesh-basic] # optional - to specify the example of
#                                     the core module from which reuse some code
#     [EXTERNAL_EXAMPLE 000-mesh] # optional - to specify the example of
#                                 the external module from which reuse some code
#     [LINK_MODULES render] # optional - to specify additional modules to link
#                             to the test
#     SOURCES main.cpp # sources of the test
# )
function(vclib_add_test name)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    _vclib_add_test_example(${name} TEST ${ARG_UNPARSED_ARGUMENTS})
endfunction()
