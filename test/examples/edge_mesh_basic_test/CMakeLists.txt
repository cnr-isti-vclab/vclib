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

project(vclib-edge-mesh-basic-test)

set(SOURCES
	main.cpp)

add_executable(vclib-edge-mesh-basic-test ${SOURCES})

target_link_libraries(vclib-edge-mesh-basic-test PUBLIC vclib)
