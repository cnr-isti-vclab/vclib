function(vclib_render_add_example name include_vclib_example_dir)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    set(TARGET_NAME "vclib-render-${name}-example")

    add_executable(${TARGET_NAME} ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${TARGET_NAME} PRIVATE vclib-render vclib-test-common)

    if (${include_vclib_example_dir})
        string(REPLACE "-" "_" out_name ${name})
        set(VCLIB_INCLUDE_TEST_DIR ${VCLIB_TEST_DIR}/examples/${out_name})

        target_include_directories(${TARGET_NAME} PUBLIC
                ${VCLIB_INCLUDE_TEST_DIR})
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")
endfunction()
