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

project(vclib-mesh-io-stl)

set(HEADERS
    load_bimba_and_bunny.h)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-io-stl ${HEADERS} ${SOURCES})

target_link_libraries(vclib-mesh-io-stl PUBLIC vclib vclib-test-common)
