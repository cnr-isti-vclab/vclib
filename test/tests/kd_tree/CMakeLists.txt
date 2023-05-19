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

project(vclib-kd-tree-test)

set(SOURCES
	main.cpp)

add_executable(vclib-kd-tree-test ${SOURCES})

target_link_libraries(vclib-kd-tree-test PUBLIC vclib)

enable_testing()
add_test(NAME kd-tree COMMAND vclib-kd-tree-test)
