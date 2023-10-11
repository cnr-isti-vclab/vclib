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

project(vclib-mesh-conversions-test)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-conversions-test ${SOURCES})

target_link_libraries(vclib-mesh-conversions-test PUBLIC vclib)

enable_testing()
add_test(NAME mesh-conversions COMMAND vclib-mesh-conversions-test)
