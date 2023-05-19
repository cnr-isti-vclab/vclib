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

project(vclib-mesh-io-stl)

set(SOURCES
	main.cpp)

add_executable(vclib-mesh-io-stl ${SOURCES})

target_link_libraries(vclib-mesh-io-stl PUBLIC vclib)
