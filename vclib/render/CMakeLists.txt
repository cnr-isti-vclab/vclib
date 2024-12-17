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

# Options
set(VCLIB_RENDER_BACKEND "bgfx" CACHE STRING "Select the rendering engine")
set_property(CACHE VCLIB_RENDER_BACKEND PROPERTY STRINGS bgfx opengl2 none)

# external libraries
message(STATUS "VCLib-Render: 3rdparty libraries")
add_subdirectory(3rdparty)

set(VCLIB_RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/render/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/vclib/render/*.cpp")

file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "assets/*")

# remove all files in ${CMAKE_CURRENT_BINARY_DIR}/include/vclib
file(GLOB_RECURSE VCLIB_RENDER_GEN_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_BINARY_DIR}/include/vclib/*")
foreach(FILE ${VCLIB_RENDER_GEN_HEADERS})
    file(REMOVE ${FILE})
endforeach()

# if render engine is bgfx - include sources that use bgfx
if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    # file(GLOB_RECURSE HEADERS_BGFX CONFIGURE_DEPENDS
    #     "include/vclib/render_bgfx/*.h")
    # list(APPEND HEADERS ${HEADERS_BGFX})

    # file(GLOB_RECURSE SOURCES_BGFX CONFIGURE_DEPENDS
    #     "src/vclib/render_bgfx/*.cpp")
    # list(APPEND SOURCES ${SOURCES_BGFX})

    # if (APPLE)
    #     file(GLOB_RECURSE SOURCES_MM CONFIGURE_DEPENDS
    #         "src/vclib/render_bgfx/*.mm")
    #     list(APPEND SOURCES ${SOURCES_MM})
    # endif()

    # file(GLOB_RECURSE SHADERS_BGFX CONFIGURE_DEPENDS
    #     "shaders/vclib/render_bgfx/*.sh" "shaders/vclib/render_bgfx/*.sc")
    # list(APPEND SHADERS ${SHADERS_BGFX})
endif()

# if render engine is opengl2 - include sources that use opengl2
if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
    # file(GLOB_RECURSE HEADERS_OPENGL2 CONFIGURE_DEPENDS
    #     "include/vclib/render_opengl2/*.h")
    # list(APPEND HEADERS ${HEADERS_OPENGL2})

    # file(GLOB_RECURSE SOURCES_OPENGL2 CONFIGURE_DEPENDS
    #     "src/vclib/render_opengl2/*.cpp")
    # list(APPEND SOURCES ${SOURCES_OPENGL2})
endif()

if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    # if (TARGET vclib-3rd-glfw)
    #     file(GLOB_RECURSE HEADERS_GLFW CONFIGURE_DEPENDS
    #         "include/vclib/glfw/*.h")
    #     list(APPEND HEADERS ${HEADERS_GLFW})

    #     file(GLOB_RECURSE SOURCES_GLFW CONFIGURE_DEPENDS "src/vclib/glfw/*.cpp")
    #     list(APPEND SOURCES ${SOURCES_GLFW})
    # endif()

    # if (TARGET vclib-3rd-qt)
    #     file(GLOB_RECURSE HEADERS_QT CONFIGURE_DEPENDS "include/vclib/qt/*.h")
    #     list(APPEND HEADERS ${HEADERS_QT})

    #     file(GLOB_RECURSE SOURCES_QT CONFIGURE_DEPENDS "src/vclib/qt/*.cpp")
    #     list(APPEND SOURCES ${SOURCES_QT})

    #     file(GLOB_RECURSE FORMS_QT CONFIGURE_DEPENDS "src/vclib/qt/*.ui")
    #     list(APPEND FORMS ${FORMS_QT})

    #     source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})
    # endif()
endif()

if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
    # list(APPEND HEADERS
    #     include/vclib/glfw/canvas_window.h
    #     include/vclib/glfw/event_manager_window.h
    #     include/vclib/glfw/input.h
    #     )

    # list(APPEND SOURCES
    #     src/vclib/glfw/canvas_window.cpp
    #     src/vclib/glfw/event_manager_window.cpp
    #     src/vclib/glfw/input.cpp
    #     src/vclib/glfw/viewer_window.cpp
    #     )

    # if (TARGET vclib-3rd-qt)
    #     file(GLOB_RECURSE HEADERS_QT CONFIGURE_DEPENDS "include/vclib/qt/*.h")
    #     list(APPEND HEADERS ${HEADERS_QT})

    #     file(GLOB_RECURSE SOURCES_QT CONFIGURE_DEPENDS "src/vclib/qt/*.cpp")
    #     list(APPEND SOURCES ${SOURCES_QT})

    #     file(GLOB_RECURSE FORMS_QT CONFIGURE_DEPENDS "src/vclib/qt/*.ui")
    #     list(APPEND FORMS ${FORMS_QT})

    #     source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})
    # endif()
endif()

# todo: this is a terrible hack
# these files should be organized in a better way
if (NOT TARGET vclib-processing)
    # remove from HEADERS, SOURCES, FORMS all the files that use the processing
    # module (they contain the 'processing' string in the path)

    foreach(FILE ${HEADERS})
        if (FILE MATCHES "processing")
            list(REMOVE_ITEM HEADERS ${FILE})
        endif()
    endforeach()
    foreach(FILE ${SOURCES})
        if (FILE MATCHES "processing")
            list(REMOVE_ITEM SOURCES ${FILE})
        endif()
    endforeach()
    foreach(FILE ${FORMS})
        if (FILE MATCHES "processing")
            list(REMOVE_ITEM FORMS ${FILE})
        endif()
    endforeach()
endif()

# vclib-render target
if (WIN32)
    # on windows vclib-render must be static - needed for bgfx
    # TODO - try to make it work also on windows with SHARED...
    set(VLIB_RENDER_LIB_TYPE STATIC)
else()
    set(VLIB_RENDER_LIB_TYPE SHARED)
endif()

add_library(${PROJECT_NAME} ${VLIB_RENDER_LIB_TYPE}
    ${HEADERS} ${SOURCES} ${SHADERS} ${FORMS})

target_include_directories(${PROJECT_NAME} PUBLIC ${VCLIB_RENDER_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_RENDER_INCLUDE_DIR ${VCLIB_RENDER_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_RENDER_SHADER_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_RENDER_BACKEND_BGFX)

    build_bgfx_shaders_to_headers(${SHADERS})
    build_assets_to_headers(${ASSETS})
endif()

if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_RENDER_BACKEND_OPENGL2)
endif()

if (TARGET vclib-processing)
    target_link_libraries(${PROJECT_NAME} PUBLIC vclib-processing)
endif()

if (TARGET vclib-3rd-qt)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTORCC ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOUIC ON)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${VCLIB_RENDER_3RDPARTY_LIBRARIES} vclib-core)
