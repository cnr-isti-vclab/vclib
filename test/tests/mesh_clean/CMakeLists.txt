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

project(vclib-mesh-clean-test)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-clean-test ${SOURCES})

target_link_libraries(vclib-mesh-clean-test PUBLIC vclib)

enable_testing()
add_test(NAME mesh-clean COMMAND vclib-mesh-clean-test)
