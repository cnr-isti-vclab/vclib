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

project(vclib-render-mesh-io-stl)

message(${VCLIB_TEST_DIR})

set(VCLIB_INCLUDE_TEST_DIR ${VCLIB_TEST_DIR}/examples/mesh_io_stl)

set(SOURCES
    main.cpp)

add_executable(vclib-render-mesh-io-stl ${SOURCES})
target_include_directories(
    vclib-render-mesh-io-stl PUBLIC
        ${VCLIB_INCLUDE_TEST_DIR})

target_link_libraries(vclib-render-mesh-io-stl PUBLIC vclib-render vclib-test-common)
