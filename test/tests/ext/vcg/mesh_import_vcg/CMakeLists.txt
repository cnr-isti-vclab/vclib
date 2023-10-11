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

project(vclib-ext-mesh-import-from-vcg-test)

set(SOURCES
    mesh.h
    main.cpp
    ${VCG_INCLUDE_DIRS}/wrap/ply/plylib.cpp)

add_executable(vclib-ext-mesh-import-from-vcg-test ${SOURCES})

target_link_libraries(vclib-ext-mesh-import-from-vcg-test PUBLIC vclib)

enable_testing()
add_test(NAME mesh-import-from-vcg COMMAND vclib-ext-mesh-import-from-vcg-test)
