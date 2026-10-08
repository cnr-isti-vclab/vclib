# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if(VCLIB_ALLOW_DOWNLOAD_TINYGLTF)
    message(STATUS "- tinygltf - using downloaded source")

    set(TINYGLTF_VERSION 3.0.0)

    set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF CACHE BOOL "" FORCE)

    if(NOT ${VCLIB_ALLOW_INSTALL_TINYGLTF})
        set(TINYGLTF_INSTALL OFF CACHE BOOL "" FORCE)
    endif()

    set(TINYGLTF_INSTALL_VENDOR OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        tinygltf
        GIT_REPOSITORY https://github.com/syoyo/tinygltf
        GIT_TAG v${TINYGLTF_VERSION}
    )
    vclib_begin_3rdparty_install_scope()

    FetchContent_MakeAvailable(tinygltf)

    vclib_end_3rdparty_install_scope()

    if(VCLIB_ALLOW_INSTALL_TINYGLTF)
        install(
            CODE
                "
            if(EXISTS \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/include/tiny_gltf.h\")
                file(MAKE_DIRECTORY \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${VCLIB_3RDPARTY_INSTALL_INCLUDEDIR}\")
                file(RENAME
                    \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/include/tiny_gltf.h\"
                    \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${VCLIB_3RDPARTY_INSTALL_INCLUDEDIR}/tiny_gltf.h\"
                )
            endif()
            if(EXISTS \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/include/tinygltf_json.h\")
                file(MAKE_DIRECTORY \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${VCLIB_3RDPARTY_INSTALL_INCLUDEDIR}\")
                file(RENAME
                    \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/include/tinygltf_json.h\"
                    \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${VCLIB_3RDPARTY_INSTALL_INCLUDEDIR}/tinygltf_json.h\"
                )
            endif()
        "
        )
    endif()

    add_library(vclib-3rd-tinygltf INTERFACE)
    target_link_libraries(
        vclib-3rd-tinygltf
        INTERFACE tinygltf vclib-3rd-nlohmann_json vclib-3rd-stb
    )

    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-tinygltf)

    target_compile_definitions(
        vclib-3rd-tinygltf
        INTERFACE VCLIB_WITH_TINYGLTF TINYGLTF_NO_INCLUDE_JSON
    )
else()
    if(VCLIB_BUILD_MODULE_RENDER)
        message(
            FATAL_ERROR
            "tinygltf is required by the render module - VCLIB_ALLOW_DOWNLOAD_TINYGLTF must be enabled."
        )
    endif()
endif()
