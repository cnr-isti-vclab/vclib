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

project(vclib-dcel-basic-test)

set(SOURCES
	main.cpp)

add_executable(vclib-dcel-basic-test ${SOURCES})

target_link_libraries(vclib-dcel-basic-test PUBLIC vclib)
