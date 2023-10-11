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

project(vclib-mesh-basic-example)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-basic-example ${SOURCES})

target_link_libraries(vclib-mesh-basic-example PUBLIC vclib)
