# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(VCLIB_EIGEN_DIR ${CMAKE_CURRENT_LIST_DIR}/eigen-5.0.1)

if(VCLIB_ALLOW_SYSTEM_EIGEN AND TARGET Eigen3::Eigen)
    message(STATUS "- Eigen - using system-provided library")
    set(VCLIB_USED_SYSTEM_EIGEN ON CACHE INTERNAL "")
    add_library(vclib-3rd-eigen INTERFACE)
    target_link_libraries(vclib-3rd-eigen INTERFACE Eigen3::Eigen)
elseif(VCLIB_ALLOW_BUNDLED_EIGEN AND EXISTS "${VCLIB_EIGEN_DIR}/Eigen/Eigen")
    message(STATUS "- Eigen - using bundled source")
    add_library(vclib-3rd-eigen INTERFACE)
    target_include_directories(vclib-3rd-eigen INTERFACE ${VCLIB_EIGEN_DIR})
    add_library(Eigen3::Eigen ALIAS vclib-3rd-eigen)
else()
    message(
        FATAL_ERROR
        "Eigen is required - at least one of VCLIB_ALLOW_SYSTEM_EIGEN or VCLIB_ALLOW_BUNDLED_EIGEN must be enabled and found."
    )
endif()

list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-eigen)

# Install
if(
    VCLIB_ALLOW_BUNDLED_EIGEN
    AND VCLIB_ALLOW_INSTALL_EIGEN
    AND EXISTS "${VCLIB_EIGEN_DIR}/Eigen/Eigen"
)
    install(
        DIRECTORY ${VCLIB_EIGEN_DIR}/Eigen
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()
