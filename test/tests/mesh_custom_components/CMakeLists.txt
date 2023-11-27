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

project(vclib-mesh-custom-components-test)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-custom-components-test ${SOURCES})

target_link_libraries(vclib-mesh-custom-components-test PUBLIC vclib vclib-test-common)

enable_testing()
add_test(NAME mesh-custom-components COMMAND vclib-mesh-custom-components-test)
