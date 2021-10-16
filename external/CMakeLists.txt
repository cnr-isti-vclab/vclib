# This file is part of mgplib: https://github.com/alemuntoni/mgplib
# This Source Code Form is subject to the terms of the GNU GPL 3.0

cmake_minimum_required(VERSION 3.13)
project(mgp-external)

# Eigen options
option(ALLOW_BUNDLED_EIGEN "Allow use of bundled Eigen source" ON)
option(ALLOW_SYSTEM_EIGEN "Allow use of system-provided Eigen" ON)

find_package(Eigen3)

### Eigen
set(MGP_EIGEN_DIR ${CMAKE_CURRENT_LIST_DIR}/eigen-3.4.0)

if(ALLOW_SYSTEM_EIGEN AND EIGEN3_INCLUDE_DIR)
	message(STATUS "- Eigen - using system-provided library")
	set(EIGEN_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIR})
elseif(ALLOW_BUNDLED_EIGEN AND EXISTS "${MGP_EIGEN_DIR}/Eigen/Eigen")
	message(STATUS "- Eigen - using bundled source")
	set(EIGEN_INCLUDE_DIRS ${MGP_EIGEN_DIR})
else()
	message(
		FATAL_ERROR
		"Eigen is required - at least one of ALLOW_SYSTEM_EIGEN or ALLOW_BUNDLED_EIGEN must be enabled and found.")
endif()

add_library(mgp-external-eigen INTERFACE)
target_include_directories(mgp-external-eigen INTERFACE ${EIGEN_INCLUDE_DIRS})
