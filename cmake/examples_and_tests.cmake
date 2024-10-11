set(VCLIB_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_library(vclib-tests-examples-common INTERFACE)

target_compile_definitions(vclib-tests-examples-common INTERFACE
    VCLIB_ASSETS_PATH="${VCLIB_ASSETS_PATH}")

target_compile_definitions(vclib-tests-examples-common INTERFACE
    VCLIB_EXAMPLE_MESHES_PATH="${VCLIB_ASSETS_PATH}/example_meshes")

target_compile_definitions(vclib-tests-examples-common INTERFACE
    VCLIB_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results")



function(_vclib_add_test_example name header_only test)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    if (${test})
        set(TARGET_NAME "vclib-test-${name}")
    else()
        set(TARGET_NAME "vclib-example-${name}")
    endif()

    add_executable(${TARGET_NAME} ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${TARGET_NAME} PRIVATE vclib-tests-examples-common)
    if (NOT ${header_only})
        target_link_libraries(${TARGET_NAME} PRIVATE vclib)
    else()
        # additional include and links required when using header only:

        # vclib-core
        target_include_directories(${TARGET_NAME} PRIVATE ${VCLIB_INCLUDE_DIR})

        # eigen
        set(EIGEN_DIR ${VCLIB_INCLUDE_DIR}/../external/eigen-3.4.0/)
        target_include_directories(${TARGET_NAME} PRIVATE ${EIGEN_DIR})

        # Tbb and Threads if available - needed for gcc
        if (TARGET vclib-external-tbb)
            target_link_libraries(${TARGET_NAME} PRIVATE vclib-external-tbb)
        endif()

        if (${test})
            # Catch2
            target_link_libraries(${TARGET_NAME} PRIVATE vclib-external-catch2)
        endif()
    endif()

    if (${test})
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "tests")
        enable_testing()
        add_test(NAME ${name} COMMAND ${TARGET_NAME})
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")
    endif()
endfunction()

function(vclib_add_example name header_only)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    _vclib_add_test_example(${name} ${header_only} FALSE ${ARG_UNPARSED_ARGUMENTS})
endfunction()

function(vclib_add_test name header_only)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    _vclib_add_test_example(${name} ${header_only} TRUE ${ARG_UNPARSED_ARGUMENTS})
endfunction()

# declare a function vclib_render_add_example:
# vclib_render_add_example(<example_name> SOURCES <source_files>... [VCLIB_EXAMPLE <vclib example>])
# where:
# example_name: the name of the example
# source_files: the source files of the example
# vclib_example: the name of the vclib example associated with the example
#                - todo: rename to core
function(vclib_render_add_example name)
    cmake_parse_arguments(ARG "" "VCLIB_EXAMPLE" "SOURCES" ${ARGN})

    set(TARGET_NAME "vclib-render-example-${name}")

    add_executable(${TARGET_NAME} ${ARG_SOURCES})
    target_link_libraries(${TARGET_NAME} PRIVATE
        vclib-render vclib-tests-examples-common vclib-render-examples-common)

    if (ARG_VCLIB_EXAMPLE)
        set(VCLIB_INCLUDE_EXAMPLES_DIR ${VCLIB_EXAMPLES_DIR}/core/${ARG_VCLIB_EXAMPLE})
        target_include_directories(${TARGET_NAME} PUBLIC
                ${VCLIB_INCLUDE_EXAMPLES_DIR})
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")
endfunction()
