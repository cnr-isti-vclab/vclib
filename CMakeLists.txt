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

# Options
set(VCLIB_RENDER_ENGINE "bgfx" CACHE STRING "Select the rendering engine")
set_property(CACHE VCLIB_RENDER_ENGINE PROPERTY STRINGS bgfx)

# vclib
add_subdirectory(vclib)

# external libraries
message(STATUS "VCLib-Render: external libraries")
add_subdirectory(external)

set(VCLIB_RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/render/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/vclib/render/*.cpp")

if (APPLE)
    file(GLOB_RECURSE SOURCES_MM CONFIGURE_DEPENDS "src/vclib/render/*.mm")
    list(APPEND SOURCES ${SOURCES_MM})
endif()

file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "assets/*")

# if render engine is bgfx - include sources that use bgfx
if (VCLIB_RENDER_ENGINE STREQUAL "bgfx")
    get_target_property(VCLIB_BGFX_DIR vclib-external-bgfx BGFX_DIR)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfx_config.cmake)

    file(GLOB_RECURSE HEADERS_BGFX CONFIGURE_DEPENDS
        "include/vclib_bgfx/render/*.h")
    list(APPEND HEADERS ${HEADERS_BGFX})

    file(GLOB_RECURSE SOURCES_BGFX CONFIGURE_DEPENDS
        "src/vclib_bgfx/render/*.cpp")
    list(APPEND SOURCES ${SOURCES_BGFX})

    file(GLOB_RECURSE SHADERS_BGFX CONFIGURE_DEPENDS
        "shaders/vclib_bgfx/render/*.sh" "shaders/vclib_bgfx/render/*.sc")
    list(APPEND SHADERS ${SHADERS_BGFX})

    # required files that use bgfx
    set(LINK_FILES
        include/vclib_bgfx/render/canvas.h
        include/vclib_bgfx/render/viewer_canvas.h
        include/vclib_bgfx/render/drawable/drawable_mesh.h)

    foreach(FILE ${LINK_FILES})
        string(REPLACE "vclib_bgfx" "vclib" TARGET_FILE ${FILE})

        get_filename_component(TARGET_DIR ${TARGET_FILE} DIRECTORY)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR})

        file(CREATE_LINK
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
            ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_FILE})
    endforeach()
endif()

if (TARGET vclib-external-glfw)
    file(GLOB_RECURSE HEADERS_GLFW CONFIGURE_DEPENDS
        "include/vclib/ext/glfw/*.h")
    list(APPEND HEADERS ${HEADERS_GLFW})

    file(GLOB_RECURSE SOURCES_GLFW CONFIGURE_DEPENDS "src/vclib/ext/glfw/*.cpp")
    list(APPEND SOURCES ${SOURCES_GLFW})
endif()

if (TARGET vclib-external-opengl)
    file(GLOB_RECURSE HEADERS_OPENGL2 CONFIGURE_DEPENDS
        "include/vclib/ext/opengl2/*.h")
    list(APPEND HEADERS ${HEADERS_OPENGL2})

    file(GLOB_RECURSE SOURCES_OPENGL2 CONFIGURE_DEPENDS
        "src/vclib/ext/opengl2/*.cpp")
    list(APPEND SOURCES ${SOURCES_OPENGL2})
endif()

if (TARGET vclib-external-qt)
    file(GLOB_RECURSE HEADERS_QT CONFIGURE_DEPENDS "include/vclib/ext/qt/*.h")
    list(APPEND HEADERS ${HEADERS_QT})

    file(GLOB_RECURSE SOURCES_QT CONFIGURE_DEPENDS "src/vclib/ext/qt/*.cpp")
    list(APPEND SOURCES ${SOURCES_QT})

    file(GLOB_RECURSE FORMS_QT CONFIGURE_DEPENDS "src/vclib/ext/qt/*.ui")
    list(APPEND FORMS ${FORMS_QT})

    if (TARGET vclib-external-qglviewer)
        file(GLOB_RECURSE HEADERS_QGLVIEWER CONFIGURE_DEPENDS
            "include/vclib/ext/qglviewer/*.h")
        list(APPEND HEADERS ${HEADERS_QGLVIEWER})

        file(GLOB_RECURSE SOURCES_QGLVIEWER CONFIGURE_DEPENDS
            "src/vclib/ext/qglviewer/*.cpp")
        list(APPEND SOURCES ${SOURCES_QGLVIEWER})

        file(GLOB_RECURSE FORMS_QGLVIEWER CONFIGURE_DEPENDS
            "src/vclib/ext/qglviewer/*.ui")
        list(APPEND FORMS ${FORMS_QGLVIEWER})
    endif()

    source_group("Forms Files" FILES ${FORMS})
endif()


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

target_include_directories(vclib-render PUBLIC ${VCLIB_RENDER_INCLUDE_DIR})
target_include_directories(vclib-render PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/include)

set_target_properties(vclib-render PROPERTIES
    VCLIB_RENDER_INCLUDE_DIR ${VCLIB_RENDER_INCLUDE_DIR})

set_target_properties(vclib-render PROPERTIES
    VCLIB_RENDER_SHADER_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

if (VCLIB_RENDER_ENGINE STREQUAL "bgfx")
    build_bgfx_shaders_to_headers(${SHADERS})
    build_assets_to_headers(${ASSETS})
endif()

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
