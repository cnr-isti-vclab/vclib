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

project(vclib-mesh-clean)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-clean ${SOURCES})

target_link_libraries(vclib-mesh-clean PUBLIC vclib vclib-test-common)
