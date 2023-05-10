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

cmake_minimum_required(VERSION 3.13)
project(vclib)

### Build settings
set(CMAKE_CXX_STANDARD 20)

### Options
option(VCLIB_BUILD_TESTS "Build VCLib tests" ON)
option(VCLIB_IDE "Create an utility target to show the library tree on IDEs" ON)

# In case of building shared libraries, on windows we need to export all symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Add the cmake folder for find_package scripts
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# external libraries
message(STATUS "VCLib: external libraries")
add_subdirectory(external)

if (WIN32)
	add_compile_definitions(NOMINMAX)
	add_compile_definitions(_USE_MATH_DEFINES)
	if (MSVC)
		# remove possible loss of data warnings in msvc, leave the others
		add_compile_options(/w44244 /w44267 /w44305)
	endif()
endif()

set(VCLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(VCLIB_SRC_DIR ${VCLIB_INCLUDE_DIR}/vclib)

set(HEADERS
	include/vclib/algorithms.h
	include/vclib/concepts.h
	include/vclib/exceptions.h
	include/vclib/image.h
	include/vclib/load_save.h
	include/vclib/math.h
	include/vclib/meshes.h
	include/vclib/miscellaneous.h
	include/vclib/space.h
	include/vclib/types.h
	include/vclib/views.h
	include/vclib/algorithms/bounding_box.h
	include/vclib/algorithms/clean.h
	include/vclib/algorithms/create.h
	include/vclib/algorithms/distance.h
	include/vclib/algorithms/export_to_matrix.h
	include/vclib/algorithms/fitting.h
	include/vclib/algorithms/generate.h
	include/vclib/algorithms/intersection.h
	include/vclib/algorithms/point_sampling.h
	include/vclib/algorithms/polygon.h
	include/vclib/algorithms/shuffle.h
	include/vclib/algorithms/smooth.h
	include/vclib/algorithms/sort.h
	include/vclib/algorithms/stat.h
	include/vclib/algorithms/update.h
	include/vclib/algorithms/create/dodecahedron.h
	include/vclib/algorithms/create/hexahedron.h
	include/vclib/algorithms/create/icosahedron.h
	include/vclib/algorithms/create/sphere.h
	include/vclib/algorithms/create/tetrahedron.h
	include/vclib/algorithms/distance/functions.h
	include/vclib/algorithms/distance/element.h
	include/vclib/algorithms/distance/mesh.h
	include/vclib/algorithms/distance/misc.h
	include/vclib/algorithms/intersection/element.h
	include/vclib/algorithms/intersection/functions.h
	include/vclib/algorithms/intersection/mesh.h
	include/vclib/algorithms/intersection/misc.h
	include/vclib/algorithms/polygon/geometry.h
	include/vclib/algorithms/polygon/topology.h
	include/vclib/algorithms/stat/scalar.h
	include/vclib/algorithms/stat/selection.h
	include/vclib/algorithms/update/bounding_box.h
	include/vclib/algorithms/update/color.h
	include/vclib/algorithms/update/curvature.h
	include/vclib/algorithms/update/flag.h
	include/vclib/algorithms/update/normal.h
	include/vclib/algorithms/update/scalar.h
	include/vclib/algorithms/update/selection.h
	include/vclib/algorithms/update/topology.h
	include/vclib/algorithms/update/transform.h
	include/vclib/concepts/const_correctness.h
	include/vclib/concepts/iterators.h
	include/vclib/concepts/logger.h
	include/vclib/concepts/mesh.h
	include/vclib/concepts/pointers.h
	include/vclib/concepts/range.h
	include/vclib/concepts/space.h
	include/vclib/concepts/mesh/components.h
	include/vclib/concepts/mesh/containers.h
	include/vclib/concepts/mesh/per_face.h
	include/vclib/concepts/mesh/per_half_edge.h
	include/vclib/concepts/mesh/per_vertex.h
	include/vclib/concepts/mesh/components/adjacent_edges.h
	include/vclib/concepts/mesh/components/adjacent_faces.h
	include/vclib/concepts/mesh/components/adjacent_vertices.h
	include/vclib/concepts/mesh/components/bit_flags.h
	include/vclib/concepts/mesh/components/bounding_box.h
	include/vclib/concepts/mesh/components/color.h
	include/vclib/concepts/mesh/components/component.h
	include/vclib/concepts/mesh/components/coordinate.h
	include/vclib/concepts/mesh/components/custom_components.h
	include/vclib/concepts/mesh/components/face_half_edge_pointers.h
	include/vclib/concepts/mesh/components/half_edge_pointers.h
	include/vclib/concepts/mesh/components/mark.h
	include/vclib/concepts/mesh/components/name.h
	include/vclib/concepts/mesh/components/normal.h
	include/vclib/concepts/mesh/components/principal_curvature.h
	include/vclib/concepts/mesh/components/scalar.h
	include/vclib/concepts/mesh/components/tex_coord.h
	include/vclib/concepts/mesh/components/texture_paths.h
	include/vclib/concepts/mesh/components/transform_matrix.h
	include/vclib/concepts/mesh/components/vertex_half_edge_pointer.h
	include/vclib/concepts/mesh/components/vertex_pointers.h
	include/vclib/concepts/mesh/components/wedge_colors.h
	include/vclib/concepts/mesh/components/wedge_tex_coords.h
	include/vclib/concepts/mesh/elements/edge.h
	include/vclib/concepts/mesh/elements/element.h
	include/vclib/concepts/mesh/elements/face.h
	include/vclib/concepts/mesh/elements/half_edge.h
	include/vclib/concepts/mesh/elements/vertex.h
	include/vclib/concepts/ranges/mesh.h
	include/vclib/concepts/ranges/range.h
	include/vclib/concepts/ranges/mesh/vertex_range.h
	include/vclib/concepts/space/box.h
	include/vclib/concepts/space/matrix.h
	include/vclib/concepts/space/plane.h
	include/vclib/concepts/space/point.h
	include/vclib/concepts/space/polygon.h
	include/vclib/concepts/space/sampler.h
	include/vclib/concepts/space/segment.h
	include/vclib/concepts/space/sphere.h
	include/vclib/concepts/space/triangle.h
	include/vclib/exceptions/io_exceptions.h
	include/vclib/exceptions/mesh_exceptions.h
	include/vclib/exceptions/misc_exceptions.h
	include/vclib/image/image.h
	include/vclib/io/file_mesh_info.h
	include/vclib/io/load.h
	include/vclib/io/save.h
	include/vclib/io/internal/io_read.h
	include/vclib/io/internal/io_utils.h
	include/vclib/io/internal/io_write.h
	include/vclib/io/obj/load.h
	include/vclib/io/obj/material.h
	include/vclib/io/obj/save.h
	include/vclib/io/off/load.h
	include/vclib/io/off/save.h
	include/vclib/io/off/utils.h
	include/vclib/io/ply/load.h
	include/vclib/io/ply/ply.h
	include/vclib/io/ply/ply_edge.h
	include/vclib/io/ply/ply_extra.h
	include/vclib/io/ply/ply_face.h
	include/vclib/io/ply/ply_header.h
	include/vclib/io/ply/ply_tristrip.h
	include/vclib/io/ply/ply_vertex.h
	include/vclib/io/ply/save.h
	include/vclib/io/stl/load.h
	include/vclib/io/stl/save.h
	include/vclib/iterators/iterator_wrapper.h
	include/vclib/iterators/mesh.h
	include/vclib/iterators/pointer_iterator.h
	include/vclib/iterators/mesh/face.h
	include/vclib/iterators/mesh/element_container_iterator.h
	include/vclib/iterators/mesh/face/edge_adj_face_iterator.h
	include/vclib/iterators/mesh/half_edge/face_adj_face_iterator.h
	include/vclib/iterators/mesh/half_edge/face_base_iterator.h
	include/vclib/iterators/mesh/half_edge/face_half_edge_iterator.h
	include/vclib/iterators/mesh/half_edge/face_vertex_iterator.h
	include/vclib/iterators/mesh/half_edge/face_wedge_color_iterator.h
	include/vclib/iterators/mesh/half_edge/face_wedge_tex_coord_iterator.h
	include/vclib/iterators/mesh/half_edge/vertex_adj_face_iterator.h
	include/vclib/iterators/mesh/half_edge/vertex_adj_vertex_iterator.h
	include/vclib/iterators/mesh/half_edge/vertex_base_iterator.h
	include/vclib/iterators/space/grid/cell_iterator.h
	include/vclib/iterators/space/grid/static_grid_iterator.h
	include/vclib/math/base.h
	include/vclib/math/distribution.h
	include/vclib/math/fibonacci.h
	include/vclib/math/histogram.h
	include/vclib/math/min_max.h
	include/vclib/math/perlin_noise.h
	include/vclib/math/random.h
	include/vclib/math/transform.h
	include/vclib/mesh/mesh.h
	include/vclib/mesh/mesh_components.h
	include/vclib/mesh/requirements.h
	include/vclib/mesh/components/adjacent_edges.h
	include/vclib/mesh/components/adjacent_faces.h
	include/vclib/mesh/components/adjacent_vertices.h
	include/vclib/mesh/components/bit_flags.h
	include/vclib/mesh/components/bounding_box.h
	include/vclib/mesh/components/color.h
	include/vclib/mesh/components/coordinate.h
	include/vclib/mesh/components/custom_components.h
	include/vclib/mesh/components/face_half_edge_pointers.h
	include/vclib/mesh/components/half_edge_pointers.h
	include/vclib/mesh/components/mark.h
	include/vclib/mesh/components/name.h
	include/vclib/mesh/components/normal.h
	include/vclib/mesh/components/parent_mesh_pointer.h
	include/vclib/mesh/components/polygon_bit_flags.h
	include/vclib/mesh/components/principal_curvature.h
	include/vclib/mesh/components/scalar.h
	include/vclib/mesh/components/vertex_half_edge_pointer.h
	include/vclib/mesh/components/vertex_pointers.h
	include/vclib/mesh/components/tex_coord.h
	include/vclib/mesh/components/texture_paths.h
	include/vclib/mesh/components/transform_matrix.h
	include/vclib/mesh/components/triangle_bit_flags.h
	include/vclib/mesh/components/wedge_colors.h
	include/vclib/mesh/components/wedge_tex_coords.h
	include/vclib/mesh/components/bases/component.h
	include/vclib/mesh/components/bases/container_component.h
	include/vclib/mesh/components/bases/pointers_container_component.h
	include/vclib/mesh/components/internal/component_data.h
	include/vclib/mesh/components/internal/custom_components_data.h
	include/vclib/mesh/containers/containers.h
	include/vclib/mesh/containers/custom_component_vector_handle.h
	include/vclib/mesh/containers/custom_components_vector_map.h
	include/vclib/mesh/containers/edge_container.h
	include/vclib/mesh/containers/element_container.h
	include/vclib/mesh/containers/face_container.h
	include/vclib/mesh/containers/half_edge_container.h
	include/vclib/mesh/containers/vertex_container.h
	include/vclib/mesh/containers/vertical_components_vector_tuple.h
	include/vclib/mesh/elements/edge.h
	include/vclib/mesh/elements/edge_components.h
	include/vclib/mesh/elements/element.h
	include/vclib/mesh/elements/face.h
	include/vclib/mesh/elements/face_components.h
	include/vclib/mesh/elements/half_edge.h
	include/vclib/mesh/elements/half_edge_components.h
	include/vclib/mesh/elements/vertex.h
	include/vclib/mesh/elements/vertex_components.h
	include/vclib/mesh/requirements/face_requirements.h
	include/vclib/mesh/requirements/half_edge_requirements.h
	include/vclib/mesh/requirements/mesh_requirements.h
	include/vclib/mesh/requirements/vertex_requirements.h
	include/vclib/mesh/utils/mesh_edge_util.h
	include/vclib/mesh/utils/mesh_inertia.h
	include/vclib/mesh/utils/mesh_pos.h
	include/vclib/mesh/utils/tri_poly_index_bimap.h
	include/vclib/meshes/dcel_mesh.h
	include/vclib/meshes/edge_mesh.h
	include/vclib/meshes/point_cloud.h
	include/vclib/meshes/poly_mesh.h
	include/vclib/meshes/tri_mesh.h
	include/vclib/misc/compactness.h
	include/vclib/misc/comparators.h
	include/vclib/misc/file_info.h
	include/vclib/misc/hash.h
	include/vclib/misc/logger.h
	include/vclib/misc/mark.h
	include/vclib/misc/nested_initializer_lists.h
	include/vclib/misc/pair.h
	include/vclib/misc/parallel.h
	include/vclib/misc/string.h
	include/vclib/misc/timer.h
	include/vclib/misc/tokenizer.h
	include/vclib/misc/tuple.h
	include/vclib/misc/internal/tmp_meshes.h
	include/vclib/misc/logger/console_logger.h
	include/vclib/misc/logger/logger.h
	include/vclib/misc/logger/null_logger.h
	include/vclib/misc/mark/markable.h
	include/vclib/misc/mark/markable_vector.h
	include/vclib/render/drawable_object.h
	include/vclib/render/drawable_object_vector.h
	include/vclib/render/generic_drawable_mesh.h
	include/vclib/render/mesh_render_buffers.h
	include/vclib/render/mesh_render_settings.h
	include/vclib/space/array.h
	include/vclib/space/bit_set.h
	include/vclib/space/box.h
	include/vclib/space/color.h
	include/vclib/space/graph.h
	include/vclib/space/grid.h
	include/vclib/space/kd_tree.h
	include/vclib/space/matrix.h
	include/vclib/space/plane.h
	include/vclib/space/point.h
	include/vclib/space/polygon.h
	include/vclib/space/principal_curvature.h
	include/vclib/space/sampler.h
	include/vclib/space/segment.h
	include/vclib/space/sphere.h
	include/vclib/space/tex_coord.h
	include/vclib/space/triangle.h
	include/vclib/space/triangle_wrapper.h
	include/vclib/space/vector.h
	include/vclib/space/bit_set/bit_proxy.h
	include/vclib/space/graph/undirected_node.h
	include/vclib/space/graph/bipartite/bipartite_graph.h
	include/vclib/space/graph/bipartite/iterator/adjacent_left_node_iterator.h
	include/vclib/space/graph/bipartite/iterator/adjacent_node_iterator.h
	include/vclib/space/graph/bipartite/iterator/adjacent_right_node_iterator.h
	include/vclib/space/graph/bipartite/iterator/node_iterator.h
	include/vclib/space/grid/abstract_grid.h
	include/vclib/space/grid/hash_table_grid.h
	include/vclib/space/grid/regular_grid.h
	include/vclib/space/grid/static_grid.h
	include/vclib/space/sampler/mesh_sampler.h
	include/vclib/space/sampler/point_sampler.h
	include/vclib/space/sampler/vertex_sampler.h
	include/vclib/types/base.h
	include/vclib/types/const_correctness.h
	include/vclib/types/inheritance.h
	include/vclib/types/pointers.h
	include/vclib/types/variadic_templates.h
	include/vclib/views/mesh.h
	include/vclib/views/view.h
	include/vclib/views/views.h
	include/vclib/views/mesh/components.h
	include/vclib/views/mesh/elements.h
	include/vclib/views/mesh/components/adj_edges.h
	include/vclib/views/mesh/components/adj_faces.h
	include/vclib/views/mesh/components/adj_vertices.h
	include/vclib/views/mesh/components/colors.h
	include/vclib/views/mesh/components/coords.h
	include/vclib/views/mesh/components/normals.h
	include/vclib/views/mesh/components/principal_curvatures.h
	include/vclib/views/mesh/components/scalars.h
	include/vclib/views/mesh/components/selection.h
	include/vclib/views/mesh/components/tex_coords.h
)

set(SOURCES
	include/vclib/algorithms/bounding_box.cpp
	include/vclib/algorithms/clean.cpp
	include/vclib/algorithms/export_to_matrix.cpp
	include/vclib/algorithms/fitting.cpp
	include/vclib/algorithms/generate.cpp
	include/vclib/algorithms/point_sampling.cpp
	include/vclib/algorithms/shuffle.cpp
	include/vclib/algorithms/smooth.cpp
	include/vclib/algorithms/sort.cpp
	include/vclib/algorithms/stat.cpp
	include/vclib/algorithms/create/dodecahedron.cpp
	include/vclib/algorithms/create/hexahedron.cpp
	include/vclib/algorithms/create/icosahedron.cpp
	include/vclib/algorithms/create/sphere.cpp
	include/vclib/algorithms/create/tetrahedron.cpp
	include/vclib/algorithms/distance/element.cpp
	include/vclib/algorithms/distance/mesh.cpp
	include/vclib/algorithms/distance/misc.cpp
	include/vclib/algorithms/intersection/element.cpp
	include/vclib/algorithms/intersection/mesh.cpp
	include/vclib/algorithms/intersection/misc.cpp
	include/vclib/algorithms/polygon/geometry.cpp
	include/vclib/algorithms/polygon/topology.cpp
	include/vclib/algorithms/stat/scalar.cpp
	include/vclib/algorithms/stat/selection.cpp
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
	include/vclib/io/save.cpp
	include/vclib/io/internal/io_read.cpp
	include/vclib/io/internal/io_utils.cpp
	include/vclib/io/internal/io_write.cpp
	include/vclib/io/obj/load.cpp
	include/vclib/io/obj/material.cpp
	include/vclib/io/obj/save.cpp
	include/vclib/io/off/load.cpp
	include/vclib/io/off/save.cpp
	include/vclib/io/off/utils.cpp
	include/vclib/io/ply/load.cpp
	include/vclib/io/ply/ply_edge.cpp
	include/vclib/io/ply/ply_extra.cpp
	include/vclib/io/ply/ply_face.cpp
	include/vclib/io/ply/ply_header.cpp
	include/vclib/io/ply/ply_tristrip.cpp
	include/vclib/io/ply/ply_vertex.cpp
	include/vclib/io/ply/save.cpp
	include/vclib/io/stl/load.cpp
	include/vclib/io/stl/save.cpp
	include/vclib/iterators/iterator_wrapper.cpp
	include/vclib/iterators/mesh/element_container_iterator.cpp
	include/vclib/iterators/mesh/face/edge_adj_face_iterator.cpp
	include/vclib/iterators/space/grid/cell_iterator.cpp
	include/vclib/iterators/space/grid/static_grid_iterator.cpp
	include/vclib/math/distribution.cpp
	include/vclib/math/fibonacci.cpp
	include/vclib/math/histogram.cpp
	include/vclib/math/min_max.cpp
	include/vclib/math/perlin_noise.cpp
	include/vclib/math/base.cpp
	include/vclib/math/random.cpp
	include/vclib/math/transform.cpp
	include/vclib/mesh/mesh.cpp
	include/vclib/mesh/components/adjacent_edges.cpp
	include/vclib/mesh/components/adjacent_faces.cpp
	include/vclib/mesh/components/adjacent_vertices.cpp
	include/vclib/mesh/components/bit_flags.cpp
	include/vclib/mesh/components/bounding_box.cpp
	include/vclib/mesh/components/color.cpp
	include/vclib/mesh/components/coordinate.cpp
	include/vclib/mesh/components/custom_components.cpp
	include/vclib/mesh/components/face_half_edge_pointers.cpp
	include/vclib/mesh/components/half_edge_pointers.cpp
	include/vclib/mesh/components/mark.cpp
	include/vclib/mesh/components/name.cpp
	include/vclib/mesh/components/normal.cpp
	include/vclib/mesh/components/parent_mesh_pointer.cpp
	include/vclib/mesh/components/polygon_bit_flags.cpp
	include/vclib/mesh/components/principal_curvature.cpp
	include/vclib/mesh/components/scalar.cpp
	include/vclib/mesh/components/tex_coord.cpp
	include/vclib/mesh/components/texture_paths.cpp
	include/vclib/mesh/components/transform_matrix.cpp
	include/vclib/mesh/components/triangle_bit_flags.cpp
	include/vclib/mesh/components/vertex_half_edge_pointer.cpp
	include/vclib/mesh/components/vertex_pointers.cpp
	include/vclib/mesh/components/wedge_colors.cpp
	include/vclib/mesh/components/wedge_tex_coords.cpp
	include/vclib/mesh/components/bases/container_component.cpp
	include/vclib/mesh/components/bases/pointers_container_component.cpp
	include/vclib/mesh/containers/custom_component_vector_handle.cpp
	include/vclib/mesh/containers/custom_components_vector_map.cpp
	include/vclib/mesh/containers/edge_container.cpp
	include/vclib/mesh/containers/element_container.cpp
	include/vclib/mesh/containers/half_edge_container.cpp
	include/vclib/mesh/containers/face_container.cpp
	include/vclib/mesh/containers/vertex_container.cpp
	include/vclib/mesh/containers/vertical_components_vector_tuple.cpp
	include/vclib/mesh/elements/edge.cpp
	include/vclib/mesh/elements/element.cpp
	include/vclib/mesh/elements/face.cpp
	include/vclib/mesh/elements/half_edge.cpp
	include/vclib/mesh/elements/vertex.cpp
	include/vclib/mesh/requirements/face_requirements.cpp
	include/vclib/mesh/requirements/half_edge_requirements.cpp
	include/vclib/mesh/requirements/mesh_requirements.cpp
	include/vclib/mesh/requirements/vertex_requirements.cpp
	include/vclib/mesh/utils/mesh_inertia.cpp
	include/vclib/mesh/utils/mesh_pos.cpp
	include/vclib/mesh/utils/tri_poly_index_bimap.cpp
	include/vclib/misc/compactness.cpp
	include/vclib/misc/file_info.cpp
	include/vclib/misc/nested_initializer_lists.cpp
	include/vclib/misc/parallel.cpp
	include/vclib/misc/string.cpp
	include/vclib/misc/timer.cpp
	include/vclib/misc/tokenizer.cpp
	include/vclib/misc/logger/console_logger.cpp
	include/vclib/misc/logger/logger.cpp
	include/vclib/misc/mark/markable_vector.cpp
	include/vclib/render/drawable_object_vector.cpp
	include/vclib/render/generic_drawable_mesh.cpp
	include/vclib/render/mesh_render_buffers.cpp
	include/vclib/render/mesh_render_settings.cpp
	include/vclib/space/array.cpp
	include/vclib/space/bit_set.cpp
	include/vclib/space/box.cpp
	include/vclib/space/color.cpp
	include/vclib/space/matrix.cpp
	include/vclib/space/kd_tree.cpp
	include/vclib/space/plane.cpp
	include/vclib/space/principal_curvature.cpp
	include/vclib/space/point.cpp
	include/vclib/space/polygon.cpp
	include/vclib/space/segment.cpp
	include/vclib/space/sphere.cpp
	include/vclib/space/tex_coord.cpp
	include/vclib/space/triangle.cpp
	include/vclib/space/triangle_wrapper.cpp
	include/vclib/space/vector.cpp
	include/vclib/space/bit_set/bit_proxy.cpp
	include/vclib/space/graph/undirected_node.cpp
	include/vclib/space/graph/bipartite/bipartite_graph.cpp
	include/vclib/space/graph/bipartite/iterator/adjacent_node_iterator.cpp
	include/vclib/space/grid/abstract_grid.cpp
	include/vclib/space/grid/hash_table_grid.cpp
	include/vclib/space/grid/regular_grid.cpp
	include/vclib/space/grid/static_grid.cpp
	include/vclib/space/sampler/mesh_sampler.cpp
	include/vclib/space/sampler/point_sampler.cpp
	include/vclib/space/sampler/vertex_sampler.cpp)

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

# ToDo: remove this and all the code under the not defined VCLIB_USES_RANGES when Apple Clang will 
# be updated by default to Clang 16 and supports the ranges library
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# using Clang
	if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 16.0)
		target_compile_definitions(vclib INTERFACE VCLIB_USES_RANGES)
		message(STATUS "VCLib: using std::ranges library")
	else()
		message(STATUS "VCLib: not using std::ranges library")
	endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# using GCC or Visual Studio C++
	target_compile_definitions(vclib INTERFACE VCLIB_USES_RANGES)
	message(STATUS "VCLib: using std::ranges library")
else()
	message(STATUS "VCLib: not using std::ranges library")
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

