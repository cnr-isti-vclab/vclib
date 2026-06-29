# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if (VCLIB_ALLOW_SYSTEM_VCG AND (DEFINED VCG_INCLUDE_DIRS OR DEFINED VCG_DIR))
    if (DEFINED VCG_DIR)
        set(VCG_INCLUDE_DIRS ${VCG_DIR})
    endif()
    message(STATUS "- VCG - using system-provided library")
    set(VCLIB_USES_VCG TRUE)
else()
    if (VCLIB_ALLOW_DOWNLOAD_VCG)
        message(STATUS "- VCG - using downloaded source")
        include(FetchContent)
        FetchContent_Declare(
            vcglib
            GIT_REPOSITORY https://github.com/cnr-isti-vclab/vcglib.git
            GIT_TAG        2025.07
            EXCLUDE_FROM_ALL)

        FetchContent_MakeAvailable(vcglib)
        set(VCG_INCLUDE_DIRS ${vcglib_SOURCE_DIR})
        set(VCLIB_USES_VCG TRUE)
    else()
        message(STATUS "- VCG - not found, skipping")
    endif()
endif()

if (VCLIB_USES_VCG)
    add_library(vclib-3rd-vcg INTERFACE)
    target_include_directories(vclib-3rd-vcg INTERFACE
        ${VCG_INCLUDE_DIRS})
    set_target_properties(
        vclib-3rd-vcg
        PROPERTIES
        VCG_INCLUDE_DIRS ${VCG_INCLUDE_DIRS})
    target_compile_definitions(vclib-3rd-vcg INTERFACE
        VCLIB_WITH_VCG)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-vcg)
endif()
