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

project(vclib-mesh-new-user-component-test)

set(HEADERS
	foo_component.h
	foo_mesh.h)

set(SOURCES
	main.cpp)

add_executable(vclib-mesh-new-user-component-test ${HEADERS} ${SOURCES})

target_link_libraries(vclib-mesh-new-user-component-test PUBLIC vclib)

enable_testing()
add_test(NAME mesh-new-user-component-test
	COMMAND vclib-mesh-new-user-component-test-test)
