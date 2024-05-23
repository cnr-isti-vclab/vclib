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

cmake_minimum_required(VERSION 3.24)
project(vclib-render-external)

include(FetchContent)

# external libraries may have warnings
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

option(VCLIB_RENDER_WITH_WAYLAND "Linux only - build vclib-render for wayland" OFF)

# Qt
option(VCLIB_ALLOW_SYSTEM_QT "Allow use of system-provided Qt" ON)

# GLFW
option(VCLIB_ALLOW_SYSTEM_GLFW "Allow use of system-provided GLFW" ON)
option(VCLIB_ALLOW_DOWNLOAD_GLFW "Allow use of downloaded GLFW source" ON)

# bgfx
option(VCLIB_ALLOW_BUNDLED_BGFX "Allow use of bundled bgfx source" ON)

# QGLViewer
option(VCLIB_ALLOW_SYSTEM_QGLVIEWER "Allow use of system-provided QGLViewer" ON)

set(VCLIB_RENDER_EXTERNAL_LIBRARIES "")

# === REQUIRED RENDER ENGINES === #

### bgfx
if (VCLIB_RENDER_ENGINE STREQUAL "bgfx")
    include(bgfx.cmake)
endif()

# === OPTIONAL === #

### Wayland
include(wayland.cmake)

### OpenGL
include(opengl.cmake)

### Qt
include(qt.cmake)

### GLFW
include(glfw.cmake)

### QGLViewer
include(qglviewer.cmake)

set(VCLIB_RENDER_EXTERNAL_LIBRARIES
    ${VCLIB_RENDER_EXTERNAL_LIBRARIES} PARENT_SCOPE)
