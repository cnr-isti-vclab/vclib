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

if (VCLIB_ALLOW_DOWNLOAD_TINYGTLF)
    message(STATUS "- tinygltf - using downloaded source")

    set(TINYGLTF_VERSION 2.9.6)

    set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)

    if (NOT ${VCLIB_ALLOW_INSTALL_TINYGLTF})
        set(TINYGLTF_INSTALL OFF)
    endif()

    FetchContent_Declare(tinygltf
        GIT_REPOSITORY https://github.com/syoyo/tinygltf
        GIT_TAG        v${TINYGLTF_VERSION})
    FetchContent_MakeAvailable(tinygltf)
    set_property(TARGET tinygltf PROPERTY POSITION_INDEPENDENT_CODE ON)

    add_library(vclib-3rd-tinygltf INTERFACE)
    target_link_libraries(vclib-3rd-tinygltf INTERFACE tinygltf)

    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-tinygltf)

    target_compile_definitions(vclib-3rd-tinygltf INTERFACE
        VCLIB_WITH_JSON VCLIB_WITH_STB VCLIB_WITH_TINYGLTF)
endif()
