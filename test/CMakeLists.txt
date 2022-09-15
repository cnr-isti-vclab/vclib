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
project(vclib-test)

add_definitions(-DVCL_TEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DVCL_TEST_MODELS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/models")
add_definitions(-DVCL_TEST_RESULTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/results")

add_subdirectory(kd_tree)
add_subdirectory(dcel_basic_test)
add_subdirectory(edge_mesh_basic_test)
add_subdirectory(mesh_basic_test)
add_subdirectory(mesh_clean)
add_subdirectory(mesh_conversion)
add_subdirectory(mesh_create)
add_subdirectory(mesh_custom_components)
add_subdirectory(mesh_export_to_matrix)
add_subdirectory(mesh_load_obj)
add_subdirectory(mesh_load_off)
add_subdirectory(mesh_load_ply)
add_subdirectory(mesh_normal)
add_subdirectory(mesh_smooth)
add_subdirectory(mesh_stat)
add_subdirectory(mesh_topology)
add_subdirectory(mesh_transform)
add_subdirectory(polygon_earcut)
add_subdirectory(polymesh_basic_test)
