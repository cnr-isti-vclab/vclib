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

add_library(vclib-external-zip-views INTERFACE)

target_link_libraries(vclib-external-zip-views INTERFACE zip-view)
target_compile_definitions(vclib-external-zip-views INTERFACE
    ZIP_VIEW_INJECT_STD_VIEWS_NAMESPACE)

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-zip-views)
