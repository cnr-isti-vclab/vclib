# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if(VCLIB_ALLOW_SYSTEM_VCG AND (DEFINED VCG_INCLUDE_DIRS OR DEFINED VCG_DIR))
    if(DEFINED VCG_DIR)
        set(VCG_INCLUDE_DIRS ${VCG_DIR})
    endif()
    message(STATUS "- VCG - using system-provided library")
    set(VCLIB_USED_SYSTEM_VCG ON CACHE INTERNAL "")
    set(VCLIB_USES_VCG TRUE)
else()
    if(VCLIB_ALLOW_DOWNLOAD_VCG)

        set(VCG_USE_OPENMP OFF)

        message(STATUS "- VCG - using downloaded source")
        include(FetchContent)
        if(NOT VCLIB_ALLOW_INSTALL_VCG)
            set(VCG_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
        else()
            set(VCG_EXCLUDE_FROM_ALL "")
        endif()

        FetchContent_Declare(
            vcglib
            GIT_REPOSITORY https://github.com/cnr-isti-vclab/vcglib.git
            GIT_TAG main
            ${VCG_EXCLUDE_FROM_ALL}
        )

        FetchContent_MakeAvailable(vcglib)
        set(VCG_INCLUDE_DIRS ${vcglib_SOURCE_DIR})
        set(VCLIB_USES_VCG TRUE)
    else()
        message(STATUS "- VCG - not found, skipping")
    endif()
endif()

if(VCLIB_USES_VCG)
    add_library(vclib-3rd-vcg INTERFACE)
    
    if(VCLIB_USED_SYSTEM_VCG)
        # Hide system include from export so it doesn't pollute the install interface
        target_include_directories(vclib-3rd-vcg INTERFACE $<BUILD_INTERFACE:${VCG_INCLUDE_DIRS}>)
    else()
        target_include_directories(vclib-3rd-vcg INTERFACE ${VCG_INCLUDE_DIRS})
    endif()
    
    set_target_properties(
        vclib-3rd-vcg
        PROPERTIES VCG_INCLUDE_DIRS ${VCG_INCLUDE_DIRS}
    )
    target_compile_definitions(vclib-3rd-vcg INTERFACE VCLIB_WITH_VCG)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-vcg)
endif()
