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

find_package(glfw3 3 QUIET)

if (VCLIB_ALLOW_SYSTEM_GLFW AND glfw3_FOUND)
    message(STATUS "- GLFW - using system-provided library")

    add_library(vclib-3rd-glfw INTERFACE)
    target_link_libraries(vclib-3rd-glfw INTERFACE glfw)

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-glfw)

elseif(VCLIB_ALLOW_DOWNLOAD_GLFW)
    message(STATUS "- GLFW - using downloaded source")

    if(LINUX)
        if (VCLIB_RENDER_WITH_WAYLAND)
            set(GLFW_BUILD_WAYLAND ON)
            set(GLFW_BUILD_X11 OFF)
        else()
            set(GLFW_BUILD_WAYLAND OFF)
            set(GLFW_BUILD_X11 ON)
        endif()
    endif()

    FetchContent_Declare(glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4)

    FetchContent_MakeAvailable(glfw3)

    add_library(vclib-3rd-glfw INTERFACE)
    target_link_libraries(vclib-3rd-glfw INTERFACE glfw)

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-glfw)

else()
    message(STATUS "- GLFW - not found, skipping")
endif()
