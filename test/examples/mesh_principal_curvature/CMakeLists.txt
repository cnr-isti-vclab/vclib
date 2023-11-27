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

project(vclib-render-mesh-principal-curvature)

set(VCLIB_INCLUDE_TEST_DIR ${VCLIB_TEST_DIR}/examples/mesh_principal_curvature)

set(SOURCES
    main.cpp)

add_executable(vclib-render-mesh-principal-curvature ${SOURCES})
target_include_directories(
    vclib-render-mesh-principal-curvature PUBLIC
        ${VCLIB_INCLUDE_TEST_DIR})

target_link_libraries(vclib-render-mesh-principal-curvature PUBLIC vclib-render vclib-test-common)
