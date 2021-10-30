# This file is part of mgplib: https://github.com/alemuntoni/mgplib
# This Source Code Form is subject to the terms of the GNU GPL 3.0

cmake_minimum_required(VERSION 3.13)
project(mgplib)

### Build settings
set(CMAKE_CXX_STANDARD 17)

### Options
option(MGP_BUILD_TESTS "Build MGP tests" ON)
option(MGP_IDE "Create an utility target to show the library tree on IDEs" ON)

add_subdirectory(external)

set(MGP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MGP_SRC_DIR ${MGP_INCLUDE_DIR}/mgp)

set(HEADERS
	${MGP_SRC_DIR}/polymesh.h
	${MGP_SRC_DIR}/trimesh.h
	${MGP_SRC_DIR}/algorithms/smooth.h
	${MGP_SRC_DIR}/algorithms/update/flag.h
	${MGP_SRC_DIR}/algorithms/update/topology.h
	${MGP_SRC_DIR}/exception/io_exception.h
	${MGP_SRC_DIR}/exception/mesh_exception.h
	${MGP_SRC_DIR}/mesh/face.h
	${MGP_SRC_DIR}/mesh/face_components.h
	${MGP_SRC_DIR}/mesh/face_components_optional.h
	${MGP_SRC_DIR}/mesh/mesh.h
	${MGP_SRC_DIR}/mesh/requirements.h
	${MGP_SRC_DIR}/mesh/vertex.h
	${MGP_SRC_DIR}/mesh/vertex_components.h
	${MGP_SRC_DIR}/mesh/vertex_components_optional.h
	${MGP_SRC_DIR}/mesh/components/bit_flags.h
	${MGP_SRC_DIR}/mesh/components/color.h
	${MGP_SRC_DIR}/mesh/components/coordinate.h
	${MGP_SRC_DIR}/mesh/components/face_references.h
	${MGP_SRC_DIR}/mesh/components/mutable_bit_flags.h
	${MGP_SRC_DIR}/mesh/components/normal.h
	${MGP_SRC_DIR}/mesh/components/scalar.h
	${MGP_SRC_DIR}/mesh/components/vertex_references.h
	${MGP_SRC_DIR}/mesh/components/triangle_bit_flags.h
	${MGP_SRC_DIR}/mesh/components_optional/custom_components.h
	${MGP_SRC_DIR}/mesh/components_optional/optional_color.h
	${MGP_SRC_DIR}/mesh/components_optional/optional_face_references.h
	${MGP_SRC_DIR}/mesh/components_optional/optional_info.h
	${MGP_SRC_DIR}/mesh/components_optional/optional_mutable_bit_flags.h
	${MGP_SRC_DIR}/mesh/components_optional/optional_normal.h
	${MGP_SRC_DIR}/mesh/components_optional/optional_scalar.h
	${MGP_SRC_DIR}/mesh/components_vector/optional_color_vector.h
	${MGP_SRC_DIR}/mesh/components_vector/components_vector.h
	${MGP_SRC_DIR}/mesh/components_vector/custom_components_vector.h
	${MGP_SRC_DIR}/mesh/components_vector/optional_adjacent_faces_ref_vector.h
	${MGP_SRC_DIR}/mesh/components_vector/optional_generic_vector.h
	${MGP_SRC_DIR}/mesh/components_vector/optional_mutable_bit_flags_vector.h
	${MGP_SRC_DIR}/mesh/components_vector/optional_normal_vector.h
	${MGP_SRC_DIR}/mesh/components_vector/optional_scalar_vector.h
	${MGP_SRC_DIR}/mesh/container/container_t.h
	${MGP_SRC_DIR}/mesh/container/containers.h
	${MGP_SRC_DIR}/mesh/container/face_container.h
	${MGP_SRC_DIR}/mesh/container/vertex_container.h
	${MGP_SRC_DIR}/mesh/iterators/container_iterator.h
	${MGP_SRC_DIR}/mesh/iterators/container_range_iterator.h
	${MGP_SRC_DIR}/mesh/iterators/range_iterator.h
	${MGP_SRC_DIR}/mesh/requirements/element_requirements.h
	${MGP_SRC_DIR}/mesh/requirements/face_requirements.h
	${MGP_SRC_DIR}/mesh/requirements/mesh_requirements.h
	${MGP_SRC_DIR}/mesh/requirements/vertex_requirements.h
	${MGP_SRC_DIR}/misc/timer.h
	${MGP_SRC_DIR}/misc/tokenizer.h
	${MGP_SRC_DIR}/io/file_mesh_mode.h
	${MGP_SRC_DIR}/io/load_ply.h
	${MGP_SRC_DIR}/io/save_ply.h
	${MGP_SRC_DIR}/io/ply/ply.h
	${MGP_SRC_DIR}/io/ply/ply_face.h
	${MGP_SRC_DIR}/io/ply/ply_header.h
	${MGP_SRC_DIR}/io/ply/ply_vertex.h
	${MGP_SRC_DIR}/space/box.h
	${MGP_SRC_DIR}/space/color.h
	${MGP_SRC_DIR}/space/point.h
	${MGP_SRC_DIR}/space/point/point_t.h
	${MGP_SRC_DIR}/space/point/point2.h
	${MGP_SRC_DIR}/space/point/point3.h
	${MGP_SRC_DIR}/space/point/point4.h)

set(SOURCES
	${MGP_SRC_DIR}/algorithms/smooth.cpp
	${MGP_SRC_DIR}/algorithms/update/flag.cpp
	${MGP_SRC_DIR}/algorithms/update/topology.cpp
	${MGP_SRC_DIR}/mesh/mesh.cpp
	${MGP_SRC_DIR}/mesh/components/bit_flags.cpp
	${MGP_SRC_DIR}/mesh/components/mutable_bit_flags.cpp
	${MGP_SRC_DIR}/mesh/components/vertex_references.cpp
	${MGP_SRC_DIR}/mesh/components/triangle_bit_flags.cpp
	${MGP_SRC_DIR}/mesh/components_optional/optional_mutable_bit_flags.cpp
	${MGP_SRC_DIR}/mesh/container/face_container.cpp
	${MGP_SRC_DIR}/mesh/container/vertex_container.cpp
	${MGP_SRC_DIR}/mesh/requirements/element_requirements.cpp
	${MGP_SRC_DIR}/mesh/requirements/face_requirements.cpp
	${MGP_SRC_DIR}/mesh/requirements/mesh_requirements.cpp
	${MGP_SRC_DIR}/mesh/requirements/vertex_requirements.cpp
	${MGP_SRC_DIR}/misc/timer.cpp
	${MGP_SRC_DIR}/misc/tokenizer.cpp
	${MGP_SRC_DIR}/io/load_ply.cpp
	${MGP_SRC_DIR}/io/save_ply.cpp
	${MGP_SRC_DIR}/io/ply/ply.cpp
	${MGP_SRC_DIR}/io/ply/ply_face.cpp
	${MGP_SRC_DIR}/io/ply/ply_header.cpp
	${MGP_SRC_DIR}/io/ply/ply_vertex.cpp
	${MGP_SRC_DIR}/space/box.cpp
	${MGP_SRC_DIR}/space/color.cpp
	${MGP_SRC_DIR}/space/point/point_t.cpp
	${MGP_SRC_DIR}/space/point/point2.cpp
	${MGP_SRC_DIR}/space/point/point3.cpp
	${MGP_SRC_DIR}/space/point/point4.cpp)

add_library(mgp INTERFACE)

target_include_directories(mgp INTERFACE ${MGP_INCLUDE_DIR})
target_link_libraries(mgp INTERFACE mgp-external-eigen)

if (MGP_IDE)
	add_custom_target(mgp_ide SOURCES ${HEADERS} ${SOURCES})
endif()

if (MGP_BUILD_TESTS)
	add_subdirectory(test)
endif()
