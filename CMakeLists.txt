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

# external libraries
add_subdirectory(external)

if (WIN32)
	add_compile_definitions(_USE_MATH_DEFINES)
endif()

set(VCLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(VCLIB_SRC_DIR ${VCLIB_INCLUDE_DIR}/vclib)

set(HEADERS
	include/vclib/polymesh.h
	include/vclib/trimesh.h
	include/vclib/algorithms/clean.h
	include/vclib/algorithms/smooth.h
	include/vclib/algorithms/create/tetrahedron.h
	include/vclib/algorithms/update/bounding_box.h
	include/vclib/algorithms/update/flag.h
	include/vclib/algorithms/update/normal.h
	include/vclib/algorithms/update/topology.h
	include/vclib/exception/io_exception.h
	include/vclib/exception/mesh_exception.h
	include/vclib/math/base.h
	include/vclib/mesh/mesh.h
	include/vclib/mesh/mesh_argument.h
	include/vclib/mesh/mesh_components.h
	include/vclib/mesh/mesh_pos.h
	include/vclib/mesh/requirements.h
	include/vclib/mesh/container/containers.h
	include/vclib/mesh/container/element_container.h
	include/vclib/mesh/container/face_container.h
	include/vclib/mesh/container/vertex_container.h
	include/vclib/mesh/elements/face.h
	include/vclib/mesh/elements/face_components.h
	include/vclib/mesh/elements/face_components_optional.h
	include/vclib/mesh/elements/vertex.h
	include/vclib/mesh/elements/vertex_components.h
	include/vclib/mesh/elements/vertex_components_optional.h
	include/vclib/mesh/iterators/container_iterator.h
	include/vclib/mesh/iterators/container_range_iterator.h
	include/vclib/mesh/iterators/range_iterator.h
	include/vclib/mesh/components/adjacent_faces.h
	include/vclib/mesh/components/adjacent_vertices.h
	include/vclib/mesh/components/bit_flags.h
	include/vclib/mesh/components/bounding_box.h
	include/vclib/mesh/components/color.h
	include/vclib/mesh/components/coordinate.h
	include/vclib/mesh/components/element_references.h
	include/vclib/mesh/components/generic_container.h
	include/vclib/mesh/components/mark.h
	include/vclib/mesh/components/mutable_bit_flags.h
	include/vclib/mesh/components/normal.h
	include/vclib/mesh/components/polygon_bit_flags.h
	include/vclib/mesh/components/scalar.h
	include/vclib/mesh/components/vertex_references.h
	include/vclib/mesh/components/tex_coord.h
	include/vclib/mesh/components/triangle_bit_flags.h
	include/vclib/mesh/components/wedge_tex_coords.h
	include/vclib/mesh/components_optional/custom_components.h
	include/vclib/mesh/components_optional/optional_adjacent_faces.h
	include/vclib/mesh/components_optional/optional_adjacent_vertices.h
	include/vclib/mesh/components_optional/optional_color.h
	include/vclib/mesh/components_optional/optional_info.h
	include/vclib/mesh/components_optional/optional_mark.h
	include/vclib/mesh/components_optional/optional_mutable_bit_flags.h
	include/vclib/mesh/components_optional/optional_normal.h
	include/vclib/mesh/components_optional/optional_scalar.h
	include/vclib/mesh/components_optional/optional_tex_coord.h
	include/vclib/mesh/components_optional/optional_wedge_tex_coords.h
	include/vclib/mesh/components_vector/optional_color_vector.h
	include/vclib/mesh/components_vector/custom_components_vector.h
	include/vclib/mesh/components_vector/optional_adjacent_faces_vector.h
	include/vclib/mesh/components_vector/optional_adjacent_vertices_vector.h
	include/vclib/mesh/components_vector/optional_generic_vector.h
	include/vclib/mesh/components_vector/optional_mark_vector.h
	include/vclib/mesh/components_vector/optional_mutable_bit_flags_vector.h
	include/vclib/mesh/components_vector/optional_normal_vector.h
	include/vclib/mesh/components_vector/optional_scalar_vector.h
	include/vclib/mesh/components_vector/optional_tex_coord_vector.h
	include/vclib/mesh/components_vector/optional_wedge_tex_coords_vector.h
	include/vclib/mesh/components_vector/components_vector.h
	include/vclib/mesh/requirements/element_requirements.h
	include/vclib/mesh/requirements/face_requirements.h
	include/vclib/mesh/requirements/mesh_requirements.h
	include/vclib/mesh/requirements/vertex_requirements.h
	include/vclib/misc/compactness.h
	include/vclib/misc/timer.h
	include/vclib/misc/tokenizer.h
	include/vclib/io/file_mesh_info.h
	include/vclib/io/load_ply.h
	include/vclib/io/save_ply.h
	include/vclib/io/ply/ply.h
	include/vclib/io/ply/ply_face.h
	include/vclib/io/ply/ply_header.h
	include/vclib/io/ply/ply_vertex.h
	include/vclib/space/box.h
	include/vclib/space/color.h
	include/vclib/space/point.h
	include/vclib/space/tex_coord.h
	include/vclib/space/point/point_t.h
	include/vclib/space/point/point2.h
	include/vclib/space/point/point3.h
	include/vclib/space/point/point4.h)

set(SOURCES
	include/vclib/algorithms/clean.cpp
	include/vclib/algorithms/smooth.cpp
	include/vclib/algorithms/create/tetrahedron.cpp
	include/vclib/algorithms/update/bounding_box.cpp
	include/vclib/algorithms/update/flag.cpp
	include/vclib/algorithms/update/normal.cpp
	include/vclib/algorithms/update/topology.cpp
	include/vclib/mesh/mesh.cpp
	include/vclib/mesh/mesh_pos.cpp
	include/vclib/mesh/container/face_container.cpp
	include/vclib/mesh/container/vertex_container.cpp
	include/vclib/mesh/components/bit_flags.cpp
	include/vclib/mesh/components/element_references.cpp
	include/vclib/mesh/components/generic_container.cpp
	include/vclib/mesh/components/mutable_bit_flags.cpp
	include/vclib/mesh/components/polygon_bit_flags.cpp
	include/vclib/mesh/components/triangle_bit_flags.cpp
	include/vclib/mesh/components_optional/optional_adjacent_faces.cpp
	include/vclib/mesh/components_optional/optional_adjacent_vertices.cpp
	include/vclib/mesh/components_optional/optional_mutable_bit_flags.cpp
	include/vclib/mesh/components_optional/optional_wedge_tex_coords.cpp
	include/vclib/mesh/components_vector/custom_components_vector.cpp
	include/vclib/mesh/components_vector/optional_generic_vector.cpp
	include/vclib/mesh/elements/face.cpp
	include/vclib/mesh/requirements/element_requirements.cpp
	include/vclib/mesh/requirements/face_requirements.cpp
	include/vclib/mesh/requirements/mesh_requirements.cpp
	include/vclib/mesh/requirements/vertex_requirements.cpp
	include/vclib/misc/compactness.cpp
	include/vclib/misc/timer.cpp
	include/vclib/misc/tokenizer.cpp
	include/vclib/io/file_mesh_info.cpp
	include/vclib/io/load_ply.cpp
	include/vclib/io/save_ply.cpp
	include/vclib/io/ply/ply.cpp
	include/vclib/io/ply/ply_face.cpp
	include/vclib/io/ply/ply_header.cpp
	include/vclib/io/ply/ply_vertex.cpp
	include/vclib/space/box.cpp
	include/vclib/space/color.cpp
	include/vclib/space/tex_coord.cpp
	include/vclib/space/point/point_t.cpp
	include/vclib/space/point/point2.cpp
	include/vclib/space/point/point3.cpp
	include/vclib/space/point/point4.cpp)

add_library(vclib INTERFACE)

target_include_directories(vclib INTERFACE ${VCLIB_INCLUDE_DIR})
target_link_libraries(vclib INTERFACE vclib-external-eigen)

if (VCLIB_IDE)
	add_custom_target(vclib_ide SOURCES ${HEADERS} ${SOURCES})
endif()

if (VCLIB_BUILD_TESTS)
	add_subdirectory(test)
endif()

if (VCLIB_BUILD_DOCS)
	add_subdirectory(docs)
endif()
