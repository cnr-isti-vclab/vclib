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

set(EXAMPLE_NAME mesh-sphere-intersection)
project(vclib-${EXAMPLE_NAME}-example)

set(HEADERS
    bimba_sphere_intersection.h)

set(SOURCES
    main.cpp)

vclib_add_example(
    ${EXAMPLE_NAME}
    ${VCLIB_TESTS_AND_EXAMPLES_HEADER_ONLY}
    ${SOURCES})
