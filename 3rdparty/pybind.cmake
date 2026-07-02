# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if(VCLIB_ALLOW_DOWNLOAD_PYBIND)
    message(STATUS "- pybind11 - using downloaded source")

    set(PYBIND11_FINDPYTHON NEW)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11
        GIT_TAG v3.0.1
        EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(pybind11)

    # get Python version
    find_package(Python QUIET)
    set(VCLIB_PYTHON_VERSION
        "${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}"
        PARENT_SCOPE
    )
else()
    message(
        FATAL_ERROR
        "pybind11 is required - please enable VCLIB_ALLOW_DOWNLOAD_PYBIND"
    )
endif()
