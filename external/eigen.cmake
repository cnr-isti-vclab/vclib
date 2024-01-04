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

find_package(Eigen3 QUIET)

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

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-eigen)
