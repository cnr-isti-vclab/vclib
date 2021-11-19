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
project(vclib-point2-example)

set(SOURCES
	main.cpp)

add_executable(vclib-point2-example ${SOURCES})

target_link_libraries(vclib-point2-example PUBLIC vclib)
