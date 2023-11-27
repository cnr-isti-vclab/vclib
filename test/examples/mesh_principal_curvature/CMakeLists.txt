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

project(vclib-mesh-principal-curvature)

set(HEADERS
    mesh_curvature.h)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-principal-curvature ${HEADERS} ${SOURCES})

target_link_libraries(vclib-mesh-principal-curvature PUBLIC vclib vclib-test-common)
