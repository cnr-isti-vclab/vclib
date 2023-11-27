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

project(vclib-mesh-export-to-matrix)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-export-to-matrix ${SOURCES})

target_link_libraries(vclib-mesh-export-to-matrix PUBLIC vclib vclib-test-common)
