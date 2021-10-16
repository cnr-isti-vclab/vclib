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
	${MGP_SRC_DIR}/geometry/point_t.h
	${MGP_SRC_DIR}/geometry/point.h
	${MGP_SRC_DIR}/geometry/point2.h
	${MGP_SRC_DIR}/geometry/point3.h
	${MGP_SRC_DIR}/geometry/point4.h
	${MGP_SRC_DIR}/mesh/vertex/vertex.h)

set(SOURCES
	${MGP_SRC_DIR}/geometry/point_t.tpp
	${MGP_SRC_DIR}/geometry/point2.tpp
	${MGP_SRC_DIR}/geometry/point3.tpp
	${MGP_SRC_DIR}/geometry/point4.tpp)

add_library(mgp INTERFACE)

target_include_directories(mgp INTERFACE ${MGP_INCLUDE_DIR})
target_link_libraries(mgp INTERFACE mgp-external-eigen)

if (MGP_IDE)
	add_custom_target(mgp_ide SOURCES ${HEADERS} ${SOURCES})
endif()

if (MGP_BUILD_TESTS)
	add_subdirectory(test)
endif()
