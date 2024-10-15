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

find_package(Eigen3 QUIET)

set(VCLIB_EIGEN_DIR ${CMAKE_CURRENT_LIST_DIR}/eigen-3.4.0)

if(VCLIB_ALLOW_SYSTEM_EIGEN AND TARGET Eigen3::Eigen)
    message(STATUS "- Eigen - using system-provided library")
    add_library(vclib-3rd-eigen INTERFACE)
    target_link_libraries(vclib-3rd-eigen INTERFACE Eigen3::Eigen)
elseif(VCLIB_ALLOW_BUNDLED_EIGEN AND EXISTS "${VCLIB_EIGEN_DIR}/Eigen/Eigen")
    message(STATUS "- Eigen - using bundled source")
    add_library(vclib-3rd-eigen INTERFACE)
    target_include_directories(vclib-3rd-eigen INTERFACE ${VCLIB_EIGEN_DIR})
else()
    message(
        FATAL_ERROR
        "Eigen is required - at least one of VCLIB_ALLOW_SYSTEM_EIGEN or VCLIB_ALLOW_BUNDLED_EIGEN must be enabled and found.")
endif()

list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-eigen)
