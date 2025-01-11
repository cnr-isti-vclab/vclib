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
            GIT_TAG        origin/devel) # TODO: change to next version of vcg

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

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-vcg)
endif()
