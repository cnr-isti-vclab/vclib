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
	${MGP_SRC_DIR}/trimesh.h
	${MGP_SRC_DIR}/space/box.h
	${MGP_SRC_DIR}/space/color.h
	${MGP_SRC_DIR}/space/point.h
	${MGP_SRC_DIR}/space/point/point_t.h
	${MGP_SRC_DIR}/space/point/point2.h
	${MGP_SRC_DIR}/space/point/point3.h
	${MGP_SRC_DIR}/space/point/point4.h
	${MGP_SRC_DIR}/mesh/face.h
	${MGP_SRC_DIR}/mesh/mesh.h
	${MGP_SRC_DIR}/mesh/vertex.h
	${MGP_SRC_DIR}/mesh/common/bit_flags.h
	${MGP_SRC_DIR}/mesh/common/color.h
	${MGP_SRC_DIR}/mesh/common/custom_attributes.h
	${MGP_SRC_DIR}/mesh/common/info.h
	${MGP_SRC_DIR}/mesh/common/mutable_bit_flags.h
	${MGP_SRC_DIR}/mesh/common/normal.h
	${MGP_SRC_DIR}/mesh/common/scalar.h
	${MGP_SRC_DIR}/mesh/common/optional_color.h
	${MGP_SRC_DIR}/mesh/common/optional_info.h
	${MGP_SRC_DIR}/mesh/common/optional_mutable_bit_flags.h
	${MGP_SRC_DIR}/mesh/common/optional_normal.h
	${MGP_SRC_DIR}/mesh/common/optional_scalar.h
	${MGP_SRC_DIR}/mesh/container/container_t.h
	${MGP_SRC_DIR}/mesh/container/containers.h
	${MGP_SRC_DIR}/mesh/container/face_container.h
	${MGP_SRC_DIR}/mesh/component_vector/optional_color_vector.h
	${MGP_SRC_DIR}/mesh/component_vector/components_vector.h
	${MGP_SRC_DIR}/mesh/component_vector/custom_attributes_vector.h
	${MGP_SRC_DIR}/mesh/component_vector/optional_generic_vector.h
	${MGP_SRC_DIR}/mesh/component_vector/optional_mutable_bit_flags_vector.h
	${MGP_SRC_DIR}/mesh/component_vector/optional_normal_vector.h
	${MGP_SRC_DIR}/mesh/component_vector/optional_scalar_vector.h
	${MGP_SRC_DIR}/mesh/container/vertex_container.h
	${MGP_SRC_DIR}/mesh/face/common.h
	${MGP_SRC_DIR}/mesh/face/vertex_ref_array.h
	${MGP_SRC_DIR}/mesh/iterators/container_iterator.h
	${MGP_SRC_DIR}/mesh/iterators/container_range_iterator.h
	${MGP_SRC_DIR}/mesh/vertex/common.h
	${MGP_SRC_DIR}/mesh/vertex/coordinate.h
	${MGP_SRC_DIR}/mesh/vertex/face_adjacency_ref.h
	${MGP_SRC_DIR}/misc/type_traits.h)

set(SOURCES
	${MGP_SRC_DIR}/space/box.cpp
	${MGP_SRC_DIR}/space/color.cpp
	${MGP_SRC_DIR}/space/point/point_t.cpp
	${MGP_SRC_DIR}/space/point/point2.cpp
	${MGP_SRC_DIR}/space/point/point3.cpp
	${MGP_SRC_DIR}/space/point/point4.cpp
	${MGP_SRC_DIR}/mesh/common/bit_flags.cpp
	${MGP_SRC_DIR}/mesh/common/mutable_bit_flags.cpp
	${MGP_SRC_DIR}/mesh/common/optional_mutable_bit_flags.cpp
	${MGP_SRC_DIR}/mesh/container/face_container.cpp
	${MGP_SRC_DIR}/mesh/container/vertex_container.cpp
	${MGP_SRC_DIR}/mesh/face/vertex_ref_array.cpp
	${MGP_SRC_DIR}/mesh/mesh.cpp)

add_library(mgp INTERFACE)

target_include_directories(mgp INTERFACE ${MGP_INCLUDE_DIR})
target_link_libraries(mgp INTERFACE mgp-external-eigen)

if (MGP_IDE)
	add_custom_target(mgp_ide SOURCES ${HEADERS} ${SOURCES})
endif()

if (MGP_BUILD_TESTS)
	add_subdirectory(test)
endif()
