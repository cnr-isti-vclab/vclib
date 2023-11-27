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

project(vclib-render-mesh-sphere-intersection)

set(VCLIB_INCLUDE_TEST_DIR ${VCLIB_TEST_DIR}/examples/mesh_sphere_intersection)

set(SOURCES
    main.cpp)

add_executable(vclib-render-mesh-sphere-intersection ${SOURCES})
target_include_directories(
    vclib-render-mesh-sphere-intersection PUBLIC
        ${VCLIB_INCLUDE_TEST_DIR})

target_link_libraries(vclib-render-mesh-sphere-intersection PUBLIC vclib-render vclib-test-common)
