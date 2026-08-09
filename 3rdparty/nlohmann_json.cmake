# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(NLOHMANN_JSON_VERSION 3.12.0)

if(VCLIB_ALLOW_SYSTEM_NLOHMANN_JSON)
    find_package(nlohmann_json QUIET)
endif()

if(VCLIB_ALLOW_SYSTEM_NLOHMANN_JSON AND nlohmann_json_FOUND)
    message(STATUS "- nlohmann_json - using system-provided library")
    set(VCLIB_USED_SYSTEM_NLOHMANN_JSON ON CACHE INTERNAL "")

    add_library(vclib-3rd-nlohmann_json INTERFACE)
    target_link_libraries(vclib-3rd-nlohmann_json INTERFACE nlohmann_json::nlohmann_json)
    list(APPEND VCLIB_CORE_OPTIONAL_SYSTEM_LIBRARIES vclib-3rd-nlohmann_json)

elseif(VCLIB_ALLOW_DOWNLOAD_NLOHMANN_JSON)
    message(STATUS "- nlohmann_json - using downloaded source")
    set(VCLIB_USED_SYSTEM_NLOHMANN_JSON OFF CACHE INTERNAL "")

    if(VCLIB_ALLOW_INSTALL_NLOHMANN_JSON)
        set(JSON_Install ON CACHE BOOL "" FORCE)
    else()
        set(JSON_Install OFF CACHE BOOL "" FORCE)
    endif()
    set(JSON_BuildTests OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v${NLOHMANN_JSON_VERSION}
    )
    FetchContent_MakeAvailable(nlohmann_json)

    add_library(vclib-3rd-nlohmann_json INTERFACE)
    target_link_libraries(vclib-3rd-nlohmann_json INTERFACE nlohmann_json::nlohmann_json)
    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-nlohmann_json)
else()
    message(
        FATAL_ERROR
        "nlohmann_json is required - VCLIB_ALLOW_DOWNLOAD_NLOHMANN_JSON must be enabled if not found."
    )
endif()
