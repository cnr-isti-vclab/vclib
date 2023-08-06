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

project(vclib-mesh-filter-test)

set(SOURCES
	main.cpp)

add_executable(vclib-mesh-filter-test ${SOURCES})

target_link_libraries(vclib-mesh-filter-test PUBLIC vclib)

enable_testing()
add_test(NAME mesh-filter COMMAND vclib-mesh-filter-test)
