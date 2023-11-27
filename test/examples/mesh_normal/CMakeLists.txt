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

project(vclib-mesh-normal)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-normal ${SOURCES})

target_link_libraries(vclib-mesh-normal PUBLIC vclib vclib-test-common)
