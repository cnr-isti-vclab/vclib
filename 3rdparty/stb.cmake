# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if(VCLIB_ALLOW_DOWNLOAD_STB)
    message(STATUS "- stb - using downloaded source")

    FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG master
        SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/stb_include/stb
    )
    FetchContent_MakeAvailable(stb)

    add_library(vclib-3rd-stb INTERFACE)

    target_include_directories(
        vclib-3rd-stb
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/stb_include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/stb_include/stb>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/stb>
    )

    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-stb)

    if(VCLIB_ALLOW_INSTALL_STB)
        # Export for installation
        install(
            DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/stb_include/stb
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "deprecated" EXCLUDE
            PATTERN "tests" EXCLUDE
            PATTERN "stb_image_resize_test" EXCLUDE
            PATTERN "tools" EXCLUDE
        )
    endif()
endif()
