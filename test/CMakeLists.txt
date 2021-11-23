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

add_definitions(-DVCL_TEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DVCL_TEST_MODELS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/models")
add_definitions(-DVCL_TEST_RESULTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/results")

add_subdirectory(mesh_basic_test)
add_subdirectory(mesh_clean)
add_subdirectory(mesh_topology)
add_subdirectory(load_mesh_ply)
add_subdirectory(laplacian_smoothing)
add_subdirectory(polymesh_basic_test)
