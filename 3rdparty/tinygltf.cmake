# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if(VCLIB_ALLOW_DOWNLOAD_TINYGTLF)
    message(STATUS "- tinygltf - using downloaded source")

    set(TINYGLTF_VERSION 3.0.0)

    set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE BOOL "" FORCE)

    if(NOT ${VCLIB_ALLOW_INSTALL_TINYGLTF})
        set(TINYGLTF_INSTALL OFF CACHE BOOL "" FORCE)
    endif()

    FetchContent_Declare(
        tinygltf
        GIT_REPOSITORY https://github.com/syoyo/tinygltf
        GIT_TAG v${TINYGLTF_VERSION}
    )
    FetchContent_MakeAvailable(tinygltf)
    set_property(TARGET tinygltf PROPERTY POSITION_INDEPENDENT_CODE ON)

    add_library(vclib-3rd-tinygltf INTERFACE)
    target_link_libraries(vclib-3rd-tinygltf INTERFACE tinygltf)

    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-tinygltf)

    target_compile_definitions(
        vclib-3rd-tinygltf
        INTERFACE VCLIB_WITH_JSON VCLIB_WITH_STB VCLIB_WITH_TINYGLTF
    )
endif()
