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

cmake_minimum_required(VERSION 3.13)
project(vclib-mesh-conversion)

set(SOURCES
	main.cpp)

add_executable(vclib-mesh-conversion ${SOURCES})

target_link_libraries(vclib-mesh-conversion PUBLIC vclib)
