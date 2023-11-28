#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)

set(TEST_NAME kd-tree)
project(vclib-${TEST_NAME}-test)

set(SOURCES
    main.cpp)

vclib_add_test(
    ${TEST_NAME}
    ${VCLIB_TESTS_AND_EXAMPLES_HEADER_ONLY}
    ${SOURCES})
