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

project(vclib-polymesh-basic-example)

set(SOURCES
    main.cpp)

add_executable(vclib-polymesh-basic-example ${SOURCES})

target_link_libraries(vclib-polymesh-basic-example PUBLIC vclib vclib-test-common)
