/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *

cmake_minimum_required(VERSION 3.13)
project(vclib-mesh-load-off)

set(SOURCES
	main.cpp)

add_executable(vclib-mesh-load-off ${SOURCES})

target_link_libraries(vclib-mesh-load-off PUBLIC vclib)
