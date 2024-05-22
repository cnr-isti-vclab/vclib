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
project(vclib-render)

include(vclib/cmake/vclib_common_settings.cmake)

# Add the cmake folder for find_package scripts
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# vclib
add_subdirectory(vclib)

# external libraries
message(STATUS "VCLib-Render: external libraries")
add_subdirectory(external)

get_target_property(VCLIB_BGFX_DIR vclib-external-bgfx BGFX_DIR)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfx_config.cmake)

set(VCLIB_RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/render/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/render/*.cpp")

if (APPLE)
    file(GLOB_RECURSE SOURCES_MM CONFIGURE_DEPENDS "src/render/*.mm")
    list(APPEND SOURCES ${SOURCES_MM})
endif()

set(ASSETS
    assets/fonts/droidsans.ttf)

file(GLOB_RECURSE SHADERS CONFIGURE_DEPENDS "shaders/vclib/render/*.sh" "shaders/vclib/render/*.sc")

if (TARGET vclib-external-glfw)
    list(APPEND HEADERS
        include/vclib/ext/glfw/canvas_window.h
        include/vclib/ext/glfw/event_manager_window.h
        include/vclib/ext/glfw/input.h
        include/vclib/ext/glfw/minimal_viewer_window.h)

    list(APPEND SOURCES
        src/ext/glfw/canvas_window.cpp
        src/ext/glfw/event_manager_window.cpp
        src/ext/glfw/input.cpp
        src/ext/glfw/minimal_viewer_window.cpp)
endif()

if (TARGET vclib-external-opengl)
    list(APPEND HEADERS
        include/vclib/ext/opengl2/draw_objects2.h
        include/vclib/ext/opengl2/draw_objects3.h
        include/vclib/ext/opengl2/drawable_mesh.h)

    list(APPEND SOURCES
        src/ext/opengl2/draw_objects2.cpp
        src/ext/opengl2/draw_objects3.cpp)
endif()

if (TARGET vclib-external-qt)
    list(APPEND HEADERS
        include/vclib/ext/qt/canvas_widget.h
        include/vclib/ext/qt/event_manager_widget.h
        include/vclib/ext/qt/input.h
        include/vclib/ext/qt/message_hider.h
        include/vclib/ext/qt/minimal_viewer_widget.h
        include/vclib/ext/qt/viewer_main_window.h
        include/vclib/ext/qt/gui/drawable_object_frame.h
        include/vclib/ext/qt/gui/drawable_object_vector_frame.h
        include/vclib/ext/qt/gui/mesh_render_settings_frame.h
        include/vclib/ext/qt/gui/q_clickable_label.h
        include/vclib/ext/qt/gui/screen_shot_dialog.h)

    list(APPEND SOURCES
        src/ext/qt/canvas_widget.cpp
        src/ext/qt/event_manager_widget.cpp
        src/ext/qt/input.cpp
        src/ext/qt/minimal_viewer_widget.cpp
        src/ext/qt/viewer_main_window.cpp
        src/ext/qt/gui/drawable_object_frame.cpp
        src/ext/qt/gui/drawable_object_vector_frame.cpp
        src/ext/qt/gui/mesh_render_settings_frame.cpp
        src/ext/qt/gui/q_clickable_label.cpp
        src/ext/qt/gui/screen_shot_dialog.cpp)

    list(APPEND FORMS
        src/ext/qt/viewer_main_window.ui
        src/ext/qt/gui/drawable_object_frame.ui
        src/ext/qt/gui/drawable_object_vector_frame.ui
        src/ext/qt/gui/mesh_render_settings_frame.ui)

    if (TARGET vclib-external-qglviewer)
        list(APPEND HEADERS
            include/vclib/ext/qglviewer/minimal_viewer_widget.h
            include/vclib/ext/qglviewer/viewer_main_window.h)

        list(APPEND SOURCES
            src/ext/qglviewer/minimal_viewer_widget.cpp
            src/ext/qglviewer/viewer_main_window.cpp)

        list(APPEND FORMS
            src/ext/qglviewer/viewer_main_window.ui)
    endif()
endif()

source_group("Forms Files" FILES ${FORMS})

# vclib-render target
if (WIN32)
    # on windows vclib-render must be static - needed for bgfx
    # TODO - try to make it work also on windows with SHARED...
    set(VLIB_RENDER_LIB_TYPE STATIC)
else()
    set(VLIB_RENDER_LIB_TYPE SHARED)
endif()

add_library(vclib-render ${VLIB_RENDER_LIB_TYPE}
    ${HEADERS} ${SOURCES} ${SHADERS} ${FORMS})

target_include_directories(vclib-render INTERFACE ${VCLIB_RENDER_INCLUDE_DIR})
set_target_properties(vclib-render PROPERTIES
    VCLIB_RENDER_INCLUDE_DIR ${VCLIB_RENDER_INCLUDE_DIR})

set_target_properties(vclib-render PROPERTIES
    VCLIB_RENDER_BGFX_SHADER_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

build_bgfx_shaders_to_headers(${SHADERS})
build_assets_to_headers(${ASSETS})
target_include_directories(vclib-render PRIVATE
   ${CMAKE_CURRENT_BINARY_DIR}/include)

if (TARGET vclib-external-qt)
    set_property(TARGET vclib-render PROPERTY AUTOMOC ON)
    set_property(TARGET vclib-render PROPERTY AUTORCC ON)
    set_property(TARGET vclib-render PROPERTY AUTOUIC ON)
endif()

target_link_libraries(vclib-render PUBLIC
    ${VCLIB_RENDER_EXTERNAL_LIBRARIES} vclib)
target_include_directories(vclib-render PRIVATE ${VCLIB_RENDER_INCLUDE_DIR})

### Examples

if (VCLIB_BUILD_EXAMPLES)
    include(cmake/examples.cmake)
    get_target_property(VCLIB_EXAMPLES_DIR vclib VCLIB_EXAMPLES_DIR)
    add_subdirectory(examples)
endif()
