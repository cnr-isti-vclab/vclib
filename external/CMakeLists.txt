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
project(vclib-external)

# Eigen options
option(ALLOW_BUNDLED_EIGEN "Allow use of bundled Eigen source" ON)
option(ALLOW_SYSTEM_EIGEN "Allow use of system-provided Eigen" ON)

find_package(Eigen3)

### Eigen
set(VCLIB_EIGEN_DIR ${CMAKE_CURRENT_LIST_DIR}/eigen-3.4.0)

if(ALLOW_SYSTEM_EIGEN AND EIGEN3_INCLUDE_DIR)
	message(STATUS "- Eigen - using system-provided library")
	set(EIGEN_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIR})
elseif(ALLOW_BUNDLED_EIGEN AND EXISTS "${VCLIB_EIGEN_DIR}/Eigen/Eigen")
	message(STATUS "- Eigen - using bundled source")
	set(EIGEN_INCLUDE_DIRS ${VCLIB_EIGEN_DIR})
else()
	message(
		FATAL_ERROR
		"Eigen is required - at least one of ALLOW_SYSTEM_EIGEN or ALLOW_BUNDLED_EIGEN must be enabled and found.")
endif()

add_library(vclib-external-eigen INTERFACE)
target_include_directories(vclib-external-eigen INTERFACE ${EIGEN_INCLUDE_DIRS})
