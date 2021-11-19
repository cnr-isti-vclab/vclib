# This file is part of mgplib: https://github.com/alemuntoni/mgplib
# This Source Code Form is subject to the terms of the GNU GPL 3.0

cmake_minimum_required(VERSION 3.13)
project(mgplib)

### Build settings
set(CMAKE_CXX_STANDARD 17)

### Options
option(MGP_BUILD_TESTS "Build MGP tests" ON)
option(MGP_IDE "Create an utility target to show the library tree on IDEs" ON)
option(MGP_BUILD_DOCS
	"Build documentation -
	requires doxygen and sphinx with readthedocs theme and breathe extensions" OFF)

# Add the cmake folder so the FindSphinx module is found
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# external libraries
add_subdirectory(external)

if (WIN32)
	add_compile_definitions(_USE_MATH_DEFINES)
endif()

set(MGP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MGP_SRC_DIR ${MGP_INCLUDE_DIR}/mgp)

set(HEADERS
	include/mgp/polymesh.h
	include/mgp/trimesh.h
	include/mgp/algorithms/clean.h
	include/mgp/algorithms/smooth.h
	include/mgp/algorithms/create/tetrahedron.h
	include/mgp/algorithms/update/bounding_box.h
	include/mgp/algorithms/update/flag.h
	include/mgp/algorithms/update/normal.h
	include/mgp/algorithms/update/topology.h
	include/mgp/exception/io_exception.h
	include/mgp/exception/mesh_exception.h
	include/mgp/math/base.h
	include/mgp/mesh/mesh.h
	include/mgp/mesh/mesh_argument.h
	include/mgp/mesh/mesh_components.h
	include/mgp/mesh/requirements.h
	include/mgp/mesh/container/containers.h
	include/mgp/mesh/container/element_container.h
	include/mgp/mesh/container/face_container.h
	include/mgp/mesh/container/vertex_container.h
	include/mgp/mesh/elements/face.h
	include/mgp/mesh/elements/face_components.h
	include/mgp/mesh/elements/face_components_optional.h
	include/mgp/mesh/elements/vertex.h
	include/mgp/mesh/elements/vertex_components.h
	include/mgp/mesh/elements/vertex_components_optional.h
	include/mgp/mesh/iterators/container_iterator.h
	include/mgp/mesh/iterators/container_range_iterator.h
	include/mgp/mesh/iterators/range_iterator.h
	include/mgp/mesh/components/adjacent_faces.h
	include/mgp/mesh/components/adjacent_vertices.h
	include/mgp/mesh/components/bit_flags.h
	include/mgp/mesh/components/bounding_box.h
	include/mgp/mesh/components/color.h
	include/mgp/mesh/components/coordinate.h
	include/mgp/mesh/components/element_references.h
	include/mgp/mesh/components/generic_container.h
	include/mgp/mesh/components/mark.h
	include/mgp/mesh/components/mutable_bit_flags.h
	include/mgp/mesh/components/normal.h
	include/mgp/mesh/components/polygon_bit_flags.h
	include/mgp/mesh/components/scalar.h
	include/mgp/mesh/components/vertex_references.h
	include/mgp/mesh/components/tex_coord.h
	include/mgp/mesh/components/triangle_bit_flags.h
	include/mgp/mesh/components/wedge_tex_coords.h
	include/mgp/mesh/components_optional/custom_components.h
	include/mgp/mesh/components_optional/optional_adjacent_faces.h
	include/mgp/mesh/components_optional/optional_adjacent_vertices.h
	include/mgp/mesh/components_optional/optional_color.h
	include/mgp/mesh/components_optional/optional_info.h
	include/mgp/mesh/components_optional/optional_mark.h
	include/mgp/mesh/components_optional/optional_mutable_bit_flags.h
	include/mgp/mesh/components_optional/optional_normal.h
	include/mgp/mesh/components_optional/optional_scalar.h
	include/mgp/mesh/components_optional/optional_tex_coord.h
	include/mgp/mesh/components_optional/optional_wedge_tex_coords.h
	include/mgp/mesh/components_vector/optional_color_vector.h
	include/mgp/mesh/components_vector/custom_components_vector.h
	include/mgp/mesh/components_vector/optional_adjacent_faces_vector.h
	include/mgp/mesh/components_vector/optional_adjacent_vertices_vector.h
	include/mgp/mesh/components_vector/optional_generic_vector.h
	include/mgp/mesh/components_vector/optional_mark_vector.h
	include/mgp/mesh/components_vector/optional_mutable_bit_flags_vector.h
	include/mgp/mesh/components_vector/optional_normal_vector.h
	include/mgp/mesh/components_vector/optional_scalar_vector.h
	include/mgp/mesh/components_vector/optional_tex_coord_vector.h
	include/mgp/mesh/components_vector/optional_wedge_tex_coords_vector.h
	include/mgp/mesh/components_vector/components_vector.h
	include/mgp/mesh/requirements/element_requirements.h
	include/mgp/mesh/requirements/face_requirements.h
	include/mgp/mesh/requirements/mesh_requirements.h
	include/mgp/mesh/requirements/vertex_requirements.h
	include/mgp/misc/compactness.h
	include/mgp/misc/timer.h
	include/mgp/misc/tokenizer.h
	include/mgp/io/file_mesh_info.h
	include/mgp/io/load_ply.h
	include/mgp/io/save_ply.h
	include/mgp/io/ply/ply.h
	include/mgp/io/ply/ply_face.h
	include/mgp/io/ply/ply_header.h
	include/mgp/io/ply/ply_vertex.h
	include/mgp/space/box.h
	include/mgp/space/color.h
	include/mgp/space/point.h
	include/mgp/space/tex_coord.h
	include/mgp/space/point/point_t.h
	include/mgp/space/point/point2.h
	include/mgp/space/point/point3.h
	include/mgp/space/point/point4.h)

set(SOURCES
	include/mgp/algorithms/clean.cpp
	include/mgp/algorithms/smooth.cpp
	include/mgp/algorithms/create/tetrahedron.cpp
	include/mgp/algorithms/update/bounding_box.cpp
	include/mgp/algorithms/update/flag.cpp
	include/mgp/algorithms/update/normal.cpp
	include/mgp/algorithms/update/topology.cpp
	include/mgp/mesh/mesh.cpp
	include/mgp/mesh/container/face_container.cpp
	include/mgp/mesh/container/vertex_container.cpp
	include/mgp/mesh/components/bit_flags.cpp
	include/mgp/mesh/components/element_references.cpp
	include/mgp/mesh/components/generic_container.cpp
	include/mgp/mesh/components/mutable_bit_flags.cpp
	include/mgp/mesh/components/polygon_bit_flags.cpp
	include/mgp/mesh/components/triangle_bit_flags.cpp
	include/mgp/mesh/components_optional/optional_adjacent_faces.cpp
	include/mgp/mesh/components_optional/optional_adjacent_vertices.cpp
	include/mgp/mesh/components_optional/optional_mutable_bit_flags.cpp
	include/mgp/mesh/components_optional/optional_wedge_tex_coords.cpp
	include/mgp/mesh/components_vector/custom_components_vector.cpp
	include/mgp/mesh/components_vector/optional_generic_vector.cpp
	include/mgp/mesh/elements/face.cpp
	include/mgp/mesh/requirements/element_requirements.cpp
	include/mgp/mesh/requirements/face_requirements.cpp
	include/mgp/mesh/requirements/mesh_requirements.cpp
	include/mgp/mesh/requirements/vertex_requirements.cpp
	include/mgp/misc/compactness.cpp
	include/mgp/misc/timer.cpp
	include/mgp/misc/tokenizer.cpp
	include/mgp/io/file_mesh_info.cpp
	include/mgp/io/load_ply.cpp
	include/mgp/io/save_ply.cpp
	include/mgp/io/ply/ply.cpp
	include/mgp/io/ply/ply_face.cpp
	include/mgp/io/ply/ply_header.cpp
	include/mgp/io/ply/ply_vertex.cpp
	include/mgp/space/box.cpp
	include/mgp/space/color.cpp
	include/mgp/space/tex_coord.cpp
	include/mgp/space/point/point_t.cpp
	include/mgp/space/point/point2.cpp
	include/mgp/space/point/point3.cpp
	include/mgp/space/point/point4.cpp)

add_library(mgp INTERFACE)

target_include_directories(mgp INTERFACE ${MGP_INCLUDE_DIR})
target_link_libraries(mgp INTERFACE mgp-external-eigen)

if (MGP_IDE)
	add_custom_target(mgp_ide SOURCES ${HEADERS} ${SOURCES})
endif()

if (MGP_BUILD_TESTS)
	add_subdirectory(test)
endif()

if (MGP_BUILD_DOCS)
	add_subdirectory(docs)
endif()
