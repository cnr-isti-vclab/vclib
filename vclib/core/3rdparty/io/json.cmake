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

if (VCLIB_ALLOW_DOWNLOAD_NLOHMANN_JSON)
    message(STATUS "- NLohmannJSON - using downloaded source")

    set(NLOHMANN_JSON_VERSION 3.11.3)

    if (${VCLIB_ALLOW_INSTALL_NLOHMANN_JSON})
        set(JSON_Install ON)
        set(VCLIB_EXCLUDE_JSON "")
    else()
        set(JSON_Install OFF)
        set(VCLIB_EXCLUDE_JSON "EXCLUDE_FROM_ALL")
    endif()


    FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v${NLOHMANN_JSON_VERSION}
        ${VCLIB_EXCLUDE_JSON})
    FetchContent_MakeAvailable(json)

    add_library(vclib-3rd-json INTERFACE)
    target_link_libraries(vclib-3rd-json INTERFACE nlohmann_json::nlohmann_json)

    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-json)

    target_compile_definitions(vclib-3rd-json INTERFACE
        VCLIB_WITH_JSON)
endif()
