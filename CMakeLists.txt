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
	include/vclib/algorithm.h
	include/vclib/concept.h
	include/vclib/exception.h
	include/vclib/image.h
	include/vclib/load_save.h
	include/vclib/math.h
	include/vclib/mesh.h
	include/vclib/miscellaneous.h
	include/vclib/space.h
	include/vclib/types.h
	include/vclib/algorithm/bounding_box.h
	include/vclib/algorithm/clean.h
	include/vclib/algorithm/create.h
	include/vclib/algorithm/distance.h
	include/vclib/algorithm/export_to_matrix.h
	include/vclib/algorithm/fitting.h
	include/vclib/algorithm/generate.h
	include/vclib/algorithm/intersection.h
	include/vclib/algorithm/point_sampling.h
	include/vclib/algorithm/polygon.h
	include/vclib/algorithm/shuffle.h
	include/vclib/algorithm/smooth.h
	include/vclib/algorithm/sort.h
	include/vclib/algorithm/stat.h
	include/vclib/algorithm/update.h
	include/vclib/algorithm/create/dodecahedron.h
	include/vclib/algorithm/create/hexahedron.h
	include/vclib/algorithm/create/icosahedron.h
	include/vclib/algorithm/create/sphere.h
	include/vclib/algorithm/create/tetrahedron.h
	include/vclib/algorithm/distance/functions.h
	include/vclib/algorithm/distance/element.h
	include/vclib/algorithm/distance/mesh.h
	include/vclib/algorithm/distance/misc.h
	include/vclib/algorithm/intersection/element.h
	include/vclib/algorithm/intersection/functions.h
	include/vclib/algorithm/intersection/mesh.h
	include/vclib/algorithm/intersection/misc.h
	include/vclib/algorithm/polygon/geometry.h
	include/vclib/algorithm/polygon/topology.h
	include/vclib/algorithm/stat/scalar.h
	include/vclib/algorithm/stat/selection.h
	include/vclib/algorithm/update/bounding_box.h
	include/vclib/algorithm/update/color.h
	include/vclib/algorithm/update/curvature.h
	include/vclib/algorithm/update/flag.h
	include/vclib/algorithm/update/normal.h
	include/vclib/algorithm/update/scalar.h
	include/vclib/algorithm/update/selection.h
	include/vclib/algorithm/update/topology.h
	include/vclib/algorithm/update/transform.h
	include/vclib/concept/logger.h
	include/vclib/concept/mesh.h
	include/vclib/concept/space.h
	include/vclib/concept/mesh/component.h
	include/vclib/concept/mesh/container.h
	include/vclib/concept/mesh/per_face.h
	include/vclib/concept/mesh/per_half_edge.h
	include/vclib/concept/mesh/per_vertex.h
	include/vclib/concept/mesh/component/adjacent_edges.h
	include/vclib/concept/mesh/component/adjacent_faces.h
	include/vclib/concept/mesh/component/adjacent_vertices.h
	include/vclib/concept/mesh/component/bit_flags.h
	include/vclib/concept/mesh/component/bounding_box.h
	include/vclib/concept/mesh/component/color.h
	include/vclib/concept/mesh/component/component.h
	include/vclib/concept/mesh/component/coordinate.h
	include/vclib/concept/mesh/component/custom_components.h
	include/vclib/concept/mesh/component/face_half_edge_reference.h
	include/vclib/concept/mesh/component/half_edge_references.h
	include/vclib/concept/mesh/component/mark.h
	include/vclib/concept/mesh/component/name.h
	include/vclib/concept/mesh/component/normal.h
	include/vclib/concept/mesh/component/principal_curvature.h
	include/vclib/concept/mesh/component/scalar.h
	include/vclib/concept/mesh/component/tex_coord.h
	include/vclib/concept/mesh/component/texture_paths.h
	include/vclib/concept/mesh/component/transform_matrix.h
	include/vclib/concept/mesh/component/vertex_half_edge_reference.h
	include/vclib/concept/mesh/component/vertex_references.h
	include/vclib/concept/mesh/component/wedge_colors.h
	include/vclib/concept/mesh/component/wedge_tex_coords.h
	include/vclib/concept/mesh/element/edge.h
	include/vclib/concept/mesh/element/element.h
	include/vclib/concept/mesh/element/face.h
	include/vclib/concept/mesh/element/half_edge.h
	include/vclib/concept/mesh/element/vertex.h
	include/vclib/concept/space/box.h
	include/vclib/concept/space/matrix.h
	include/vclib/concept/space/plane.h
	include/vclib/concept/space/point.h
	include/vclib/concept/space/polygon.h
	include/vclib/concept/space/sampler.h
	include/vclib/concept/space/segment.h
	include/vclib/concept/space/sphere.h
	include/vclib/concept/space/triangle.h
	include/vclib/exception/io_exception.h
	include/vclib/exception/mesh_exception.h
	include/vclib/exception/misc_exception.h
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
	include/vclib/iterators/pointer_iterator.h
	include/vclib/iterators/view.h
	include/vclib/math/base.h
	include/vclib/math/distribution.h
	include/vclib/math/fibonacci.h
	include/vclib/math/histogram.h
	include/vclib/math/min_max.h
	include/vclib/math/perlin_noise.h
	include/vclib/math/random.h
	include/vclib/math/transform.h
	include/vclib/mesh/dcel_mesh.h
	include/vclib/mesh/edge_mesh.h
	include/vclib/mesh/iterator.h
	include/vclib/mesh/point_cloud.h
	include/vclib/mesh/poly_mesh.h
	include/vclib/mesh/requirements.h
	include/vclib/mesh/tri_mesh.h
	include/vclib/mesh/components/adjacent_edges.h
	include/vclib/mesh/components/adjacent_faces.h
	include/vclib/mesh/components/adjacent_vertices.h
	include/vclib/mesh/components/bit_flags.h
	include/vclib/mesh/components/bounding_box.h
	include/vclib/mesh/components/color.h
	include/vclib/mesh/components/coordinate.h
	include/vclib/mesh/components/custom_components.h
	include/vclib/mesh/components/face_half_edge_reference.h
	include/vclib/mesh/components/half_edge_references.h
	include/vclib/mesh/components/mark.h
	include/vclib/mesh/components/name.h
	include/vclib/mesh/components/normal.h
	include/vclib/mesh/components/parent_mesh_pointer.h
	include/vclib/mesh/components/polygon_bit_flags.h
	include/vclib/mesh/components/principal_curvature.h
	include/vclib/mesh/components/scalar.h
	include/vclib/mesh/components/vertex_half_edge_reference.h
	include/vclib/mesh/components/vertex_references.h
	include/vclib/mesh/components/tex_coord.h
	include/vclib/mesh/components/texture_paths.h
	include/vclib/mesh/components/transform_matrix.h
	include/vclib/mesh/components/triangle_bit_flags.h
	include/vclib/mesh/components/wedge_colors.h
	include/vclib/mesh/components/wedge_tex_coords.h
	include/vclib/mesh/components/internal/component_data.h
	include/vclib/mesh/components/internal/custom_components_data.h
	include/vclib/mesh/components/internal/element_references.h
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
	include/vclib/mesh/iterators/component.h
	include/vclib/mesh/iterators/face.h
	include/vclib/mesh/iterators/element_container_iterator.h
	include/vclib/mesh/iterators/component/component_view.h
	include/vclib/mesh/iterators/component/coord_iterator.h
	include/vclib/mesh/iterators/component/scalar_iterator.h
	include/vclib/mesh/iterators/component/selection_iterator.h
	include/vclib/mesh/iterators/face/edge_adj_face_iterator.h
	include/vclib/mesh/iterators/half_edge/base_iterator.h
	include/vclib/mesh/iterators/half_edge/face_adj_face_iterator.h
	include/vclib/mesh/iterators/half_edge/face_base_iterator.h
	include/vclib/mesh/iterators/half_edge/face_half_edge_iterator.h
	include/vclib/mesh/iterators/half_edge/face_vertex_iterator.h
	include/vclib/mesh/iterators/half_edge/face_wedge_color_iterator.h
	include/vclib/mesh/iterators/half_edge/face_wedge_tex_coord_iterator.h
	include/vclib/mesh/iterators/half_edge/vertex_adj_face_iterator.h
	include/vclib/mesh/iterators/half_edge/vertex_adj_vertex_iterator.h
	include/vclib/mesh/iterators/half_edge/vertex_base_iterator.h
	include/vclib/mesh/mesh/mesh.h
	include/vclib/mesh/mesh/mesh_components.h
	include/vclib/mesh/requirements/face_requirements.h
	include/vclib/mesh/requirements/half_edge_requirements.h
	include/vclib/mesh/requirements/mesh_requirements.h
	include/vclib/mesh/requirements/vertex_requirements.h
	include/vclib/mesh/utils/mesh_edge_util.h
	include/vclib/mesh/utils/mesh_inertia.h
	include/vclib/mesh/utils/mesh_pos.h
	include/vclib/mesh/utils/tri_poly_index_bimap.h
	include/vclib/misc/bit_proxy.h
	include/vclib/misc/compactness.h
	include/vclib/misc/comparators.h
	include/vclib/misc/file_info.h
	include/vclib/misc/hash.h
	include/vclib/misc/logger.h
	include/vclib/misc/mark.h
	include/vclib/misc/nested_initializer_lists.h
	include/vclib/misc/pair.h
	include/vclib/misc/parallel.h
	include/vclib/misc/random_access_container.h
	include/vclib/misc/string.h
	include/vclib/misc/timer.h
	include/vclib/misc/tokenizer.h
	include/vclib/misc/tuple.h
	include/vclib/misc/internal/tmp_meshes.h
	include/vclib/misc/logger/console_logger.h
	include/vclib/misc/logger/logger.h
	include/vclib/misc/mark/markable.h
	include/vclib/misc/mark/markable_vector.h
	include/vclib/render/drawable_object.h
	include/vclib/render/drawable_object_vector.h
	include/vclib/render/generic_drawable_mesh.h
	include/vclib/render/mesh_render_buffers.h
	include/vclib/render/mesh_render_settings.h
	include/vclib/space/array.h
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
	include/vclib/space/array/array_t.h
	include/vclib/space/array/array2.h
	include/vclib/space/array/array3.h
	include/vclib/space/array/array4.h
	include/vclib/space/array/array_bool.h
	include/vclib/space/graph/undirected_node.h
	include/vclib/space/graph/bipartite/bipartite_graph.h
	include/vclib/space/graph/bipartite/iterator/adjacent_left_node_iterator.h
	include/vclib/space/graph/bipartite/iterator/adjacent_node_iterator.h
	include/vclib/space/graph/bipartite/iterator/adjacent_right_node_iterator.h
	include/vclib/space/graph/bipartite/iterator/node_iterator.h
	include/vclib/space/grid/abstract_ds_grid.h
	include/vclib/space/grid/hash_table_grid.h
	include/vclib/space/grid/regular_grid_t.h
	include/vclib/space/grid/regular_grid2.h
	include/vclib/space/grid/regular_grid3.h
	include/vclib/space/grid/static_grid.h
	include/vclib/space/grid/iterators/cell_iterator.h
	include/vclib/space/grid/iterators/static_grid_iterator.h
	include/vclib/space/point/point_t.h
	include/vclib/space/point/point2.h
	include/vclib/space/point/point3.h
	include/vclib/space/point/point4.h
	include/vclib/space/sampler/mesh_sampler.h
	include/vclib/space/sampler/point_sampler.h
	include/vclib/space/sampler/vertex_sampler.h
	include/vclib/types/base.h
	include/vclib/types/const_correctness.h
	include/vclib/types/inheritance.h
	include/vclib/types/iterators.h
	include/vclib/types/pointers.h
	include/vclib/types/variadic_templates.h
)

set(SOURCES
	include/vclib/algorithm/bounding_box.cpp
	include/vclib/algorithm/clean.cpp
	include/vclib/algorithm/export_to_matrix.cpp
	include/vclib/algorithm/fitting.cpp
	include/vclib/algorithm/generate.cpp
	include/vclib/algorithm/point_sampling.cpp
	include/vclib/algorithm/shuffle.cpp
	include/vclib/algorithm/smooth.cpp
	include/vclib/algorithm/sort.cpp
	include/vclib/algorithm/stat.cpp
	include/vclib/algorithm/create/dodecahedron.cpp
	include/vclib/algorithm/create/hexahedron.cpp
	include/vclib/algorithm/create/icosahedron.cpp
	include/vclib/algorithm/create/sphere.cpp
	include/vclib/algorithm/create/tetrahedron.cpp
	include/vclib/algorithm/distance/element.cpp
	include/vclib/algorithm/distance/mesh.cpp
	include/vclib/algorithm/distance/misc.cpp
	include/vclib/algorithm/intersection/element.cpp
	include/vclib/algorithm/intersection/mesh.cpp
	include/vclib/algorithm/intersection/misc.cpp
	include/vclib/algorithm/polygon/geometry.cpp
	include/vclib/algorithm/polygon/topology.cpp
	include/vclib/algorithm/stat/scalar.cpp
	include/vclib/algorithm/stat/selection.cpp
	include/vclib/algorithm/update/bounding_box.cpp
	include/vclib/algorithm/update/color.cpp
	include/vclib/algorithm/update/curvature.cpp
	include/vclib/algorithm/update/flag.cpp
	include/vclib/algorithm/update/normal.cpp
	include/vclib/algorithm/update/scalar.cpp
	include/vclib/algorithm/update/selection.cpp
	include/vclib/algorithm/update/topology.cpp
	include/vclib/algorithm/update/transform.cpp
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
	include/vclib/math/distribution.cpp
	include/vclib/math/fibonacci.cpp
	include/vclib/math/histogram.cpp
	include/vclib/math/min_max.cpp
	include/vclib/math/perlin_noise.cpp
	include/vclib/math/base.cpp
	include/vclib/math/random.cpp
	include/vclib/math/transform.cpp
	include/vclib/mesh/components/adjacent_edges.cpp
	include/vclib/mesh/components/adjacent_faces.cpp
	include/vclib/mesh/components/adjacent_vertices.cpp
	include/vclib/mesh/components/bit_flags.cpp
	include/vclib/mesh/components/bounding_box.cpp
	include/vclib/mesh/components/color.cpp
	include/vclib/mesh/components/coordinate.cpp
	include/vclib/mesh/components/custom_components.cpp
	include/vclib/mesh/components/face_half_edge_reference.cpp
	include/vclib/mesh/components/half_edge_references.cpp
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
	include/vclib/mesh/components/vertex_half_edge_reference.cpp
	include/vclib/mesh/components/vertex_references.cpp
	include/vclib/mesh/components/wedge_colors.cpp
	include/vclib/mesh/components/wedge_tex_coords.cpp
	include/vclib/mesh/components/internal/element_references.cpp
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
	include/vclib/mesh/iterators/element_container_iterator.cpp
	include/vclib/mesh/iterators/face/edge_adj_face_iterator.cpp
	include/vclib/mesh/iterators/half_edge/face_base_iterator.cpp
	include/vclib/mesh/iterators/half_edge/vertex_base_iterator.cpp
	include/vclib/mesh/mesh/mesh.cpp
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
	include/vclib/misc/random_access_container.cpp
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
	include/vclib/space/array/array_t.cpp
	include/vclib/space/array/array2.cpp
	include/vclib/space/array/array3.cpp
	include/vclib/space/array/array4.cpp
	include/vclib/space/graph/undirected_node.cpp
	include/vclib/space/graph/bipartite/bipartite_graph.cpp
	include/vclib/space/graph/bipartite/iterator/adjacent_node_iterator.cpp
	include/vclib/space/grid/abstract_ds_grid.cpp
	include/vclib/space/grid/hash_table_grid.cpp
	include/vclib/space/grid/regular_grid_t.cpp
	include/vclib/space/grid/static_grid.cpp
	include/vclib/space/grid/iterators/cell_iterator.cpp
	include/vclib/space/grid/iterators/static_grid_iterator.cpp
	include/vclib/space/point/point_t.cpp
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


