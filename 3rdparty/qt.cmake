# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(QT_MINIMUM_VERSION 6.7)

# on linux, avoid extra dependency to libOpengl.so, linked by Qt::Gui module
set(OpenGL_GL_PREFERENCE LEGACY)

set(QT_COMPONENTS Core Gui)
if(VCLIB_BUILD_MODULE_RENDER)
    list(APPEND QT_COMPONENTS Xml Widgets)
    if(VCLIB_RENDER_BACKEND STREQUAL "opengl2")
        list(APPEND QT_COMPONENTS OpenGL OpenGLWidgets)
    endif()
endif()

if(VCLIB_REQUIRES_QT)
    set(VCLIB_QT_REQUIRED "REQUIRED")
else()
    set(VCLIB_QT_REQUIRED "")
endif()

find_package(
    Qt6
    ${QT_MINIMUM_VERSION}
    COMPONENTS ${QT_COMPONENTS}
    QUIET
    ${VCLIB_QT_REQUIRED}
)

if(VCLIB_ALLOW_SYSTEM_QT)
    if(Qt6_FOUND)
        message(STATUS "- Qt6 - using system-provided library")

        if(NOT TARGET vclib-3rd-qt)
            add_library(vclib-3rd-qt INTERFACE)
        endif()
        target_compile_definitions(vclib-3rd-qt INTERFACE VCLIB_WITH_QT)

        target_link_libraries(vclib-3rd-qt INTERFACE Qt6::Core Qt6::Gui)
        if(VCLIB_BUILD_MODULE_RENDER)
            target_link_libraries(vclib-3rd-qt INTERFACE Qt6::Widgets Qt6::Xml)
            if(VCLIB_RENDER_BACKEND STREQUAL "opengl2")
                target_link_libraries(
                    vclib-3rd-qt
                    INTERFACE Qt6::OpenGL Qt6::OpenGLWidgets
                )
            endif()
        endif()

        list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-qt)
    else()
        message(STATUS "- Qt - not found, skipping")
    endif()
endif()
