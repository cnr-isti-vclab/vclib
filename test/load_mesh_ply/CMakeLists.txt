#*****************************************************************************
#* VCLib                                                             o o     *
#* Visual Computing Library                                        o     o   *
#*                                                                _   O  _   *
#* Copyright(C) 2021-2022                                           \/)\/    *
#* Visual Computing Lab                                            /\/|      *
#* ISTI - Italian National Research Council                           |      *
#*                                                                    \      *
#* All rights reserved.                                                      *
#****************************************************************************/

cmake_minimum_required(VERSION 3.13)
project(vclib-load-mesh-ply)

set(SOURCES
	main.cpp)

add_executable(vclib-load-mesh-ply ${SOURCES})

target_link_libraries(vclib-load-mesh-ply PUBLIC vclib)
