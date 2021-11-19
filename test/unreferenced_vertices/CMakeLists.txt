#*****************************************************************************
#* VCLib                                                             o o     *
#* Visual and Computer Graphics Library                            o     o   *
#*                                                                _   O  _   *
#* Copyright(C) 2021-2022                                           \/)\/    *
#* Visual Computing Lab                                            /\/|      *
#* ISTI - Italian National Research Council                           |      *
#*                                                                    \      *
#* All rights reserved.                                                      *
#****************************************************************************/

cmake_minimum_required(VERSION 3.13)
project(vclib-unreferenced-vertices)

set(SOURCES
	main.cpp)

add_executable(vclib-unreferenced-vertices ${SOURCES})

target_link_libraries(vclib-unreferenced-vertices PUBLIC vclib)
