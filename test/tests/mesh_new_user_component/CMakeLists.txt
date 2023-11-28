#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)

set(TEST_NAME mesh-new-user-component)
project(vclib-${TEST_NAME}-test)

set(SOURCES
    main.cpp)

vclib_add_test(
    ${TEST_NAME}
    ${VCLIB_TESTS_AND_EXAMPLES_HEADER_ONLY}
    ${SOURCES})

target_include_directories(vclib-${TEST_NAME}-test
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../examples/mesh_new_user_component)
