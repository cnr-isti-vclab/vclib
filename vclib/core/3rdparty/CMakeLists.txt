#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the Mozilla Public License Version 2.0 as published *
#* by the Mozilla Foundation; either version 2 of the License, or            *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* Mozilla Public License Version 2.0                                        *
#* (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)
project(vclib-core-3rdparty)

include(FetchContent)

# === REQUIRED === #

# Eigen
option(VCLIB_ALLOW_BUNDLED_EIGEN "Allow use of bundled Eigen source" ON)
option(VCLIB_ALLOW_SYSTEM_EIGEN "Allow use of system-provided Eigen" ON)

# MapBox earcut
option(VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT
    "Allow use of bundled Mapbox-Eaurcut source" ON)
option(VCLIB_ALLOW_INSTALL_MAPBOX_EARCUT
    "Allow install Mapbox-Eaurcut source" ON)

# Zip iterator
option(VCLIB_ALLOW_BUNDLED_ZIP_VIEWS
    "Allow use of bundled ZipViews source" ON)
option(VCLIB_ALLOW_INSTALL_ZIP_VIEWS
    "Allow install ZipViews source" ON)

# poolSTL
option(VCLIB_ALLOW_BUNDLED_POOLSTL "Allow use of bundled poolSTL source" ON)
option(VCLIB_ALLOW_INSTALL_POOLSTL "Allow install poolSTL source" ON)

# === OPTIONAL === #

# TBB
option(VCLIB_ALLOW_SYSTEM_TBB "Allow use of system-provided TBB" ON)

# IO - 3rd Party libraries
if (VCLIB_BUILD_CORE_WITH_IO_3RDPARTY_LIBRARIES)
    # Qt
    option(VCLIB_ALLOW_SYSTEM_QT "Allow use of system-provided Qt" ON)

    # STB
    option(VCLIB_ALLOW_BUNDLED_STB "Allow use of bundled STB source" ON)
    option(VCLIB_ALLOW_INSTALL_STB "Allow install STB source" ON)
endif()


# === Fetch dependencies === #

set(VCLIB_CORE_3RDPARTY_LIBRARIES "")

### Eigen
include(eigen.cmake)

### Mapbox-Earcut
include(mapbox.cmake)

### Zip iterator
include(zip-views.cmake)

### TBB
include(tbb.cmake) # leave this BEFORE poolSTL

### poolSTL
include(poolstl.cmake)

### IO - 3rd Party libraries

if (VCLIB_BUILD_CORE_WITH_IO_3RDPARTY_LIBRARIES)
    # Qt
    include(io/qt.cmake)

    ### STB
    include(io/stb.cmake)
endif()


set(VCLIB_CORE_3RDPARTY_LIBRARIES ${VCLIB_CORE_3RDPARTY_LIBRARIES} PARENT_SCOPE)
