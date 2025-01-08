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
project(vclib-render-3rdparty)

include(FetchContent)

# Add the cmake folder for find_package scripts
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# 3rdparty libraries may have warnings
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

# bgfx
option(VCLIB_ALLOW_BUNDLED_BGFX "Allow use of bundled bgfx source" ON)
option(VCLIB_ALLOW_SYSTEM_BGFX "Allow use of system-provided bgfx" ON)

# Wayland
option(VCLIB_RENDER_WITH_WAYLAND "Linux only - build vclib-render for wayland" OFF)

# Qt
option(VCLIB_ALLOW_SYSTEM_QT "Allow use of system-provided Qt" ON)

# GLFW
option(VCLIB_ALLOW_SYSTEM_GLFW "Allow use of system-provided GLFW" ON)
option(VCLIB_ALLOW_DOWNLOAD_GLFW "Allow use of downloaded GLFW source" ON)

# ImGui
option(VCLIB_ALLOW_DOWNLOAD_IMGUI "Allow use of downloaded ImGui source" ON)

set(VCLIB_RENDER_3RDPARTY_LIBRARIES "")

# === RENDER ENGINES (and optional libraries binded to them) === #

### bgfx
if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    ### Wayland (optional)
    include(wayland.cmake)

    include(bgfx.cmake)
endif()

### OpenGL
if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
    include(opengl.cmake)
endif()

# === OPTIONAL === #

### Qt
include(qt.cmake)

### GLFW
include(glfw.cmake)

### ImGui
include(imgui.cmake)

set(VCLIB_RENDER_3RDPARTY_LIBRARIES
    ${VCLIB_RENDER_3RDPARTY_LIBRARIES} PARENT_SCOPE)
