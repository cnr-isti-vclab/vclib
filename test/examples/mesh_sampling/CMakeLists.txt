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

cmake_minimum_required(VERSION 3.24)

set(EXAMPLE_NAME mesh-sampling)
project(vclib-render-${EXAMPLE_NAME}-example)

set(SOURCES
    main.cpp)

vclib_render_add_example(${EXAMPLE_NAME} TRUE ${SOURCES})
