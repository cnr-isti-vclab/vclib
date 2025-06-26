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

if (VCLIB_ALLOW_DOWNLOAD_IMGUI)
    message(STATUS "- ImGui - using downloaded source")

    set(IMGUI_VERSION 1.91.9b)

    # ImGui (glfw + bgfx)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v${IMGUI_VERSION}
        EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(imgui)

    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h)
    add_library(imgui
        ${IMGUI_SOURCES}
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp # TODO: std::string? check
    )
    if (TARGET vclib-3rd-glfw)
        target_sources(imgui PRIVATE
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h)
        target_link_libraries(imgui PRIVATE vclib-3rd-glfw)
    endif()

    if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
        target_sources(imgui PRIVATE
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.h
        )
    endif()

    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/misc/cpp
        ${imgui_SOURCE_DIR}/backends
    )
    # this is the way to include the custom imconfig.h
    # target_compile_definitions(imgui PRIVATE IMGUI_USER_CONFIG="<vclib/bgfx/imgui/imconfig.h>")

    # add -fPIC flag to imgui target
    set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)

    add_library(vclib-3rd-imgui INTERFACE)
    target_link_libraries(vclib-3rd-imgui INTERFACE imgui)

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-imgui)

    target_compile_definitions(vclib-3rd-imgui INTERFACE
        VCLIB_WITH_IMGUI)
endif()
