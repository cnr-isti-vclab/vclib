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

project(vclib-mesh-new-user-component)

set(HEADERS
    bar_component.h
    foo_component.h
    foo_mesh.h)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-new-user-component ${HEADERS} ${SOURCES})

target_link_libraries(vclib-mesh-new-user-component PUBLIC vclib)
