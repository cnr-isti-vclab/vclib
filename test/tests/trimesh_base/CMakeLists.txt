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

project(vclib-trimesh-base-test)

set(SOURCES
	main.cpp)

add_executable(vclib-trimesh-base-test ${SOURCES})

target_link_libraries(vclib-trimesh-base-test PUBLIC vclib)

enable_testing()
add_test(NAME trimesh-base COMMAND vclib-trimesh-base-test)
