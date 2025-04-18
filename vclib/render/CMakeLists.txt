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
project(vclib-render)

# Options
set(VCLIB_RENDER_BACKEND "bgfx" CACHE STRING "Select the rendering engine")
set_property(CACHE VCLIB_RENDER_BACKEND PROPERTY STRINGS bgfx opengl2 none)

option(VCLIB_RENDER_BUILD_SHARED_LIB "Build shared library (ignored on Windows)" ON)

# external libraries
message(STATUS "VCLib-Render: 3rdparty libraries")
add_subdirectory(3rdparty)

set(VCLIB_RENDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/render/*.h")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/vclib/render/*.cpp")

file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS "assets/*")

# if render engine is bgfx - include sources that use bgfx
if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    file(GLOB_RECURSE HEADERS_BGFX CONFIGURE_DEPENDS
        "include/vclib/bgfx/*.h")
    list(APPEND HEADERS ${HEADERS_BGFX})

    file(GLOB_RECURSE SOURCES_BGFX CONFIGURE_DEPENDS
        "src/vclib/bgfx/*.cpp")
    list(APPEND SOURCES ${SOURCES_BGFX})

    if (APPLE)
        file(GLOB_RECURSE SOURCES_MM CONFIGURE_DEPENDS
            "src/vclib/bgfx/*.mm")
        list(APPEND SOURCES ${SOURCES_MM})
    endif()

    file(GLOB_RECURSE SHADERS_BGFX CONFIGURE_DEPENDS
        "shaders/vclib/bgfx/*.sh" "shaders/vclib/bgfx/*.sc")
    list(APPEND SHADERS ${SHADERS_BGFX})

    if (TARGET vclib-3rd-imgui)
        file(GLOB_RECURSE HEADERS_BGFX_IMGUI CONFIGURE_DEPENDS
            "include/vclib/bgfx_imgui/*.h")
        list(APPEND HEADERS ${HEADERS_BGFX_IMGUI})

        file(GLOB_RECURSE SOURCES_BGFX_IMGUI CONFIGURE_DEPENDS
            "src/vclib/bgfx_imgui/*.cpp")
        list(APPEND SOURCES ${SOURCES_BGFX_IMGUI})

        file(GLOB_RECURSE SHADERS_BGFX_IMGUI CONFIGURE_DEPENDS
            "shaders/vclib/bgfx_imgui/*.sh" "shaders/vclib/bgfx_imgui/*.sc")
        list(APPEND SHADERS ${SHADERS_BGFX_IMGUI})
    endif()
endif()

# if render engine is opengl2 - include sources that use opengl2
if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
    file(GLOB_RECURSE HEADERS_OPENGL2 CONFIGURE_DEPENDS
        "include/vclib/opengl2/*.h")
    list(APPEND HEADERS ${HEADERS_OPENGL2})

    file(GLOB_RECURSE SOURCES_OPENGL2 CONFIGURE_DEPENDS
        "src/vclib/opengl2/*.cpp")
    list(APPEND SOURCES ${SOURCES_OPENGL2})
endif()

if (TARGET vclib-3rd-glfw)
    file(GLOB_RECURSE HEADERS_GLFW CONFIGURE_DEPENDS
        "include/vclib/glfw/*.h")
    list(APPEND HEADERS ${HEADERS_GLFW})

    file(GLOB_RECURSE SOURCES_GLFW CONFIGURE_DEPENDS "src/vclib/glfw/*.cpp")
    list(APPEND SOURCES ${SOURCES_GLFW})

    if (TARGET vclib-3rd-imgui)
        file(GLOB_RECURSE HEADERS_GLFW_IMGUI CONFIGURE_DEPENDS
            "include/vclib/glfw_imgui/*.h")
        list(APPEND HEADERS ${HEADERS_GLFW_IMGUI})

        file(GLOB_RECURSE SOURCES_GLFW_IMGUI CONFIGURE_DEPENDS
            "src/vclib/glfw_imgui/*.cpp")
        list(APPEND SOURCES ${SOURCES_GLFW_IMGUI})
    endif()
endif()

if (TARGET vclib-3rd-qt)
    file(GLOB_RECURSE HEADERS_QT CONFIGURE_DEPENDS "include/vclib/qt/*.h")
    list(APPEND HEADERS ${HEADERS_QT})

    file(GLOB_RECURSE SOURCES_QT CONFIGURE_DEPENDS "src/vclib/qt/*.cpp")
    list(APPEND SOURCES ${SOURCES_QT})

    file(GLOB_RECURSE FORMS_QT CONFIGURE_DEPENDS "src/vclib/qt/*.ui")
    list(APPEND FORMS ${FORMS_QT})

    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Forms Files" FILES ${FORMS})

    if (TARGET vclib-3rd-imgui)
        file(GLOB_RECURSE HEADERS_QT_IMGUI CONFIGURE_DEPENDS
            "include/vclib/qt_imgui/*.h")
        list(APPEND HEADERS ${HEADERS_QT_IMGUI})

        file(GLOB_RECURSE SOURCES_QT_IMGUI CONFIGURE_DEPENDS
            "src/vclib/qt_imgui/*.cpp")
        list(APPEND SOURCES ${SOURCES_QT_IMGUI})
    endif()
endif()

# vclib-render target
if (WIN32)
    # on windows vclib-render must be static - needed for bgfx
    # TODO - try to make it work also on windows with SHARED...
    set(VCLIB_RENDER_LIB_TYPE STATIC)
else()
    if (VCLIB_RENDER_BUILD_SHARED_LIB)
        set(VCLIB_RENDER_LIB_TYPE SHARED)
    else()
        set(VCLIB_RENDER_LIB_TYPE STATIC)
    endif()
endif()

add_library(${PROJECT_NAME} ${VCLIB_RENDER_LIB_TYPE}
    ${HEADERS} ${SOURCES} ${SHADERS} ${FORMS})
add_library(vclib::render ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${VCLIB_RENDER_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/include)

target_compile_definitions(${PROJECT_NAME} INTERFACE VCLIB_RENDER_MODULE)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_RENDER_INCLUDE_DIR ${VCLIB_RENDER_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_RENDER_SHADER_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders")

if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfx_config.cmake)

    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_RENDER_BACKEND_BGFX)

    build_bgfx_shaders_to_headers(${SHADERS})
    build_assets_to_headers(${ASSETS})
endif()

if (VCLIB_RENDER_BACKEND STREQUAL "opengl2")
    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_RENDER_BACKEND_OPENGL2)
endif()

if (TARGET vclib-3rd-qt)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTORCC ON)
    set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOUIC ON)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${VCLIB_RENDER_3RDPARTY_LIBRARIES} vclib::core)

# Install
install(DIRECTORY ${VCLIB_RENDER_INCLUDE_DIR}/vclib
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/vclib
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if (VCLIB_RENDER_BACKEND STREQUAL "bgfx")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfx_config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vclib)
endif()
