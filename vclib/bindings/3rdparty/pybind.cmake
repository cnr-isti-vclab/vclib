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

if (VCLIB_ALLOW_DOWNLOAD_PYBIND)
    find_package(Python3 QUIET)
    set(VCLIB_PYTHON_VERSION
        "${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}" PARENT_SCOPE)

    message(STATUS "- pybind11 - using downloaded source")
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11
        GIT_TAG        v2.13.6
        EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(pybind11)
else()
    message(FATAL_ERROR "pybind11 is required - please enable VCLIB_ALLOW_DOWNLOAD_PYBIND")
endif()


