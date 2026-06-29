# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

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
