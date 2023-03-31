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

if (APPLE)
	option(VCLIB_ALLOW_BUNDLED_PSTLD "Allow use of bundled pstld source" ON)
endif()

# Doctest
if (VCLIB_BUILD_TESTS)
	option(VCLIB_ALLOW_BUNDLED_DOCTEST "Allow use of bundled doctest source" ON)
endif()

# OpenGL
option(VCLIB_ALLOW_SYSTEM_OPENGL "Allow use of system-provided OpenGL" ON)

# Qt
option(VCLIB_ALLOW_SYSTEM_QT "Allow use of system-provided Qt" ON)

# QGLViewer
option(VCLIB_ALLOW_SYSTEM_QGLVIEWER "Allow use of system-provided QGLViewer" ON)

# STB
option(VCLIB_ALLOW_BUNDLED_STB "Allow use of bundled STB source" ON)

# TBB
option(VCLIB_ALLOW_SYSTEM_TBB "Allow use of system-provided TBB" ON)

set(VCLIB_EXTERNAL_LIBRARIES "")

# === REQUIRED === #

### Eigen
include(eigen.cmake)

### Mapbox-Earcut
include(mapbox.cmake)

### pstld, required on Apple clang (waiting for c++17 parallel algorithms support)
if (APPLE)
	include(pstld.cmake)
endif()

# === OPTIONAL === #

### Doctest
include(doctest.cmake)

### OpenGL
include(opengl.cmake)

### Qt
include(qt.cmake)

### QGLViewer
include(qglviewer.cmake)

### STB
include(stb.cmake)

### TBB
include(tbb.cmake)

set(VCLIB_EXTERNAL_LIBRARIES ${VCLIB_EXTERNAL_LIBRARIES} PARENT_SCOPE)
