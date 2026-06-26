# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

find_package(Catch2 3 QUIET)

if(VCLIB_ALLOW_SYSTEM_CATCH2 AND TARGET Catch2::Catch2WithMain)
    message(STATUS "- Catch2 - using system-provided library")
elseif (VCLIB_ALLOW_DOWNLOAD_CATCH2)
    message(STATUS "- Catch2 - using downloaded source")

    FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.3.2)

    FetchContent_MakeAvailable(Catch2)
else()
    message(
        FATAL_ERROR
        "Catch2 is required to build tests - VCLIB_ALLOW_DOWNLOAD_CATCH2 must be enabled and found.")
endif()

target_link_libraries(vclib-tests-examples-common INTERFACE Catch2::Catch2WithMain)

