#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

project(vclib-mesh-stat)

set(SOURCES
	main.cpp)

add_executable(vclib-mesh-stat ${SOURCES})

target_link_libraries(vclib-mesh-stat PUBLIC vclib)
