#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

cmake_minimum_required(VERSION 3.13)
project(vclib-test)

add_definitions(-DVCL_TEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DVCL_TEST_MODELS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/models")
add_definitions(-DVCL_TEST_RESULTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/results")

# examples
if (VCLIB_BUILD_EXAMPLES)
	add_subdirectory(examples/distance)
	add_subdirectory(examples/edge_mesh_basic)
	add_subdirectory(examples/grid)
	add_subdirectory(examples/misc)
	add_subdirectory(examples/mesh_basic)
	add_subdirectory(examples/mesh_clean)
	add_subdirectory(examples/mesh_conversion)
	add_subdirectory(examples/mesh_create)
	add_subdirectory(examples/mesh_custom_components)
	add_subdirectory(examples/mesh_export_to_matrix)
	add_subdirectory(examples/mesh_generate)
	add_subdirectory(examples/mesh_iterators)
	add_subdirectory(examples/mesh_io_obj)
	add_subdirectory(examples/mesh_io_off)
	add_subdirectory(examples/mesh_io_ply)
	add_subdirectory(examples/mesh_io_stl)
	add_subdirectory(examples/mesh_normal)
	add_subdirectory(examples/mesh_principal_curvature)
	add_subdirectory(examples/mesh_sampling)
	add_subdirectory(examples/mesh_smooth)
	add_subdirectory(examples/mesh_stat)
	add_subdirectory(examples/mesh_topology)
	add_subdirectory(examples/mesh_transform)
	add_subdirectory(examples/polygon_earcut)
	add_subdirectory(examples/polymesh_basic)
	add_subdirectory(examples/static_asserts)

	if (TARGET vclib-external-qglviewer)
		add_subdirectory(examples/ext/qglviewer/viewer)
	endif()
endif()



# tests
if (VCLIB_BUILD_AND_RUN_TESTS)
	add_subdirectory(tests/kd_tree)
	add_subdirectory(tests/mesh_conversions)
	add_subdirectory(tests/mesh_clean)
	add_subdirectory(tests/mesh_custom_components)
	add_subdirectory(tests/mesh_filter)
	add_subdirectory(tests/mesh_topology)
	add_subdirectory(tests/trimesh_base)

	if (TARGET vclib-external-vcg)
		add_subdirectory(tests/ext/vcg/mesh_import_vcg)
	endif()
endif()

