# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.24)
project(vclib-3rdparty)

include(FetchContent)

# 3rdparty libraries may have warnings
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

# Add the cmake folder for find_package scripts
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# === REQUIRED === #

# Optional system libraries used by VCLib modules.
# These lists track system dependencies (like Qt) so they can be
# exported to vclibConfig.cmake and properly re-linked by find_dependency().
set(VCLIB_CORE_3RDPARTY_LIBRARIES "")
set(VCLIB_CORE_OPTIONAL_SYSTEM_LIBRARIES "")

set(VCLIB_EXTERNAL_3RDPARTY_LIBRARIES "")
set(VCLIB_EXTERNAL_OPTIONAL_SYSTEM_LIBRARIES "")

set(VCLIB_RENDER_3RDPARTY_LIBRARIES "")
set(VCLIB_RENDER_OPTIONAL_SYSTEM_LIBRARIES "")

# Eigen
option(VCLIB_ALLOW_BUNDLED_EIGEN "Allow use of bundled Eigen source" ON)
option(VCLIB_ALLOW_SYSTEM_EIGEN "Allow use of system-provided Eigen" ON)
option(VCLIB_ALLOW_INSTALL_EIGEN "Allow install Eigen source" ON)
include(eigen.cmake)

# MapBox earcut
option(
    VCLIB_ALLOW_BUNDLED_MAPBOX_EARCUT
    "Allow use of bundled Mapbox-Eaurcut source"
    ON
)
option(
    VCLIB_ALLOW_INSTALL_MAPBOX_EARCUT
    "Allow install Mapbox-Eaurcut source"
    ON
)
include(mapbox.cmake)

# poolSTL (not required if C++17 parallel algorithms are available) and TBB
option(VCLIB_ALLOW_BUNDLED_POOLSTL "Allow use of bundled poolSTL source" ON)
option(VCLIB_ALLOW_INSTALL_POOLSTL "Allow install poolSTL source" ON)
option(VCLIB_ALLOW_SYSTEM_TBB "Allow use of system-provided TBB" ON)
include(poolstl_tbb.cmake)

# Zip iterator
option(VCLIB_ALLOW_BUNDLED_ZIP_VIEWS "Allow use of bundled ZipViews source" ON)
option(VCLIB_ALLOW_INSTALL_ZIP_VIEWS "Allow install ZipViews source" ON)
include(zip-views.cmake)

# === OPTIONAL === #

# IO or RENDER 3rd Party libraries
if(VCLIB_BUILD_CORE_WITH_3RDPARTY_IO_LIBRARIES OR VCLIB_BUILD_MODULE_RENDER)
    # Qt
    option(VCLIB_ALLOW_SYSTEM_QT "Allow use of system-provided Qt" ON)
    include(qt.cmake)
endif()

# IO - 3rd Party libraries
if(VCLIB_BUILD_CORE_WITH_3RDPARTY_IO_LIBRARIES)
    # tinygltf
    option(
        VCLIB_ALLOW_DOWNLOAD_TINYGTLF
        "Allow use of downloaded tinygltf source
        (brings stb and nlohmann-json)"
        ON
    )
    option(VCLIB_ALLOW_INSTALL_TINYGTLF "Allow install tinygltf source" ON)
    include(tinygltf.cmake)
endif()

if(VCLIB_BUILD_MODULE_EXTERNAL)
    # boost
    option(VCLIB_ALLOW_SYSTEM_BOOST "Allow use of system-provided Boost" ON)
    option(VCLIB_ALLOW_DOWNLOAD_BOOST "Allow use of downloaded Boost source" ON)
    option(VCLIB_ALLOW_INSTALL_BOOST "Allow install Boost source" ON)
    include(boost.cmake)

    # CGAL
    option(VCLIB_ALLOW_DOWNLOAD_CGAL "Allow use of downloaded CGAL source" ON)
    option(VCLIB_ALLOW_SYSTEM_CGAL "Allow use of system-provided CGAL" ON)
    option(VCLIB_ALLOW_INSTALL_CGAL "Allow install CGAL source" ON)

    include(cgal.cmake)

    # embree
    option(VCLIB_ALLOW_SYSTEM_EMBREE "Allow use of system-provided Embree" ON)
    option(
        VCLIB_ALLOW_DOWNLOAD_EMBREE
        "Allow download and use of embree (Windows only)"
        ON
    )
    include(embree.cmake)

    # libigl
    option(VCLIB_ALLOW_SYSTEM_LIBIGL "Allow use of system-provided libigl" ON)
    option(
        VCLIB_ALLOW_DOWNLOAD_LIBIGL
        "Allow use of downloaded libigl source"
        ON
    )
    option(VCLIB_ALLOW_INSTALL_LIBIGL "Allow install libigl source" ON)
    include(libigl.cmake)

    # PoissonRecon
    option(
        VCLIB_ALLOW_DOWNLOAD_POISSON_RECON
        "Allow use of downloaded PoissonRecon source"
        ON
    )
    option(
        VCLIB_ALLOW_INSTALL_POISSON_RECON
        "Allow install PoissonRecon source"
        ON
    )
    include(poisson-recon.cmake)

    # VCG
    option(VCLIB_ALLOW_DOWNLOAD_VCG "Allow use of downloaded VCG source" ON)
    option(VCLIB_ALLOW_SYSTEM_VCG "Allow use of system-provided VCG" ON)
    option(VCLIB_ALLOW_INSTALL_VCG "Allow install VCG source" ON)
    include(vcg.cmake)
endif()

if(VCLIB_BUILD_MODULE_RENDER)
    ### bgfx
    if(VCLIB_RENDER_BACKEND STREQUAL "bgfx")
        # Wayland (optional)
        option(
            VCLIB_RENDER_WITH_WAYLAND
            "Linux only - build vclib-render for wayland"
            OFF
        )
        include(wayland.cmake)

        # bgfx
        option(VCLIB_ALLOW_SYSTEM_BGFX "Allow use of system-provided bgfx" ON)
        option(
            VCLIB_ALLOW_DOWNLOAD_BGFX
            "Allow use of downloaded BGFX source"
            ON
        )
        option(VCLIB_ALLOW_INSTALL_BGFX "Allow install bgfx source" ON)
        include(bgfx.cmake)
    endif()

    ### OpenGL
    if(VCLIB_RENDER_BACKEND STREQUAL "opengl2")
        include(opengl.cmake)
    endif()

    ### GLFW
    option(VCLIB_ALLOW_SYSTEM_GLFW "Allow use of system-provided GLFW" ON)
    option(VCLIB_ALLOW_DOWNLOAD_GLFW "Allow use of downloaded GLFW source" ON)
    option(VCLIB_ALLOW_INSTALL_GLFW "Allow install GLFW source" ON)
    include(glfw.cmake)

    ### ImGui
    option(VCLIB_ALLOW_DOWNLOAD_IMGUI "Allow use of downloaded ImGui source" ON)
    option(VCLIB_ALLOW_INSTALL_IMGUI "Allow install ImGui source" ON)
    include(imgui.cmake)
endif()

if(VCLIB_BUILD_MODULE_BINDINGS)
    # pybind11
    option(
        VCLIB_ALLOW_DOWNLOAD_PYBIND
        "Allow use of downloaded pybind11 source"
        ON
    )
    include(pybind.cmake)
endif()

set(ALL_3RDPARTY_TARGETS "")
if(VCLIB_INSTALL_MODULE_CORE)
    list(APPEND ALL_3RDPARTY_TARGETS ${VCLIB_CORE_3RDPARTY_LIBRARIES})
endif()
if(VCLIB_INSTALL_MODULE_EXTERNAL)
    list(APPEND ALL_3RDPARTY_TARGETS ${VCLIB_EXTERNAL_3RDPARTY_LIBRARIES})
endif()
if(VCLIB_INSTALL_MODULE_RENDER)
    list(APPEND ALL_3RDPARTY_TARGETS ${VCLIB_RENDER_3RDPARTY_LIBRARIES})
endif()

set(EXPORT_QUEUE ${ALL_3RDPARTY_TARGETS})
set(EXPORTED_TARGETS "")

# Iterate through the dependency graph of all 3rdparty targets to export them.
# We use a queue to traverse the dependencies (INTERFACE_LINK_LIBRARIES and LINK_LIBRARIES).
while(EXPORT_QUEUE)
    list(POP_FRONT EXPORT_QUEUE tgt)
    if(NOT TARGET ${tgt})
        continue()
    endif()

    get_target_property(aliased ${tgt} ALIASED_TARGET)
    if(aliased)
        set(tgt ${aliased})
    endif()

    if(tgt IN_LIST EXPORTED_TARGETS)
        continue()
    endif()

    get_target_property(is_imported ${tgt} IMPORTED)
    if(is_imported)
        continue()
    endif()

    list(APPEND EXPORTED_TARGETS ${tgt})

    get_target_property(linked_libs ${tgt} INTERFACE_LINK_LIBRARIES)
    if(linked_libs)
        foreach(lib IN LISTS linked_libs)
            if(lib MATCHES "\\$<LINK_ONLY:(.+)>")
                set(lib "${CMAKE_MATCH_1}")
            endif()
            if(TARGET ${lib})
                list(APPEND EXPORT_QUEUE ${lib})
            endif()
        endforeach()
    endif()

    get_target_property(type ${tgt} TYPE)
    if(NOT type STREQUAL "INTERFACE_LIBRARY")
        get_target_property(linked_libs ${tgt} LINK_LIBRARIES)
        if(linked_libs)
            foreach(lib IN LISTS linked_libs)
                if(lib MATCHES "\\$<LINK_ONLY:(.+)>")
                    set(lib "${CMAKE_MATCH_1}")
                endif()
                if(TARGET ${lib})
                    list(APPEND EXPORT_QUEUE ${lib})
                endif()
            endforeach()
        endif()
    endif()
endwhile()

foreach(tgt IN LISTS EXPORTED_TARGETS)
    get_target_property(inc_dirs ${tgt} INTERFACE_INCLUDE_DIRECTORIES)
    if(inc_dirs)
        set(new_inc_dirs "")
        set(added_install_interface FALSE)
        foreach(inc_dir IN LISTS inc_dirs)
            if(inc_dir MATCHES "\\$<")
                list(APPEND new_inc_dirs "${inc_dir}")
            else()
                list(APPEND new_inc_dirs "$<BUILD_INTERFACE:${inc_dir}>")
                if(NOT added_install_interface)
                    list(
                        APPEND new_inc_dirs
                        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                    )
                    set(added_install_interface TRUE)
                endif()
            endif()
        endforeach()
        set_target_properties(
            ${tgt}
            PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${new_inc_dirs}"
        )
    endif()

    install(
        TARGETS ${tgt}
        EXPORT vclibTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endforeach()

set(VCLIB_CORE_3RDPARTY_LIBRARIES ${VCLIB_CORE_3RDPARTY_LIBRARIES} PARENT_SCOPE)
set(VCLIB_CORE_OPTIONAL_SYSTEM_LIBRARIES
    ${VCLIB_CORE_OPTIONAL_SYSTEM_LIBRARIES}
    PARENT_SCOPE
)

set(VCLIB_EXTERNAL_3RDPARTY_LIBRARIES
    ${VCLIB_EXTERNAL_3RDPARTY_LIBRARIES}
    PARENT_SCOPE
)
set(VCLIB_EXTERNAL_OPTIONAL_SYSTEM_LIBRARIES
    ${VCLIB_EXTERNAL_OPTIONAL_SYSTEM_LIBRARIES}
    PARENT_SCOPE
)

set(VCLIB_RENDER_3RDPARTY_LIBRARIES
    ${VCLIB_RENDER_3RDPARTY_LIBRARIES}
    PARENT_SCOPE
)
set(VCLIB_RENDER_OPTIONAL_SYSTEM_LIBRARIES
    ${VCLIB_RENDER_OPTIONAL_SYSTEM_LIBRARIES}
    PARENT_SCOPE
)
