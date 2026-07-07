# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

if(VCLIB_ALLOW_DOWNLOAD_IMGUI)
    message(STATUS "- ImGui - using downloaded source")

    set(IMGUI_VERSION 1.92.0)

    set(IMGUI_EXCLUDE_FROM_ALL_OPTION "")
    if(NOT VCLIB_ALLOW_INSTALL_IMGUI)
        set(IMGUI_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
    endif()

    # ImGui (glfw + bgfx)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v${IMGUI_VERSION}
        SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/imgui-src/imgui
        ${IMGUI_EXCLUDE_FROM_ALL_OPTION}
    )
    FetchContent_MakeAvailable(imgui)

    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h)
    add_library(
        imgui
        ${IMGUI_SOURCES}
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp # TODO: std::string? check
    )
    if(TARGET vclib-3rd-glfw)
        target_sources(
            imgui
            PRIVATE
                ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
        )
        target_link_libraries(imgui PRIVATE vclib-3rd-glfw)
    endif()

    if(VCLIB_RENDER_BACKEND STREQUAL "opengl2")
        target_sources(
            imgui
            PRIVATE
                ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.h
        )
    endif()

    target_include_directories(
        imgui
        PUBLIC
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
    target_include_directories(
        vclib-3rd-imgui
        INTERFACE
            $<BUILD_INTERFACE:${imgui_SOURCE_DIR}/..>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/imgui>
    )

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-imgui)

    target_compile_definitions(vclib-3rd-imgui INTERFACE VCLIB_WITH_IMGUI)

    if(VCLIB_ALLOW_INSTALL_IMGUI)
        install(
            FILES
                ${imgui_SOURCE_DIR}/imgui.h
                ${imgui_SOURCE_DIR}/imgui_internal.h
                ${imgui_SOURCE_DIR}/imstb_rectpack.h
                ${imgui_SOURCE_DIR}/imstb_textedit.h
                ${imgui_SOURCE_DIR}/imstb_truetype.h
                ${imgui_SOURCE_DIR}/imconfig.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imgui
        )
        install(
            DIRECTORY ${imgui_SOURCE_DIR}/backends/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imgui/backends
            FILES_MATCHING
            PATTERN "imgui_impl_*.h"
        )
        install(
            DIRECTORY ${imgui_SOURCE_DIR}/misc/cpp/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imgui/misc/cpp
            FILES_MATCHING
            PATTERN "*.h"
        )
    endif()
endif()
