# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

find_package(glfw3 3 QUIET)

if(VCLIB_ALLOW_SYSTEM_GLFW AND glfw3_FOUND)
    message(STATUS "- GLFW - using system-provided library")

    set(VCLIB_USES_GLFW TRUE)

elseif(VCLIB_ALLOW_DOWNLOAD_GLFW)
    message(STATUS "- GLFW - using downloaded source")

    if(LINUX)
        if(VCLIB_RENDER_WITH_WAYLAND)
            set(GLFW_BUILD_WAYLAND ON)
            set(GLFW_BUILD_X11 OFF)
        else()
            set(GLFW_BUILD_WAYLAND OFF)
            set(GLFW_BUILD_X11 ON)
        endif()
    endif()

    FetchContent_Declare(
        glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(glfw3)

    set(VCLIB_USES_GLFW TRUE)
endif()

if(VCLIB_USES_GLFW)
    add_library(vclib-3rd-glfw INTERFACE)
    target_link_libraries(vclib-3rd-glfw INTERFACE glfw)

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-glfw)

    target_compile_definitions(vclib-3rd-glfw INTERFACE VCLIB_WITH_GLFW)
else()
    message(STATUS "- GLFW - not found, skipping")
endif()
