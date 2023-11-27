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

project(vclib-mesh-sampling)

set(HEADERS
    montecarlo_sampling.h)

set(SOURCES
    main.cpp)

add_executable(vclib-mesh-sampling ${HEADERS} ${SOURCES})

target_link_libraries(vclib-mesh-sampling PUBLIC vclib vclib-test-common)
