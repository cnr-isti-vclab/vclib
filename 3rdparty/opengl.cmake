# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

find_package(OpenGL QUIET)

if (OpenGL_FOUND)
    message(STATUS "- OpenGL - using system-provided library")

    add_library(vclib-3rd-opengl INTERFACE)
    target_link_libraries(vclib-3rd-opengl INTERFACE
        OpenGL::GL OpenGL::GLU)

    if(APPLE)
        target_compile_definitions(vclib-3rd-opengl
            INTERFACE GL_SILENCE_DEPRECATION)
    endif()

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-opengl)
else()
    message(FATAL ERROR "- OpenGL is required - system-provided library not found.")
endif()
