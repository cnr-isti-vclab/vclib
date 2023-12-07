#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
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

set(VCLIB_MAPBOX_EARCUT_DIR ${CMAKE_CURRENT_LIST_DIR}/earcut.hpp-2.2.3)

if (VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT AND EXISTS ${VCLIB_MAPBOX_EARCUT_DIR}/include/mapbox/earcut.hpp)
    message(STATUS "- Mapbox-Eaurcut - using bundled source")
else()
    message(
        FATAL_ERROR
        "MapBox earcut is required - VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT must be enabled and found.")
endif()

set(MAPBOX_EARCUT_INCLUDE_DIRS ${VCLIB_MAPBOX_EARCUT_DIR}/include)

add_library(vclib-external-mapbox-earcut INTERFACE)

target_include_directories(vclib-external-mapbox-earcut INTERFACE ${MAPBOX_EARCUT_INCLUDE_DIRS})

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-mapbox-earcut)
