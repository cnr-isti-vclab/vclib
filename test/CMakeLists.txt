#*****************************************************************************
#* VCLib                                                             o o     *
#* Visual and Computer Graphics Library                            o     o   *
#*                                                                _   O  _   *
#* Copyright(C) 2021-2022                                           \/)\/    *
#* Visual Computing Lab                                            /\/|      *
#* ISTI - Italian National Research Council                           |      *
#*                                                                    \      *
#* All rights reserved.                                                      *
#****************************************************************************/

cmake_minimum_required(VERSION 3.13)
project(vclib-test)

add_subdirectory(point2_example)
add_subdirectory(point3_example)
add_subdirectory(point4_example)
add_subdirectory(mesh_example)
add_subdirectory(load_mesh_ply)
add_subdirectory(laplacian_smoothing)
add_subdirectory(unreferenced_vertices)
