# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(VCLIB_ZIP_VIEWS_DIR ${CMAKE_CURRENT_LIST_DIR}/zip-views-1.0)

if (VCLIB_ALLOW_BUNDLED_ZIP_VIEWS AND
        EXISTS ${VCLIB_ZIP_VIEWS_DIR}/zip_view.hpp)
    message(STATUS "- ZipViews - using bundled source")
else()
    message(
        FATAL_ERROR
        "ZipViews is required -
        VCLIB_ALLOW_BUNDLED_ZIP_VIEWS must be enabled and found.")
endif()

set(ZIP_VIEWS_INCLUDE_DIRS ${VCLIB_ZIP_VIEWS_DIR})

set(ZIP_VIEW_BUILD_TEST OFF CACHE BOOL "Build ZipViews tests")
add_subdirectory(${VCLIB_ZIP_VIEWS_DIR})

add_library(vclib-3rd-zip-views INTERFACE)

target_link_libraries(vclib-3rd-zip-views INTERFACE zip-view)

list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-zip-views)

# Install
if (VCLIB_ALLOW_INSTALL_ZIP_VIEWS)
    install(FILES ${VCLIB_ZIP_VIEWS_DIR}/zip_view.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
