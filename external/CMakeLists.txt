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
project(vclib-external)

# Eigen options
option(VCLIB_ALLOW_BUNDLED_EIGEN "Allow use of bundled Eigen source" ON)
option(VCLIB_ALLOW_SYSTEM_EIGEN "Allow use of system-provided Eigen" ON)

# MapBox earcut
option(VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT "Allow use of bundled Mapbox-Eaurcut source" ON)

find_package(Eigen3 QUIET)

### Eigen
set(VCLIB_EIGEN_DIR ${CMAKE_CURRENT_LIST_DIR}/eigen-3.4.0)

if(VCLIB_ALLOW_SYSTEM_EIGEN AND EIGEN3_INCLUDE_DIR)
	message(STATUS "- Eigen - using system-provided library")
	set(EIGEN_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIR})
elseif(VCLIB_ALLOW_BUNDLED_EIGEN AND EXISTS "${VCLIB_EIGEN_DIR}/Eigen/Eigen")
	message(STATUS "- Eigen - using bundled source")
	set(EIGEN_INCLUDE_DIRS ${VCLIB_EIGEN_DIR})
else()
	message(
		FATAL_ERROR
		"Eigen is required - at least one of VCLIB_ALLOW_SYSTEM_EIGEN or VCLIB_ALLOW_BUNDLED_EIGEN must be enabled and found.")
endif()

add_library(vclib-external-eigen INTERFACE)
target_include_directories(vclib-external-eigen INTERFACE ${EIGEN_INCLUDE_DIRS})

### Mapbox-Earcut
set(VCLIB_MAPBOX_EARCUT_DIR ${CMAKE_CURRENT_LIST_DIR}/earcut.hpp-2.2.3)

if (VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT)
	message(STATUS "- Mapbox-Eaurcut - using bundled source")
	set(MAPBOX_EARCUT_INCLUDE_DIRS ${VCLIB_MAPBOX_EARCUT_DIR}/include)
else()
	message(
		FATAL_ERROR
		"MapBox earcut is required - VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT must be enabled and found.")
endif()

add_library(vclib-external-mapbox-earcut INTERFACE)
target_include_directories(vclib-external-mapbox-earcut INTERFACE ${MAPBOX_EARCUT_INCLUDE_DIRS})
