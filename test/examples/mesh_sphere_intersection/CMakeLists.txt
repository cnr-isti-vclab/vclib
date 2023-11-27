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

project(vclib-mesh-sphere-intersection)

set(HEADERS
    bimba_sphere_intersection.h)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-sphere-intersection ${HEADERS} ${SOURCES})

target_link_libraries(vclib-mesh-sphere-intersection PUBLIC vclib vclib-test-common)
