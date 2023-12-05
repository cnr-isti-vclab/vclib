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

get_target_property(VCG_INCLUDE_DIRS vclib-external-vcg VCG_INCLUDE_DIRS)

set(SOURCES
    mesh.h
    main.cpp
    ${VCG_INCLUDE_DIRS}/wrap/ply/plylib.cpp)

add_executable(vclib-ext-mesh-import-from-vcg-test ${SOURCES})

target_link_libraries(vclib-ext-mesh-import-from-vcg-test PUBLIC vclib vclib-test-common)

enable_testing()
add_test(NAME mesh-import-from-vcg COMMAND vclib-ext-mesh-import-from-vcg-test)
