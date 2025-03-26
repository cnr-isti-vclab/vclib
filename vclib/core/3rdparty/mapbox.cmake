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

set(VCLIB_MAPBOX_EARCUT_DIR ${CMAKE_CURRENT_LIST_DIR}/earcut.hpp-2.2.3)

if (VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT AND
        EXISTS ${VCLIB_MAPBOX_EARCUT_DIR}/include/mapbox/earcut.hpp)
    message(STATUS "- Mapbox-Eaurcut - using bundled source")
else()
    message(FATAL_ERROR
        "MapBox earcut is required - VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT"
        "must be enabled and found.")
endif()

set(MAPBOX_EARCUT_INCLUDE_DIRS ${VCLIB_MAPBOX_EARCUT_DIR}/include)

add_library(vclib-3rd-mapbox-earcut INTERFACE)

target_include_directories(vclib-3rd-mapbox-earcut
    INTERFACE ${MAPBOX_EARCUT_INCLUDE_DIRS})

list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-mapbox-earcut)

# Install
if (VCLIB_ALLOW_INSTALL_MAPBOX_EARCUT)
    install(DIRECTORY ${VCLIB_MAPBOX_EARCUT_DIR}/include/mapbox
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
