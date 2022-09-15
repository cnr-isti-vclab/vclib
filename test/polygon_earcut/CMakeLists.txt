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
project(vclib-polygon-earcut-test)

set(SOURCES
	main.cpp)

add_executable(vclib-polygon-earcut-test ${SOURCES})

target_link_libraries(vclib-polygon-earcut-test PUBLIC vclib)
