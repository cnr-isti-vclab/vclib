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
#* it under the terms of the GNU General Public License as published by      *
#* the Free Software Foundation; either version 3 of the License, or         *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
#* for more details.                                                         *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)
project(vclib-render)

include(vclib/cmake/vclib_common_settings.cmake)

# Add the cmake folder for find_package scripts
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# vclib
add_subdirectory(vclib)

# set the VCLIB_TEST_DIR variable to be used by the examples and tests
if (VCLIB_BUILD_EXAMPLES OR VCLIB_BUILD_TESTS)
    get_target_property(VCLIB_TEST_DIR vclib VCLIB_TEST_DIR)
endif()

# external libraries
message(STATUS "VCLib-Render: external libraries")
add_subdirectory(external)

if (TARGET vclib-external-bgfx)
    get_target_property(VCLIB_BGFX_DIR vclib-external-bgfx BGFX_DIR)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfx_config.cmake)
endif()

set(VCLIB_RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(HEADERS
    include/vclib/gui/desktop_trackball.h
    include/vclib/gui/input.h
    include/vclib/render/camera.h
    include/vclib/render/drawable_object.h
    include/vclib/render/drawable_object_vector.h
    include/vclib/render/generic_drawable_mesh.h
    include/vclib/render/generic_drawable_mesh_shader_program.h
    include/vclib/render/lights.h
    include/vclib/render/matrix.h
    include/vclib/render/mesh_render_buffers.h
    include/vclib/render/mesh_render_settings.h
    include/vclib/render/mesh_render_settings_macros.h
    include/vclib/render/trackball.h
    include/vclib/render/lights/directional_light.h
)

# headers to include only if using OpenGL2
if (TARGET vclib-external-opengl)
    list(APPEND HEADERS
        include/vclib/ext/opengl2/draw_objects2.h
        include/vclib/ext/opengl2/draw_objects3.h
        include/vclib/ext/opengl2/drawable_mesh.h)
endif()

# headers to include only if using bgfx
if (TARGET vclib-external-bgfx)
    list(APPEND HEADERS
        include/vclib/ext/bgfx/canvas.h
        include/vclib/ext/bgfx/drawable_axis.h
        include/vclib/ext/bgfx/drawable_mesh.h
        include/vclib/ext/bgfx/mesh_render_buffers.h
        include/vclib/ext/bgfx/minimal_viewer.h
        include/vclib/ext/bgfx/shader_programs/drawable_axis_shader_program.h
        include/vclib/ext/bgfx/shader_programs/drawable_mesh_shader_program.h
        include/vclib/ext/bgfx/shader_programs/load_program.h
        include/vclib/ext/bgfx/uniforms/camera_uniforms.h
        include/vclib/ext/bgfx/uniforms/directional_light_uniforms.h
        include/vclib/ext/bgfx/uniforms/drawable_axis_uniforms.h
        include/vclib/ext/bgfx/uniforms/drawable_mesh_uniforms.h
        include/vclib/ext/bgfx/uniforms/mesh_render_settings_uniforms.h
        include/vclib/ext/bgfx/uniforms/shader_uniform.h)

    set(SHADERS
        shaders/vclib/ext/bgfx/shaders_common.sh
        shaders/vclib/ext/bgfx/drawable_axis/fs_drawable_axis.sc
        shaders/vclib/ext/bgfx/drawable_axis/varying.def.sc
        shaders/vclib/ext/bgfx/drawable_axis/vs_drawable_axis.sc
        shaders/vclib/ext/bgfx/drawable_axis/uniforms.sh
        shaders/vclib/ext/bgfx/drawable_mesh/fs_drawable_mesh.sc
        shaders/vclib/ext/bgfx/drawable_mesh/varying.def.sc
        shaders/vclib/ext/bgfx/drawable_mesh/vs_drawable_mesh.sc
        shaders/vclib/ext/bgfx/drawable_mesh/uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/camera_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/directional_light_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/drawable_axis_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/drawable_mesh_uniforms.sh
        shaders/vclib/ext/bgfx/uniforms/mesh_render_settings_uniforms.sh
        )
endif()

# headers and sources to include only if using qt
if (TARGET vclib-external-qt)
    # header only sources
    list(APPEND HEADERS
        include/vclib/ext/qt/gui/input.h)

    # sources that must be built
    list(APPEND HEADERS_LIB
        include/vclib/ext/qt/gui/drawable_object_frame.h
        include/vclib/ext/qt/gui/drawable_object_vector_frame.h
        include/vclib/ext/qt/gui/mesh_render_settings_frame.h
        include/vclib/ext/qt/gui/q_clickable_label.h)

    list(APPEND SOURCES_LIB
        src/ext/qt/gui/drawable_object_frame.cpp
        src/ext/qt/gui/drawable_object_vector_frame.cpp
        src/ext/qt/gui/mesh_render_settings_frame.cpp
        src/ext/qt/gui/q_clickable_label.cpp)

    list(APPEND FORMS_LIB
        src/ext/qt/gui/drawable_object_frame.ui
        src/ext/qt/gui/drawable_object_vector_frame.ui
        src/ext/qt/gui/mesh_render_settings_frame.ui)

    # headers and sources to include only if using QGLViewer
    if (TARGET vclib-external-qglviewer)
        # header only sources
        list(APPEND HEADERS
            include/vclib/ext/qglviewer/minimal_viewer_widget.h)

        # sources that must be built
        list(APPEND HEADERS_LIB
            include/vclib/ext/qglviewer/viewer_main_window.h)

        list(APPEND SOURCES_LIB
            src/ext/qglviewer/viewer_main_window.cpp)

        list(APPEND FORMS_LIB
            src/ext/qglviewer/viewer_main_window.ui)
    endif()

    # headers and sources to include only if using bgfx
    if (TARGET vclib-external-bgfx)
        list(APPEND HEADERS_LIB
            include/vclib/ext/qt/bgfx/canvas_widget.h
            include/vclib/ext/qt/bgfx/canvas_window.h
            include/vclib/ext/qt/bgfx/minimal_viewer_widget.h
            include/vclib/ext/qt/bgfx/minimal_viewer_window.h
            include/vclib/ext/qt/bgfx/viewer_main_window.h)

        list(APPEND SOURCES_LIB
            src/ext/qt/bgfx/canvas_window.cpp
            src/ext/qt/bgfx/minimal_viewer_window.cpp
            src/ext/qt/bgfx/viewer_main_window.cpp)

        list(APPEND FORMS_LIB
            src/ext/qt/bgfx/viewer_main_window.ui)
    endif()
endif()

source_group("Forms Files" FILES ${FORMS_LIB})

# vclib-render target - will be used by external projects
# will show all the headers in the IDE
add_library(vclib-render INTERFACE)
target_link_libraries(vclib-render INTERFACE ${VCLIB_RENDER_EXTERNAL_LIBRARIES})
target_include_directories(vclib-render INTERFACE ${VCLIB_RENDER_INCLUDE_DIR})
set_target_properties(vclib-render PROPERTIES
    VCLIB_RENDER_INCLUDE_DIR ${VCLIB_RENDER_INCLUDE_DIR})
set_target_properties(vclib-render PROPERTIES
    VCLIB_RENDER_BGFX_SHADERS "${SHADERS}")

if (TARGET vclib-external-bgfx)
    set_target_properties(vclib-render PROPERTIES
        VCLIB_RENDER_BGFX_SHADER_INCLUDE_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vclib/ext/bgfx")
    target_add_bgfx_shaders(vclib-render ${SHADERS})
endif()

target_sources(vclib-render PRIVATE ${HEADERS})

# vclib-render-lib target
if (WIN32)
    # on windows vclib-render-lib must be static - needed for bgfx
    # TODO - try to make it work also on windows with SHARED...
    set(VLIB_RENDER_LIB_TYPE STATIC)
else()
    set(VLIB_RENDER_LIB_TYPE SHARED)
endif()

add_library(vclib-render-lib ${VLIB_RENDER_LIB_TYPE}
    ${HEADERS_LIB} ${SOURCES_LIB} ${FORMS_LIB})

target_link_libraries(vclib-render-lib PUBLIC
    ${VCLIB_RENDER_EXTERNAL_LIBRARIES} vclib)
target_include_directories(vclib-render-lib PRIVATE ${VCLIB_RENDER_INCLUDE_DIR})

target_link_libraries(vclib-render INTERFACE vclib vclib-render-lib)

if (VCLIB_BUILD_EXAMPLES OR VCLIB_BUILD_TESTS)
    add_subdirectory(test)
endif()
