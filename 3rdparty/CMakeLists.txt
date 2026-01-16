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

cmake_minimum_required(VERSION 3.24)
project(vclib-3rdparty)

include(FetchContent)

# 3rdparty libraries may have warnings
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

# Add the cmake folder for find_package scripts
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# === REQUIRED === #

set(VCLIB_CORE_3RDPARTY_LIBRARIES "")
set(VCLIB_EXTERNAL_3RDPARTY_LIBRARIES "")
set(VCLIB_RENDER_3RDPARTY_LIBRARIES "")

# Eigen
option(VCLIB_ALLOW_BUNDLED_EIGEN "Allow use of bundled Eigen source" ON)
option(VCLIB_ALLOW_SYSTEM_EIGEN "Allow use of system-provided Eigen" ON)
include(eigen.cmake)

# MapBox earcut
option(VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT
    "Allow use of bundled Mapbox-Eaurcut source" ON)
option(VCLIB_ALLOW_INSTALL_MAPBOX_EARCUT
    "Allow install Mapbox-Eaurcut source" ON)
include(mapbox.cmake)

# poolSTL (not required if C++17 parallel algorithms are available) and TBB
option(VCLIB_ALLOW_BUNDLED_POOLSTL "Allow use of bundled poolSTL source" ON)
option(VCLIB_ALLOW_INSTALL_POOLSTL "Allow install poolSTL source" ON)
option(VCLIB_ALLOW_SYSTEM_TBB "Allow use of system-provided TBB" ON)
include(poolstl_tbb.cmake)

# Zip iterator
option(VCLIB_ALLOW_BUNDLED_ZIP_VIEWS
    "Allow use of bundled ZipViews source" ON)
option(VCLIB_ALLOW_INSTALL_ZIP_VIEWS
    "Allow install ZipViews source" ON)
include(zip-views.cmake)

# === OPTIONAL === #

# IO or RENDER 3rd Party libraries
if (VCLIB_BUILD_CORE_WITH_3RDPARTY_IO_LIBRARIES OR VCLIB_BUILD_MODULE_RENDER)
    # Qt
    option(VCLIB_ALLOW_SYSTEM_QT "Allow use of system-provided Qt" ON)
    include(qt.cmake)
endif()

# IO - 3rd Party libraries
if (VCLIB_BUILD_CORE_WITH_3RDPARTY_IO_LIBRARIES)
    # tinygltf
    option(VCLIB_ALLOW_DOWNLOAD_TINYGTLF
        "Allow use of downloaded tinygltf source
        (brings stb and nlohmann-json)" ON)
    option(VCLIB_ALLOW_INSTALL_TINYGTLF "Allow install tinygltf source" ON)
    include(tinygltf.cmake)
endif()

if (VCLIB_BUILD_MODULE_EXTERNAL)
    # embree
    option(VCLIB_ALLOW_SYSTEM_EMBREE
        "Allow use of system-provided Embree" ON)
    include(embree.cmake)

    # VCG
    option(VCLIB_ALLOW_DOWNLOAD_VCG "Allow use of downloaded VCG source" ON)
    option(VCLIB_ALLOW_SYSTEM_VCG "Allow use of system-provided VCG" ON)
    include(vcg.cmake)
endif()

if (VCLIB_BUILD_MODULE_RENDER)
    ### bgfx
    if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
        # Wayland (optional)
        option(VCLIB_RENDER_WITH_WAYLAND
            "Linux only - build vclib-render for wayland" OFF)
        include(wayland.cmake)

        # bgfx
        option(VCLIB_ALLOW_SYSTEM_BGFX
            "Allow use of system-provided bgfx" ON)
        option(VCLIB_ALLOW_DOWNLOAD_BGFX
            "Allow use of downloaded BGFX source" ON)
        include(bgfx.cmake)
    endif()

    ### OpenGL
    if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
        include(opengl.cmake)
    endif()

    ### GLFW
    option(VCLIB_ALLOW_SYSTEM_GLFW "Allow use of system-provided GLFW" ON)
    option(VCLIB_ALLOW_DOWNLOAD_GLFW "Allow use of downloaded GLFW source" ON)
    include(glfw.cmake)

    ### ImGui
    option(VCLIB_ALLOW_DOWNLOAD_IMGUI "Allow use of downloaded ImGui source" ON)
    include(imgui.cmake)
endif()

if (VCLIB_BUILD_MODULE_BINDINGS)
    # pybind11
    option(VCLIB_ALLOW_DOWNLOAD_PYBIND
        "Allow use of downloaded pybind11 source" ON)
    include(pybind.cmake)
endif()

set(VCLIB_CORE_3RDPARTY_LIBRARIES
    ${VCLIB_CORE_3RDPARTY_LIBRARIES} PARENT_SCOPE)
set(VCLIB_EXTERNAL_3RDPARTY_LIBRARIES
    ${VCLIB_EXTERNAL_3RDPARTY_LIBRARIES} PARENT_SCOPE)
set(VCLIB_RENDER_3RDPARTY_LIBRARIES
    ${VCLIB_RENDER_3RDPARTY_LIBRARIES} PARENT_SCOPE)
