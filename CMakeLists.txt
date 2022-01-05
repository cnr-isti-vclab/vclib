#*****************************************************************************
#* VCLib                                                             o o     *
#* Visual Computing Library                                        o     o   *
#*                                                                _   O  _   *
#* Copyright(C) 2021-2022                                           \/)\/    *
#* Visual Computing Lab                                            /\/|      *
#* ISTI - Italian National Research Council                           |      *
#*                                                                    \      *
#* All rights reserved.                                                      *
#****************************************************************************/

cmake_minimum_required(VERSION 3.13)
project(vclib)

### Build settings
set(CMAKE_CXX_STANDARD 17)

### Options
option(VCLIB_BUILD_TESTS "Build VCLib tests" ON)
option(VCLIB_IDE "Create an utility target to show the library tree on IDEs" ON)
option(VCLIB_BUILD_DOCS
	"Build documentation -
	requires doxygen and sphinx with readthedocs theme and breathe extensions" OFF)

# Add the cmake folder so the FindSphinx module is found
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# threads and tbb, required for parallel on linux and macos
if (UNIX)
	find_package(Threads)
	find_package(TBB)
endif()

# external libraries
add_subdirectory(external)

if (WIN32)
	add_compile_definitions(_USE_MATH_DEFINES)
	if (MSVC)
		# remove possible loss of data warnings in msvc, leave the others
		add_compile_options(/w44244 /w44267 /w44305)
	endif()
endif()

set(VCLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(VCLIB_SRC_DIR ${VCLIB_INCLUDE_DIR}/vclib)

set(HEADERS
	include/vclib/edgemesh.h
	include/vclib/pointcloud.h
	include/vclib/polymesh.h
	include/vclib/trimesh.h
	include/vclib/algorithms/clean.h
	include/vclib/algorithms/export_to_matrix.h
	include/vclib/algorithms/fitting.h
	include/vclib/algorithms/polygon.h
	include/vclib/algorithms/smooth.h
	include/vclib/algorithms/stat.h
	include/vclib/algorithms/create/dodecahedron.h
	include/vclib/algorithms/create/hexahedron.h
	include/vclib/algorithms/create/tetrahedron.h
	include/vclib/algorithms/internal/inertia.h
	include/vclib/algorithms/internal/per_face_edge.h
	include/vclib/algorithms/update/bounding_box.h
	include/vclib/algorithms/update/color.h
	include/vclib/algorithms/update/flag.h
	include/vclib/algorithms/update/normal.h
	include/vclib/algorithms/update/scalar.h
	include/vclib/algorithms/update/selection.h
	include/vclib/algorithms/update/topology.h
	include/vclib/algorithms/update/transform.h
	include/vclib/exception/io_exception.h
	include/vclib/exception/mesh_exception.h
	include/vclib/io/file_mesh_info.h
	include/vclib/io/load.h
	include/vclib/io/load_obj.h
	include/vclib/io/load_off.h
	include/vclib/io/load_ply.h
	include/vclib/io/save.h
	include/vclib/io/save_obj.h
	include/vclib/io/save_off.h
	include/vclib/io/save_ply.h
	include/vclib/io/internal/io_read.h
	include/vclib/io/internal/io_utils.h
	include/vclib/io/internal/io_write.h
	include/vclib/io/obj/material.h
	include/vclib/io/off/off.h
	include/vclib/io/ply/ply.h
	include/vclib/io/ply/ply_extra.h
	include/vclib/io/ply/ply_face.h
	include/vclib/io/ply/ply_header.h
	include/vclib/io/ply/ply_tristrip.h
	include/vclib/io/ply/ply_vertex.h
	include/vclib/iterators/container_iterator.h
	include/vclib/iterators/container_range_iterator.h
	include/vclib/iterators/range_iterator.h
	include/vclib/math/base.h
	include/vclib/math/distribution.h
	include/vclib/math/matrix.h
	include/vclib/math/perlin_noise.h
	include/vclib/mesh/mesh.h
	include/vclib/mesh/mesh_argument.h
	include/vclib/mesh/mesh_components.h
	include/vclib/mesh/mesh_pos.h
	include/vclib/mesh/requirements.h
	include/vclib/mesh/components/adjacent_edges.h
	include/vclib/mesh/components/adjacent_faces.h
	include/vclib/mesh/components/adjacent_vertices.h
	include/vclib/mesh/components/bit_flags.h
	include/vclib/mesh/components/bounding_box.h
	include/vclib/mesh/components/color.h
	include/vclib/mesh/components/coordinate.h
	include/vclib/mesh/components/mark.h
	include/vclib/mesh/components/normal.h
	include/vclib/mesh/components/polygon_bit_flags.h
	include/vclib/mesh/components/principal_curvature.h
	include/vclib/mesh/components/scalar.h
	include/vclib/mesh/components/vertex_references.h
	include/vclib/mesh/components/tex_coord.h
	include/vclib/mesh/components/texture_file_names.h
	include/vclib/mesh/components/transform_matrix.h
	include/vclib/mesh/components/triangle_bit_flags.h
	include/vclib/mesh/components/wedge_colors.h
	include/vclib/mesh/components/wedge_tex_coords.h
	include/vclib/mesh/components/detection/adjacent_edges_detection.h
	include/vclib/mesh/components/detection/adjacent_faces_detection.h
	include/vclib/mesh/components/detection/adjacent_vertices_detection.h
	include/vclib/mesh/components/detection/bit_flags_detection.h
	include/vclib/mesh/components/detection/bounding_box_detection.h
	include/vclib/mesh/components/detection/color_detection.h
	include/vclib/mesh/components/detection/coordinate_detection.h
	include/vclib/mesh/components/detection/custom_components_detection.h
	include/vclib/mesh/components/detection/mark_detection.h
	include/vclib/mesh/components/detection/normal_detection.h
	include/vclib/mesh/components/detection/principal_curvature_detection.h
	include/vclib/mesh/components/detection/scalar_detection.h
	include/vclib/mesh/components/detection/tex_coord_detection.h
	include/vclib/mesh/components/detection/tex_file_names_detection.h
	include/vclib/mesh/components/detection/transfrom_matrix_detection.h
	include/vclib/mesh/components/detection/vertex_references_detection.h
	include/vclib/mesh/components/detection/wedge_colors_detection.h
	include/vclib/mesh/components/detection/wedge_tex_coords_detection.h
	include/vclib/mesh/components/internal/element_references.h
	include/vclib/mesh/components/internal/generic_container.h
	include/vclib/mesh/components_optional/custom_components.h
	include/vclib/mesh/components_optional/optional_adjacent_edges.h
	include/vclib/mesh/components_optional/optional_adjacent_faces.h
	include/vclib/mesh/components_optional/optional_adjacent_vertices.h
	include/vclib/mesh/components_optional/optional_color.h
	include/vclib/mesh/components_optional/optional_info.h
	include/vclib/mesh/components_optional/optional_mark.h
	include/vclib/mesh/components_optional/optional_normal.h
	include/vclib/mesh/components_optional/optional_principal_curvature.h
	include/vclib/mesh/components_optional/optional_scalar.h
	include/vclib/mesh/components_optional/optional_tex_coord.h
	include/vclib/mesh/components_optional/optional_wedge_colors.h
	include/vclib/mesh/components_optional/optional_wedge_tex_coords.h
	include/vclib/mesh/components_vector/optional_color_vector.h
	include/vclib/mesh/components_vector/custom_component_vector_handle.h
	include/vclib/mesh/components_vector/custom_components_vector.h
	include/vclib/mesh/components_vector/optional_adjacent_edges_vector.h
	include/vclib/mesh/components_vector/optional_adjacent_faces_vector.h
	include/vclib/mesh/components_vector/optional_adjacent_vertices_vector.h
	include/vclib/mesh/components_vector/optional_generic_vector.h
	include/vclib/mesh/components_vector/optional_mark_vector.h
	include/vclib/mesh/components_vector/optional_normal_vector.h
	include/vclib/mesh/components_vector/optional_principal_curvature_vector.h
	include/vclib/mesh/components_vector/optional_scalar_vector.h
	include/vclib/mesh/components_vector/optional_tex_coord_vector.h
	include/vclib/mesh/components_vector/optional_wedge_colors_vector.h
	include/vclib/mesh/components_vector/optional_wedge_tex_coords_vector.h
	include/vclib/mesh/components_vector/components_vector.h
	include/vclib/mesh/container/containers.h
	include/vclib/mesh/container/containers_detection.h
	include/vclib/mesh/container/edge_container.h
	include/vclib/mesh/container/element_container.h
	include/vclib/mesh/container/face_container.h
	include/vclib/mesh/container/vertex_container.h
	include/vclib/mesh/elements/edge.h
	include/vclib/mesh/elements/edge_components.h
	include/vclib/mesh/elements/edge_components_optional.h
	include/vclib/mesh/elements/face.h
	include/vclib/mesh/elements/face_components.h
	include/vclib/mesh/elements/face_components_optional.h
	include/vclib/mesh/elements/vertex.h
	include/vclib/mesh/elements/vertex_components.h
	include/vclib/mesh/elements/vertex_components_optional.h
	include/vclib/mesh/requirements/element_requirements.h
	include/vclib/mesh/requirements/face_requirements.h
	include/vclib/mesh/requirements/mesh_requirements.h
	include/vclib/mesh/requirements/vertex_requirements.h
	include/vclib/misc/compactness.h
	include/vclib/misc/comparators.h
	include/vclib/misc/file_info.h
	include/vclib/misc/nested_initializer_lists.h
	include/vclib/misc/parallel.h
	include/vclib/misc/string.h
	include/vclib/misc/timer.h
	include/vclib/misc/tokenizer.h
	include/vclib/misc/types.h
	include/vclib/misc/internal/tmp_meshes.h
	include/vclib/space/array.h
	include/vclib/space/box.h
	include/vclib/space/color.h
	include/vclib/space/kd_tree.h
	include/vclib/space/plane.h
	include/vclib/space/point.h
	include/vclib/space/principal_curvature.h
	include/vclib/space/tex_coord.h
	include/vclib/space/array/array_t.h
	include/vclib/space/array/array2.h
	include/vclib/space/array/array3.h
	include/vclib/space/array/array4.h
	include/vclib/space/array/array_bool.h
	include/vclib/space/point/point_t.h
	include/vclib/space/point/point2.h
	include/vclib/space/point/point3.h
	include/vclib/space/point/point4.h)

set(SOURCES
	include/vclib/algorithms/clean.cpp
	include/vclib/algorithms/export_to_matrix.cpp
	include/vclib/algorithms/fitting.cpp
	include/vclib/algorithms/polygon.cpp
	include/vclib/algorithms/smooth.cpp
	include/vclib/algorithms/stat.cpp
	include/vclib/algorithms/create/dodecahedron.cpp
	include/vclib/algorithms/create/hexahedron.cpp
	include/vclib/algorithms/create/tetrahedron.cpp
	include/vclib/algorithms/internal/inertia.cpp
	include/vclib/algorithms/update/bounding_box.cpp
	include/vclib/algorithms/update/color.cpp
	include/vclib/algorithms/update/flag.cpp
	include/vclib/algorithms/update/normal.cpp
	include/vclib/algorithms/update/scalar.cpp
	include/vclib/algorithms/update/selection.cpp
	include/vclib/algorithms/update/topology.cpp
	include/vclib/algorithms/update/transform.cpp
	include/vclib/io/file_mesh_info.cpp
	include/vclib/io/load.cpp
	include/vclib/io/load_obj.cpp
	include/vclib/io/load_off.cpp
	include/vclib/io/load_ply.cpp
	include/vclib/io/save.cpp
	include/vclib/io/save_obj.cpp
	include/vclib/io/save_off.cpp
	include/vclib/io/save_ply.cpp
	include/vclib/io/internal/io_read.cpp
	include/vclib/io/internal/io_utils.cpp
	include/vclib/io/internal/io_write.cpp
	include/vclib/io/obj/material.cpp
	include/vclib/io/off/off.cpp
	include/vclib/io/ply/ply_extra.cpp
	include/vclib/io/ply/ply_face.cpp
	include/vclib/io/ply/ply_header.cpp
	include/vclib/io/ply/ply_tristrip.cpp
	include/vclib/io/ply/ply_vertex.cpp
	include/vclib/iterators/container_iterator.cpp
	include/vclib/math/distribution.cpp
	include/vclib/math/matrix.cpp
	include/vclib/math/perlin_noise.cpp
	include/vclib/mesh/mesh.cpp
	include/vclib/mesh/mesh_pos.cpp
	include/vclib/mesh/components/adjacent_edges.cpp
	include/vclib/mesh/components/adjacent_faces.cpp
	include/vclib/mesh/components/adjacent_vertices.cpp
	include/vclib/mesh/components/bit_flags.cpp
	include/vclib/mesh/components/bounding_box.cpp
	include/vclib/mesh/components/color.cpp
	include/vclib/mesh/components/coordinate.cpp
	include/vclib/mesh/components/mark.cpp
	include/vclib/mesh/components/normal.cpp
	include/vclib/mesh/components/polygon_bit_flags.cpp
	include/vclib/mesh/components/principal_curvature.cpp
	include/vclib/mesh/components/scalar.cpp
	include/vclib/mesh/components/tex_coord.cpp
	include/vclib/mesh/components/texture_file_names.cpp
	include/vclib/mesh/components/transform_matrix.cpp
	include/vclib/mesh/components/triangle_bit_flags.cpp
	include/vclib/mesh/components/vertex_references.cpp
	include/vclib/mesh/components/wedge_colors.cpp
	include/vclib/mesh/components/wedge_tex_coords.cpp
	include/vclib/mesh/components/internal/element_references.cpp
	include/vclib/mesh/components/internal/generic_container.cpp
	include/vclib/mesh/components_optional/custom_components.cpp
	include/vclib/mesh/components_optional/optional_adjacent_edges.cpp
	include/vclib/mesh/components_optional/optional_adjacent_faces.cpp
	include/vclib/mesh/components_optional/optional_adjacent_vertices.cpp
	include/vclib/mesh/components_optional/optional_color.cpp
	include/vclib/mesh/components_optional/optional_mark.cpp
	include/vclib/mesh/components_optional/optional_normal.cpp
	include/vclib/mesh/components_optional/optional_principal_curvature.cpp
	include/vclib/mesh/components_optional/optional_scalar.cpp
	include/vclib/mesh/components_optional/optional_tex_coord.cpp
	include/vclib/mesh/components_optional/optional_wedge_colors.cpp
	include/vclib/mesh/components_optional/optional_wedge_tex_coords.cpp
	include/vclib/mesh/components_vector/custom_component_vector_handle.cpp
	include/vclib/mesh/components_vector/custom_components_vector.cpp
	include/vclib/mesh/components_vector/optional_generic_vector.cpp
	include/vclib/mesh/container/edge_container.cpp
	include/vclib/mesh/container/face_container.cpp
	include/vclib/mesh/container/vertex_container.cpp
	include/vclib/mesh/elements/edge.cpp
	include/vclib/mesh/elements/face.cpp
	include/vclib/mesh/elements/vertex.cpp
	include/vclib/mesh/requirements/element_requirements.cpp
	include/vclib/mesh/requirements/face_requirements.cpp
	include/vclib/mesh/requirements/mesh_requirements.cpp
	include/vclib/mesh/requirements/vertex_requirements.cpp
	include/vclib/misc/compactness.cpp
	include/vclib/misc/file_info.cpp
	include/vclib/misc/nested_initializer_lists.cpp
	include/vclib/misc/parallel.cpp
	include/vclib/misc/string.cpp
	include/vclib/misc/timer.cpp
	include/vclib/misc/tokenizer.cpp
	include/vclib/space/box.cpp
	include/vclib/space/color.cpp
	include/vclib/space/kd_tree.cpp
	include/vclib/space/plane.cpp
	include/vclib/space/principal_curvature.cpp
	include/vclib/space/point.cpp
	include/vclib/space/tex_coord.cpp
	include/vclib/space/array/array_t.cpp
	include/vclib/space/array/array2.cpp
	include/vclib/space/array/array3.cpp
	include/vclib/space/array/array4.cpp
	include/vclib/space/point/point_t.cpp
	include/vclib/space/point/point2.cpp
	include/vclib/space/point/point3.cpp
	include/vclib/space/point/point4.cpp)

add_library(vclib INTERFACE)

target_include_directories(vclib INTERFACE ${VCLIB_INCLUDE_DIR})
target_link_libraries(vclib INTERFACE vclib-external-eigen vclib-external-mapbox-earcut)

if (UNIX)
	if (TARGET TBB::tbb AND TARGET Threads::Threads)
		message(STATUS "VCLIB using parallel algorithms")
		target_link_libraries(vclib INTERFACE TBB::tbb Threads::Threads)
	else()
		message(STATUS "VCLIB not using parallel algorithms")
	endif()
else() # natively supported by msvc
	message(STATUS "VCLIB using parallel algorithms")
endif()

if (VCLIB_IDE)
	add_custom_target(vclib_ide SOURCES ${HEADERS} ${SOURCES})
endif()

if (VCLIB_BUILD_TESTS)
	add_subdirectory(test)
endif()

if (VCLIB_BUILD_DOCS)
	add_subdirectory(docs)
endif()
