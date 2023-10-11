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

project(vclib-mesh-topology-test)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-topology-test ${SOURCES})

target_link_libraries(vclib-mesh-topology-test PUBLIC vclib)

enable_testing()
add_test(NAME mesh-topology COMMAND vclib-mesh-topology-test)
