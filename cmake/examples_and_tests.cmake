function(_vclib_add_test_example name header_only test)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    if (${test})
        set(TARGET_NAME "vclib-${name}-test")
    else()
        set(TARGET_NAME "vclib-${name}-example")
    endif()

    add_executable(${TARGET_NAME} ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${TARGET_NAME} PRIVATE vclib-examples-common)
    if (NOT ${header_only})
        target_link_libraries(${TARGET_NAME} PRIVATE vclib)
    else()
        # additional include and links required when using header only:

        # vclib
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

set(VCLIB_ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/examples/assets")

add_library(vclib-examples-common INTERFACE)
target_compile_definitions(vclib-examples-common INTERFACE
    VCLIB_TEST_MODELS_PATH="${VCLIB_ASSETS_PATH}")
target_compile_definitions(vclib-examples-common INTERFACE
    VCLIB_TEST_RESULTS_PATH="${VCLIB_ASSETS_PATH}/results")
