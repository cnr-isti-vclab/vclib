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
    add_library(vclib-external-vcg INTERFACE)
    target_include_directories(vclib-external-vcg INTERFACE
        ${VCG_INCLUDE_DIRS})
    set_target_properties(
        vclib-external-vcg
        PROPERTIES
        VCG_INCLUDE_DIRS ${VCG_INCLUDE_DIRS})

    list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-vcg)
endif()
