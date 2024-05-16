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

if (TARGET vclib-external-bgfx)
    get_target_property(VCLIB_BGFX_DIR vclib-external-bgfx BGFX_DIR)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfx_config.cmake)

set(VCLIB_RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(HEADERS
    include/vclib/gui/desktop_trackball.h
    include/vclib/gui/event_manager_interface.h
    include/vclib/gui/input.h
    include/vclib/gui/native_window_handle.h
    include/vclib/render/camera.h
    include/vclib/render/drawable_object_vector.h
    include/vclib/render/lights.h
    include/vclib/render/matrix.h
    include/vclib/render/mesh_render_data.h
    include/vclib/render/mesh_render_settings.h
    include/vclib/render/mesh_render_settings_macros.h
    include/vclib/render/trackball.h
    include/vclib/render/interfaces/drawable_mesh_i.h
    include/vclib/render/interfaces/drawable_object_i.h
    include/vclib/render/lights/directional_light.h)

set(SOURCES
    src/gui/native_window_handle.cpp
    src/render/drawable_object_vector.cpp
    src/render/mesh_render_settings.cpp)

if (APPLE)
    list (APPEND SOURCES
        src/gui/native_window_handle.mm)
endif()

set(ASSETS
    assets/fonts/droidsans.ttf)

if (TARGET vclib-external-bgfx)
    list(APPEND HEADERS
        include/vclib/ext/bgfx/canvas.h
        include/vclib/ext/bgfx/context.h
        include/vclib/ext/bgfx/drawable_axis.h
        include/vclib/ext/bgfx/drawable_directional_light.h
        include/vclib/ext/bgfx/drawable_mesh.h
        include/vclib/ext/bgfx/drawable_trackball.h
        include/vclib/ext/bgfx/load_program.h
        include/vclib/ext/bgfx/mesh_render_buffers.h
        include/vclib/ext/bgfx/minimal_viewer.h
        include/vclib/ext/bgfx/context/callback.h
        include/vclib/ext/bgfx/context/embedded_shaders.h
        include/vclib/ext/bgfx/context/font_manager.h
        include/vclib/ext/bgfx/context/program_manger.h
        include/vclib/ext/bgfx/context/embedded_shaders/drawable_axis.h
        include/vclib/ext/bgfx/context/embedded_shaders/drawable_directional_light.h
        include/vclib/ext/bgfx/context/embedded_shaders/drawable_mesh.h
        include/vclib/ext/bgfx/context/embedded_shaders/drawable_trackball.h
        include/vclib/ext/bgfx/context/embedded_shaders/embedded_shader.h
        include/vclib/ext/bgfx/context/embedded_shaders/font_basic.h
        include/vclib/ext/bgfx/context/embedded_shaders/font_distance_field.h
        include/vclib/ext/bgfx/context/embedded_shaders/font_distance_field_drop_shadow.h
        include/vclib/ext/bgfx/context/embedded_shaders/font_distance_field_drop_shadow_image.h
        include/vclib/ext/bgfx/context/embedded_shaders/font_distance_field_outline.h
        include/vclib/ext/bgfx/context/embedded_shaders/font_distance_field_outline_drop_shadow_image.h
        include/vclib/ext/bgfx/context/embedded_shaders/font_distance_field_outline_image.h
        include/vclib/ext/bgfx/context/embedded_shaders/font_distance_field_subpixel.h
        include/vclib/ext/bgfx/text/embedded_fonts.h
        include/vclib/ext/bgfx/text/text_manager.h
        include/vclib/ext/bgfx/text/text_view.h
        include/vclib/ext/bgfx/text/embedded_fonts/droid_sans.h
        include/vclib/ext/bgfx/text/embedded_fonts/embedded_font.h
        include/vclib/ext/bgfx/text/font/cube_atlas.h
        include/vclib/ext/bgfx/text/font/font_manager.h
        include/vclib/ext/bgfx/text/font/text_buffer_manager.h
        include/vclib/ext/bgfx/text/font/text_metrics.h
        include/vclib/ext/bgfx/text/font/utf8.h
        include/vclib/ext/bgfx/uniforms/camera_uniforms.h
        include/vclib/ext/bgfx/uniforms/directional_light_uniforms.h
        include/vclib/ext/bgfx/uniforms/drawable_axis_uniforms.h
        include/vclib/ext/bgfx/uniforms/drawable_directional_light_uniforms.h
        include/vclib/ext/bgfx/uniforms/drawable_mesh_uniforms.h
        include/vclib/ext/bgfx/uniforms/drawable_trackball_uniforms.h
        include/vclib/ext/bgfx/uniforms/mesh_render_settings_uniforms.h
        include/vclib/ext/bgfx/uniforms/shader_uniform.h)

    list(APPEND SOURCES
        src/ext/bgfx/canvas.cpp
        src/ext/bgfx/context.cpp
        src/ext/bgfx/drawable_axis.cpp
        src/ext/bgfx/drawable_directional_light.cpp
        src/ext/bgfx/drawable_trackball.cpp
        src/ext/bgfx/load_program.cpp
        src/ext/bgfx/minimal_viewer.cpp
        src/ext/bgfx/context/callback.cpp
        src/ext/bgfx/context/font_manager.cpp
        src/ext/bgfx/context/program_manager.cpp
        src/ext/bgfx/context/embedded_shaders/drawable_axis.cpp
        src/ext/bgfx/context/embedded_shaders/drawable_directional_light.cpp
        src/ext/bgfx/context/embedded_shaders/drawable_mesh.cpp
        src/ext/bgfx/context/embedded_shaders/drawable_trackball.cpp
        src/ext/bgfx/context/embedded_shaders/font_basic.cpp
        src/ext/bgfx/context/embedded_shaders/font_distance_field.cpp
        src/ext/bgfx/context/embedded_shaders/font_distance_field_drop_shadow.cpp
        src/ext/bgfx/context/embedded_shaders/font_distance_field_drop_shadow_image.cpp
        src/ext/bgfx/context/embedded_shaders/font_distance_field_outline.cpp
        src/ext/bgfx/context/embedded_shaders/font_distance_field_outline_drop_shadow_image.cpp
        src/ext/bgfx/context/embedded_shaders/font_distance_field_outline_image.cpp
        src/ext/bgfx/context/embedded_shaders/font_distance_field_subpixel.cpp
        src/ext/bgfx/text/text_manager.cpp
        src/ext/bgfx/text/text_view.cpp
        src/ext/bgfx/text/embedded_fonts/droid_sans.cpp
        src/ext/bgfx/text/font/cube_atlas.cpp
        src/ext/bgfx/text/font/font_manager.cpp
        src/ext/bgfx/text/font/text_buffer_manager.cpp
        src/ext/bgfx/text/font/text_metrics.cpp
        src/ext/bgfx/text/font/utf8.cpp)

    list(APPEND SHADERS
        shaders/vclib/ext/bgfx/shaders_common.sh
        shaders/vclib/ext/bgfx/drawable_axis/fs_drawable_axis.sc
        shaders/vclib/ext/bgfx/drawable_axis/varying.def.sc
        shaders/vclib/ext/bgfx/drawable_axis/vs_drawable_axis.sc
        shaders/vclib/ext/bgfx/drawable_axis/uniforms.sh
        shaders/vclib/ext/bgfx/drawable_directional_light/fs_drawable_directional_light.sc
        shaders/vclib/ext/bgfx/drawable_directional_light/varying.def.sc
        shaders/vclib/ext/bgfx/drawable_directional_light/vs_drawable_directional_light.sc
        shaders/vclib/ext/bgfx/drawable_directional_light/uniforms.sh
        shaders/vclib/ext/bgfx/drawable_mesh/fs_drawable_mesh.sc
        shaders/vclib/ext/bgfx/drawable_mesh/varying.def.sc
        shaders/vclib/ext/bgfx/drawable_mesh/vs_drawable_mesh.sc
        shaders/vclib/ext/bgfx/drawable_mesh/uniforms.sh
        shaders/vclib/ext/bgfx/drawable_trackball/fs_drawable_trackball.sc
        shaders/vclib/ext/bgfx/drawable_trackball/varying.def.sc
        shaders/vclib/ext/bgfx/drawable_trackball/vs_drawable_trackball.sc
        shaders/vclib/ext/bgfx/drawable_trackball/uniforms.sh
        shaders/vclib/ext/bgfx/font/fs_font_basic.sc
        shaders/vclib/ext/bgfx/font/fs_font_distance_field.sc
        shaders/vclib/ext/bgfx/font/fs_font_distance_field_drop_shadow.sc
        shaders/vclib/ext/bgfx/font/fs_font_distance_field_drop_shadow_image.sc
        shaders/vclib/ext/bgfx/font/fs_font_distance_field_outline.sc
        shaders/vclib/ext/bgfx/font/fs_font_distance_field_outline_drop_shadow_image.sc
        shaders/vclib/ext/bgfx/font/fs_font_distance_field_outline_image.sc
        shaders/vclib/ext/bgfx/font/fs_font_distance_field_subpixel.sc
        shaders/vclib/ext/bgfx/font/varying.def.sc
        shaders/vclib/ext/bgfx/font/vs_font_basic.sc
        shaders/vclib/ext/bgfx/font/vs_font_distance_field.sc
        shaders/vclib/ext/bgfx/font/vs_font_distance_field_drop_shadow.sc
        shaders/vclib/ext/bgfx/font/vs_font_distance_field_drop_shadow_image.sc
        shaders/vclib/ext/bgfx/font/vs_font_distance_field_outline.sc
        shaders/vclib/ext/bgfx/font/vs_font_distance_field_outline_drop_shadow_image.sc
        shaders/vclib/ext/bgfx/font/vs_font_distance_field_outline_image.sc
        shaders/vclib/ext/bgfx/font/vs_font_distance_field_subpixel.sc
        shaders/vclib/ext/bgfx/uniforms/camera_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/directional_light_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/drawable_axis_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/drawable_directional_light_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/drawable_mesh_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/drawable_trackball_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/mesh_render_settings_uniforms.sh)
endif()

if (TARGET vclib-external-glfw)
    list(APPEND HEADERS
        include/vclib/ext/glfw/gui/input.h)

    list(APPEND SOURCES
        src/ext/glfw/gui/input.cpp)

    if (TARGET vclib-external-bgfx)
        list(APPEND HEADERS
            include/vclib/ext/glfw/bgfx/canvas_window.h
            include/vclib/ext/glfw/bgfx/minimal_viewer_window.h)

        list(APPEND SOURCES
            src/ext/glfw/bgfx/canvas_window.cpp
            src/ext/glfw/bgfx/minimal_viewer_window.cpp)
    endif()
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
        include/vclib/ext/qt/message_hider.h
        include/vclib/ext/qt/gui/drawable_object_frame.h
        include/vclib/ext/qt/gui/drawable_object_vector_frame.h
        include/vclib/ext/qt/gui/input.h
        include/vclib/ext/qt/gui/mesh_render_settings_frame.h
        include/vclib/ext/qt/gui/q_clickable_label.h
        include/vclib/ext/qt/gui/screen_shot_dialog.h)

    list(APPEND SOURCES
        src/ext/qt/gui/drawable_object_frame.cpp
        src/ext/qt/gui/drawable_object_vector_frame.cpp
        src/ext/qt/gui/input.cpp
        src/ext/qt/gui/mesh_render_settings_frame.cpp
        src/ext/qt/gui/q_clickable_label.cpp
        src/ext/qt/gui/screen_shot_dialog.cpp)

    list(APPEND FORMS
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

    if (TARGET vclib-external-bgfx)
        list(APPEND HEADERS
            include/vclib/ext/qt/bgfx/canvas_widget.h
            include/vclib/ext/qt/bgfx/minimal_viewer_widget.h
            include/vclib/ext/qt/bgfx/viewer_main_window.h)

        list(APPEND SOURCES
            src/ext/qt/bgfx/canvas_widget.cpp
            src/ext/qt/bgfx/minimal_viewer_widget.cpp
            src/ext/qt/bgfx/viewer_main_window.cpp)

        list(APPEND FORMS
            src/ext/qt/bgfx/viewer_main_window.ui)
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

if (TARGET vclib-external-qt)
    set_property(TARGET vclib-render PROPERTY AUTOMOC ON)
    set_property(TARGET vclib-render PROPERTY AUTORCC ON)
    set_property(TARGET vclib-render PROPERTY AUTOUIC ON)
endif()

if (TARGET vclib-external-bgfx)
    set_target_properties(vclib-render PROPERTIES
        VCLIB_RENDER_BGFX_SHADER_INCLUDE_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vclib/ext/bgfx")

    build_bgfx_shaders_to_headers(${SHADERS})
    build_assets_to_headers(${ASSETS})
    target_include_directories(vclib-render PRIVATE
       ${CMAKE_CURRENT_BINARY_DIR}/include)
endif()

target_link_libraries(vclib-render PUBLIC
    ${VCLIB_RENDER_EXTERNAL_LIBRARIES} vclib)
target_include_directories(vclib-render PRIVATE ${VCLIB_RENDER_INCLUDE_DIR})

### Examples

include(cmake/examples.cmake)

if (VCLIB_BUILD_EXAMPLES)
    get_target_property(VCLIB_EXAMPLES_DIR vclib VCLIB_EXAMPLES_DIR)
endif()

if (VCLIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
