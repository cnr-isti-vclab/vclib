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
project(vclib)

### Build settings
set(CMAKE_CXX_STANDARD 20)

### Options
option(VCLIB_BUILD_TESTS "Build VCLib tests" ON)
option(VCLIB_IDE "Create an utility target to show the library tree on IDEs" ON)
option(VCLIB_BUILD_DOCS
	"Build documentation -
	requires doxygen and sphinx with readthedocs theme and breathe extensions" OFF)

# In case of building shared libraries, on windows we need to export all symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Add the cmake folder for find_package scripts
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# external libraries
message(STATUS "VCLib: external libraries")
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
	include/vclib/dcel_mesh.h
	include/vclib/edge_mesh.h
	include/vclib/point_cloud.h
	include/vclib/poly_mesh.h
	include/vclib/tri_mesh.h
	include/vclib/algorithms/clean.h
	include/vclib/algorithms/export_to_matrix.h
	include/vclib/algorithms/fitting.h
	include/vclib/algorithms/grid.h
	include/vclib/algorithms/intersection.h
	include/vclib/algorithms/polygon.h
	include/vclib/algorithms/smooth.h
	include/vclib/algorithms/sort.h
	include/vclib/algorithms/stat.h
	include/vclib/algorithms/create/dodecahedron.h
	include/vclib/algorithms/create/hexahedron.h
	include/vclib/algorithms/create/tetrahedron.h
	include/vclib/algorithms/update/bounding_box.h
	include/vclib/algorithms/update/color.h
	include/vclib/algorithms/update/curvature.h
	include/vclib/algorithms/update/flag.h
	include/vclib/algorithms/update/normal.h
	include/vclib/algorithms/update/scalar.h
	include/vclib/algorithms/update/selection.h
	include/vclib/algorithms/update/topology.h
	include/vclib/algorithms/update/transform.h
	include/vclib/exception/io_exception.h
	include/vclib/exception/mesh_exception.h
	include/vclib/exception/misc_exception.h
	include/vclib/image/image.h
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
	include/vclib/io/ply/ply_edge.h
	include/vclib/io/ply/ply_extra.h
	include/vclib/io/ply/ply_face.h
	include/vclib/io/ply/ply_header.h
	include/vclib/io/ply/ply_tristrip.h
	include/vclib/io/ply/ply_vertex.h
	include/vclib/iterators/container_iterator.h
	include/vclib/iterators/container_range_iterator.h
	include/vclib/iterators/range_iterator.h
	include/vclib/iterators/half_edge/face_adj_face_iterator.h
	include/vclib/iterators/half_edge/face_base_iterator.h
	include/vclib/iterators/half_edge/face_half_edge_iterator.h
	include/vclib/iterators/half_edge/face_vertex_iterator.h
	include/vclib/iterators/half_edge/face_wedge_color_iterator.h
	include/vclib/iterators/half_edge/face_wedge_tex_coord_iterator.h
	include/vclib/iterators/half_edge/vertex_adj_face_iterator.h
	include/vclib/iterators/half_edge/vertex_adj_vertex_iterator.h
	include/vclib/iterators/half_edge/vertex_base_iterator.h
	include/vclib/math/base.h
	include/vclib/math/distribution.h
	include/vclib/math/matrix.h
	include/vclib/math/perlin_noise.h
	include/vclib/mesh/mesh.h
	include/vclib/mesh/mesh_algorithms.h
	include/vclib/mesh/mesh_components.h
	include/vclib/mesh/requirements.h
	include/vclib/mesh/components/concepts/adjacent_edges.h
	include/vclib/mesh/components/concepts/adjacent_faces.h
	include/vclib/mesh/components/concepts/adjacent_vertices.h
	include/vclib/mesh/components/concepts/bit_flags.h
	include/vclib/mesh/components/concepts/bounding_box.h
	include/vclib/mesh/components/concepts/color.h
	include/vclib/mesh/components/concepts/coordinate.h
	include/vclib/mesh/components/concepts/custom_components.h
	include/vclib/mesh/components/concepts/face_half_edge_reference.h
	include/vclib/mesh/components/concepts/half_edge_references.h
	include/vclib/mesh/components/concepts/mark.h
	include/vclib/mesh/components/concepts/name.h
	include/vclib/mesh/components/concepts/normal.h
	include/vclib/mesh/components/concepts/principal_curvature.h
	include/vclib/mesh/components/concepts/scalar.h
	include/vclib/mesh/components/concepts/tex_coord.h
	include/vclib/mesh/components/concepts/texture_paths.h
	include/vclib/mesh/components/concepts/transform_matrix.h
	include/vclib/mesh/components/concepts/vertex_half_edge_reference.h
	include/vclib/mesh/components/concepts/vertex_references.h
	include/vclib/mesh/components/concepts/wedge_colors.h
	include/vclib/mesh/components/concepts/wedge_tex_coords.h
	include/vclib/mesh/components/horizontal/adjacent_edges.h
	include/vclib/mesh/components/horizontal/adjacent_faces.h
	include/vclib/mesh/components/horizontal/adjacent_vertices.h
	include/vclib/mesh/components/horizontal/bit_flags.h
	include/vclib/mesh/components/horizontal/bounding_box.h
	include/vclib/mesh/components/horizontal/color.h
	include/vclib/mesh/components/horizontal/coordinate.h
	include/vclib/mesh/components/horizontal/face_half_edge_reference.h
	include/vclib/mesh/components/horizontal/half_edge_references.h
	include/vclib/mesh/components/horizontal/mark.h
	include/vclib/mesh/components/horizontal/name.h
	include/vclib/mesh/components/horizontal/normal.h
	include/vclib/mesh/components/horizontal/polygon_bit_flags.h
	include/vclib/mesh/components/horizontal/principal_curvature.h
	include/vclib/mesh/components/horizontal/scalar.h
	include/vclib/mesh/components/horizontal/vertex_half_edge_reference.h
	include/vclib/mesh/components/horizontal/vertex_references.h
	include/vclib/mesh/components/horizontal/tex_coord.h
	include/vclib/mesh/components/horizontal/texture_paths.h
	include/vclib/mesh/components/horizontal/transform_matrix.h
	include/vclib/mesh/components/horizontal/triangle_bit_flags.h
	include/vclib/mesh/components/horizontal/wedge_colors.h
	include/vclib/mesh/components/horizontal/wedge_tex_coords.h
	include/vclib/mesh/components/horizontal/internal/element_references.h
	include/vclib/mesh/components/horizontal/internal/generic_container.h
	include/vclib/mesh/components/vertical/vertical_component.h
	include/vclib/mesh/components/vertical/optional/optional_adjacent_edges.h
	include/vclib/mesh/components/vertical/optional/optional_adjacent_faces.h
	include/vclib/mesh/components/vertical/optional/optional_adjacent_vertices.h
	include/vclib/mesh/components/vertical/optional/optional_color.h
	include/vclib/mesh/components/vertical/optional/optional_mark.h
	include/vclib/mesh/components/vertical/optional/optional_normal.h
	include/vclib/mesh/components/vertical/optional/optional_principal_curvature.h
	include/vclib/mesh/components/vertical/optional/optional_scalar.h
	include/vclib/mesh/components/vertical/optional/optional_tex_coord.h
	include/vclib/mesh/components/vertical/optional/optional_wedge_colors.h
	include/vclib/mesh/components/vertical/optional/optional_wedge_tex_coords.h
	include/vclib/mesh/components/vertical/persistent/custom_components.h
	include/vclib/mesh/components/vertical/vectors/adjacent_edges_vector.h
	include/vclib/mesh/components/vertical/vectors/adjacent_faces_vector.h
	include/vclib/mesh/components/vertical/vectors/adjacent_vertices_vector.h
	include/vclib/mesh/components/vertical/vectors/color_vector.h
	include/vclib/mesh/components/vertical/vectors/custom_component_vector_handle.h
	include/vclib/mesh/components/vertical/vectors/custom_components_vector.h
	include/vclib/mesh/components/vertical/vectors/generic_component_vector.h
	include/vclib/mesh/components/vertical/vectors/mark_vector.h
	include/vclib/mesh/components/vertical/vectors/normal_vector.h
	include/vclib/mesh/components/vertical/vectors/principal_curvature_vector.h
	include/vclib/mesh/components/vertical/vectors/scalar_vector.h
	include/vclib/mesh/components/vertical/vectors/tex_coord_vector.h
	include/vclib/mesh/components/vertical/vectors/vertical_components_vector.h
	include/vclib/mesh/components/vertical/vectors/wedge_colors_vector.h
	include/vclib/mesh/components/vertical/vectors/wedge_tex_coords_vector.h
	include/vclib/mesh/containers/containers.h
	include/vclib/mesh/containers/containers_concepts.h
	include/vclib/mesh/containers/edge_container.h
	include/vclib/mesh/containers/element_container.h
	include/vclib/mesh/containers/face_container.h
	include/vclib/mesh/containers/half_edge_container.h
	include/vclib/mesh/containers/vertex_container.h
	include/vclib/mesh/elements/edge.h
	include/vclib/mesh/elements/edge_components.h
	include/vclib/mesh/elements/edge_components_optional.h
	include/vclib/mesh/elements/edge_concept.h
	include/vclib/mesh/elements/face.h
	include/vclib/mesh/elements/face_components.h
	include/vclib/mesh/elements/face_components_optional.h
	include/vclib/mesh/elements/face_concept.h
	include/vclib/mesh/elements/half_edge.h
	include/vclib/mesh/elements/half_edge_components.h
	include/vclib/mesh/elements/half_edge_components_optional.h
	include/vclib/mesh/elements/half_edge_concept.h
	include/vclib/mesh/elements/vertex.h
	include/vclib/mesh/elements/vertex_components.h
	include/vclib/mesh/elements/vertex_components_optional.h
	include/vclib/mesh/elements/vertex_concept.h
	include/vclib/mesh/requirements/element_concepts.h
	include/vclib/mesh/requirements/face_concepts.h
	include/vclib/mesh/requirements/face_requirements.h
	include/vclib/mesh/requirements/half_edge_concepts.h
	include/vclib/mesh/requirements/half_edge_requirements.h
	include/vclib/mesh/requirements/mesh_concepts.h
	include/vclib/mesh/requirements/mesh_requirements.h
	include/vclib/mesh/requirements/vertex_concepts.h
	include/vclib/mesh/requirements/vertex_requirements.h
	include/vclib/mesh_utils/mesh_edge_util.h
	include/vclib/mesh_utils/mesh_inertia.h
	include/vclib/mesh_utils/mesh_pos.h
	include/vclib/mesh_utils/tri_poly_index_bimap.h
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
	include/vclib/render/drawable_object.h
	include/vclib/render/drawable_object_vector.h
	include/vclib/render/generic_drawable_mesh.h
	include/vclib/render/mesh_render_buffers.h
	include/vclib/render/mesh_render_settings.h
	include/vclib/space/array.h
	include/vclib/space/box.h
	include/vclib/space/color.h
	include/vclib/space/grid.h
	include/vclib/space/kd_tree.h
	include/vclib/space/plane.h
	include/vclib/space/point.h
	include/vclib/space/principal_curvature.h
	include/vclib/space/sampler.h
	include/vclib/space/tex_coord.h
	include/vclib/space/array/array_t.h
	include/vclib/space/array/array2.h
	include/vclib/space/array/array3.h
	include/vclib/space/array/array4.h
	include/vclib/space/array/array_bool.h
	include/vclib/space/grid/grid_t.h
	include/vclib/space/point/point_t.h
	include/vclib/space/point/point2.h
	include/vclib/space/point/point3.h
	include/vclib/space/point/point4.h
	include/vclib/space/sampler/mesh_sampler.h
	include/vclib/space/sampler/point_sampler.h
	include/vclib/space/sampler/sampler_concept.h
	include/vclib/space/sampler/vertex_index_sampler.h)

set(SOURCES
	include/vclib/algorithms/clean.cpp
	include/vclib/algorithms/export_to_matrix.cpp
	include/vclib/algorithms/fitting.cpp
	include/vclib/algorithms/grid.cpp
	include/vclib/algorithms/intersection.cpp
	include/vclib/algorithms/polygon.cpp
	include/vclib/algorithms/sort.cpp
	include/vclib/algorithms/smooth.cpp
	include/vclib/algorithms/stat.cpp
	include/vclib/algorithms/create/dodecahedron.cpp
	include/vclib/algorithms/create/hexahedron.cpp
	include/vclib/algorithms/create/tetrahedron.cpp
	include/vclib/algorithms/update/bounding_box.cpp
	include/vclib/algorithms/update/color.cpp
	include/vclib/algorithms/update/curvature.cpp
	include/vclib/algorithms/update/flag.cpp
	include/vclib/algorithms/update/normal.cpp
	include/vclib/algorithms/update/scalar.cpp
	include/vclib/algorithms/update/selection.cpp
	include/vclib/algorithms/update/topology.cpp
	include/vclib/algorithms/update/transform.cpp
	include/vclib/image/image.cpp
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
	include/vclib/io/ply/ply_edge.cpp
	include/vclib/io/ply/ply_extra.cpp
	include/vclib/io/ply/ply_face.cpp
	include/vclib/io/ply/ply_header.cpp
	include/vclib/io/ply/ply_tristrip.cpp
	include/vclib/io/ply/ply_vertex.cpp
	include/vclib/iterators/container_iterator.cpp
	include/vclib/iterators/half_edge/face_base_iterator.cpp
	include/vclib/iterators/half_edge/vertex_base_iterator.cpp
	include/vclib/math/distribution.cpp
	include/vclib/math/matrix.cpp
	include/vclib/math/perlin_noise.cpp
	include/vclib/mesh/mesh.cpp
	include/vclib/mesh/mesh_algorithms.cpp
	include/vclib/mesh/components/horizontal/adjacent_edges.cpp
	include/vclib/mesh/components/horizontal/adjacent_faces.cpp
	include/vclib/mesh/components/horizontal/adjacent_vertices.cpp
	include/vclib/mesh/components/horizontal/bit_flags.cpp
	include/vclib/mesh/components/horizontal/bounding_box.cpp
	include/vclib/mesh/components/horizontal/color.cpp
	include/vclib/mesh/components/horizontal/coordinate.cpp
	include/vclib/mesh/components/horizontal/face_half_edge_reference.cpp
	include/vclib/mesh/components/horizontal/half_edge_references.cpp
	include/vclib/mesh/components/horizontal/mark.cpp
	include/vclib/mesh/components/horizontal/name.cpp
	include/vclib/mesh/components/horizontal/normal.cpp
	include/vclib/mesh/components/horizontal/polygon_bit_flags.cpp
	include/vclib/mesh/components/horizontal/principal_curvature.cpp
	include/vclib/mesh/components/horizontal/scalar.cpp
	include/vclib/mesh/components/horizontal/tex_coord.cpp
	include/vclib/mesh/components/horizontal/texture_paths.cpp
	include/vclib/mesh/components/horizontal/transform_matrix.cpp
	include/vclib/mesh/components/horizontal/triangle_bit_flags.cpp
	include/vclib/mesh/components/horizontal/vertex_half_edge_reference.cpp
	include/vclib/mesh/components/horizontal/vertex_references.cpp
	include/vclib/mesh/components/horizontal/wedge_colors.cpp
	include/vclib/mesh/components/horizontal/wedge_tex_coords.cpp
	include/vclib/mesh/components/horizontal/internal/element_references.cpp
	include/vclib/mesh/components/horizontal/internal/generic_container.cpp
	include/vclib/mesh/components/vertical/optional/optional_adjacent_edges.cpp
	include/vclib/mesh/components/vertical/optional/optional_adjacent_faces.cpp
	include/vclib/mesh/components/vertical/optional/optional_adjacent_vertices.cpp
	include/vclib/mesh/components/vertical/optional/optional_color.cpp
	include/vclib/mesh/components/vertical/optional/optional_mark.cpp
	include/vclib/mesh/components/vertical/optional/optional_normal.cpp
	include/vclib/mesh/components/vertical/optional/optional_principal_curvature.cpp
	include/vclib/mesh/components/vertical/optional/optional_scalar.cpp
	include/vclib/mesh/components/vertical/optional/optional_tex_coord.cpp
	include/vclib/mesh/components/vertical/optional/optional_wedge_colors.cpp
	include/vclib/mesh/components/vertical/optional/optional_wedge_tex_coords.cpp
	include/vclib/mesh/components/vertical/persistent/custom_components.cpp
	include/vclib/mesh/components/vertical/vectors/custom_component_vector_handle.cpp
	include/vclib/mesh/components/vertical/vectors/custom_components_vector.cpp
	include/vclib/mesh/components/vertical/vectors/generic_component_vector.cpp
	include/vclib/mesh/containers/edge_container.cpp
	include/vclib/mesh/containers/element_container.cpp
	include/vclib/mesh/containers/half_edge_container.cpp
	include/vclib/mesh/containers/face_container.cpp
	include/vclib/mesh/containers/vertex_container.cpp
	include/vclib/mesh/elements/edge.cpp
	include/vclib/mesh/elements/face.cpp
	include/vclib/mesh/elements/half_edge.cpp
	include/vclib/mesh/elements/vertex.cpp
	include/vclib/mesh/requirements/face_requirements.cpp
	include/vclib/mesh/requirements/half_edge_requirements.cpp
	include/vclib/mesh/requirements/mesh_requirements.cpp
	include/vclib/mesh/requirements/vertex_requirements.cpp
	include/vclib/mesh_utils/mesh_inertia.cpp
	include/vclib/mesh_utils/mesh_pos.cpp
	include/vclib/mesh_utils/tri_poly_index_bimap.cpp
	include/vclib/misc/compactness.cpp
	include/vclib/misc/file_info.cpp
	include/vclib/misc/nested_initializer_lists.cpp
	include/vclib/misc/parallel.cpp
	include/vclib/misc/string.cpp
	include/vclib/misc/timer.cpp
	include/vclib/misc/tokenizer.cpp
	include/vclib/render/drawable_object_vector.cpp
	include/vclib/render/generic_drawable_mesh.cpp
	include/vclib/render/mesh_render_buffers.cpp
	include/vclib/render/mesh_render_settings.cpp
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
	include/vclib/space/grid/grid_t.cpp
	include/vclib/space/point/point_t.cpp
	include/vclib/space/point/point2.cpp
	include/vclib/space/point/point3.cpp
	include/vclib/space/point/point4.cpp
	include/vclib/space/sampler/mesh_sampler.cpp
	include/vclib/space/sampler/point_sampler.cpp
	include/vclib/space/sampler/vertex_index_sampler.cpp)

set(HEADERS_LIB
	)

set(SOURCES_LIB
	)

set (FORMS_LIB
	)

# headers and sources to include only if using OpenGL
if (TARGET vclib-external-opengl)
	list(APPEND HEADERS
		include/vclib/ext/opengl2/draw_objects2.h
		include/vclib/ext/opengl2/draw_objects3.h
		include/vclib/ext/opengl2/drawable_mesh.h)

	list(APPEND SOURCES
		include/vclib/ext/opengl2/draw_objects2.cpp
		include/vclib/ext/opengl2/draw_objects3.cpp
		include/vclib/ext/opengl2/drawable_mesh.cpp)
endif()

# headers and sources to include only if using qt
if (TARGET vclib-external-qt)
	# header only sources
	list(APPEND HEADERS
		)

	list(APPEND SOURCES
		)

	# sources that must be built
	list(APPEND HEADERS_LIB
		include/vclib/ext/qt/gui/drawable_object_frame.h
		include/vclib/ext/qt/gui/drawable_object_vector_frame.h
		include/vclib/ext/qt/gui/mesh_render_settings_frame.h
		include/vclib/ext/qt/gui/q_clickable_label.h)

	list(APPEND SOURCES_LIB
		include/vclib/ext/qt/gui/drawable_object_frame.cpp
		include/vclib/ext/qt/gui/drawable_object_vector_frame.cpp
		include/vclib/ext/qt/gui/mesh_render_settings_frame.cpp
		include/vclib/ext/qt/gui/q_clickable_label.cpp)

	list(APPEND FORMS_LIB
		include/vclib/ext/qt/gui/drawable_object_frame.ui
		include/vclib/ext/qt/gui/drawable_object_vector_frame.ui
		include/vclib/ext/qt/gui/mesh_render_settings_frame.ui)

	# headers and sources to include only if using QGLViewer
	if (TARGET vclib-external-qglviewer)
		# header only sources
		list(APPEND HEADERS
			include/vclib/ext/qglviewer/gl_area.h)

		list(APPEND SOURCES
			include/vclib/ext/qglviewer/gl_area.cpp)

		# sources that must be built
		list(APPEND HEADERS_LIB
			include/vclib/ext/qglviewer/viewer_main_window.h)

		list(APPEND SOURCES_LIB
			include/vclib/ext/qglviewer/viewer_main_window.cpp)

		list(APPEND FORMS_LIB
			include/vclib/ext/qglviewer/viewer_main_window.ui)
	endif()
endif()

add_library(vclib INTERFACE)

target_include_directories(vclib INTERFACE ${VCLIB_INCLUDE_DIR})
target_link_libraries(vclib INTERFACE ${VCLIB_EXTERNAL_LIBRARIES})

if (WIN32 OR TARGET vclib-external-tbb)
	message(STATUS "VCLib: using parallel algorithms")
else()
	message(STATUS "VCLib: not using parallel algorithms")
endif()

if (TARGET vclib-external-qt)
	add_library(vclib-qt SHARED ${HEADERS_LIB} ${SOURCES_LIB} ${FORMS_LIB})
	target_include_directories(vclib-qt PRIVATE ${VCLIB_INCLUDE_DIR})
	target_link_libraries(vclib-qt PRIVATE ${VCLIB_EXTERNAL_LIBRARIES})

	target_link_libraries(vclib INTERFACE vclib-qt)
endif()

if (VCLIB_IDE)
	add_custom_target(vclib_ide SOURCES
		${HEADERS} ${HEADERS_LIB} ${SOURCES} ${SOURCES_LIB} ${FORMS_LIB})
endif()

if (VCLIB_BUILD_TESTS)
	add_subdirectory(test)
endif()

if (VCLIB_BUILD_DOCS)
	add_subdirectory(docs)
endif()


