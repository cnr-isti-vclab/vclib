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

project(vclib-ext-qglviewer-viewer)

set(SOURCES
    main.cpp)

add_executable(vclib-ext-qglviewer-viewer ${SOURCES})

target_link_libraries(vclib-ext-qglviewer-viewer PUBLIC vclib vclib-render vclib-test-common)
