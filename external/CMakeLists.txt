#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2024                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the GNU General Public License as published by      *
#* the Free Software Foundation; either version 3 of the License, or         *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
#* for more details.                                                         *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)
project(vclib-external)

include(FetchContent)

# Eigen options
option(VCLIB_ALLOW_BUNDLED_EIGEN "Allow use of bundled Eigen source" ON)
option(VCLIB_ALLOW_SYSTEM_EIGEN "Allow use of system-provided Eigen" ON)

# MapBox earcut
option(VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT
    "Allow use of bundled Mapbox-Eaurcut source" ON)

# Zip iterator
option(VCLIB_ALLOW_BUNDLED_ZIP_VIEWS
    "Allow use of bundled ZipViews source" ON)

if (APPLE)
    option(VCLIB_ALLOW_BUNDLED_PSTLD "Allow use of bundled pstld source" ON)
endif()

# Catch2
if (VCLIB_BUILD_TESTS)
    option(VCLIB_ALLOW_DOWNLOAD_CATCH2
        "Allow use of downloaded Catch2 source" ON)
    option(VCLIB_ALLOW_SYSTEM_CATCH2 "Allow use of system-provided Catch2" ON)
endif()

# STB
option(VCLIB_ALLOW_BUNDLED_STB "Allow use of bundled STB source" ON)

# TBB
option(VCLIB_ALLOW_SYSTEM_TBB "Allow use of system-provided TBB" ON)

#VCG
option(VCLIB_ALLOW_DOWNLOAD_VCG "Allow use of downloaded VCG source" ON)
option(VCLIB_ALLOW_SYSTEM_VCG "Allow use of system-provided VCG" ON)

set(VCLIB_EXTERNAL_LIBRARIES "")

# === REQUIRED === #

### Eigen
include(eigen.cmake)

### Mapbox-Earcut
include(mapbox.cmake)

### Zip iterator
include(zip-views.cmake)

### pstld, required on AppleClang
### (waiting for c++17 parallel algorithms support)
if (APPLE)
    include(pstld.cmake)
endif()

# === OPTIONAL === #

### Catch2
include(catch2.cmake)

### STB
include(stb.cmake)

### TBB
include(tbb.cmake)

### VCG
include(vcg.cmake)

set(VCLIB_EXTERNAL_LIBRARIES ${VCLIB_EXTERNAL_LIBRARIES} PARENT_SCOPE)
