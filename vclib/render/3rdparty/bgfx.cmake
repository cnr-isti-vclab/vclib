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

find_package(bgfx QUIET)

set(VCLIB_BGFX_DIR ${CMAKE_CURRENT_LIST_DIR}/bgfx)

if (VCLIB_ALLOW_SYSTEM_BGFX AND bgfx_FOUND)
    message(STATUS "- bgfx - using system-provided library")

    add_library(vclib-3rd-bgfx INTERFACE)

    target_link_libraries(vclib-3rd-bgfx INTERFACE bgfx::bx bgfx::bgfx bgfx::bimg)
    target_include_directories(vclib-3rd-bgfx
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sdf/include)

    # make sure that the imported targets are global
    set_target_properties(bgfx::bin2c PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::texturec PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::texturev PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::geometryc PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::geometryv PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::shaderc PROPERTIES IMPORTED_GLOBAL TRUE)

    set(VCLIB_BGFX_CMAKE_DIR "${bgfx_DIR}")

    # get bgfx include path
    get_target_property(BGFX_INCLUDE_PATH bgfx::bgfx INTERFACE_INCLUDE_DIRECTORIES)

    # for each path in BGFX_INCLUDE_PATH:
    foreach(PATH ${BGFX_INCLUDE_PATH})
        # if $PATH/bgfx_shader.sh exists
        if (EXISTS ${PATH}/bgfx_shader.sh)
            # set BGFX_SHADER_INCLUDE_PATH to $PATH/bgfx
            set (BGFX_SHADER_INCLUDE_PATH ${PATH}/bgfx)
        endif()
    endforeach(PATH ${BGFX_INCLUDE_PATH})

    set_target_properties(vclib-3rd-bgfx PROPERTIES
        BGFX_SHADER_INCLUDE_PATH ${BGFX_SHADER_INCLUDE_PATH})

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-bgfx)

elseif (VCLIB_ALLOW_BUNDLED_BGFX AND EXISTS ${VCLIB_BGFX_DIR})
    set(VCLIB_BGFX_DIR ${CMAKE_CURRENT_LIST_DIR}/bgfx)

    message(STATUS "- bgfx - using bundled source")

    # leave the option to build bgfx examples, but set it to OFF by default
    option(BGFX_BUILD_EXAMPLES "Build bgfx examples" OFF)

    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    set(BGFX_OPENGL_VERSION 44)
    if (TARGET vclib-3rd-wayland AND VCLIB_RENDER_WITH_WAYLAND)
        add_definitions(-DWL_EGL_PLATFORM=1)
    endif()
    add_subdirectory(${VCLIB_BGFX_DIR})

    add_library(vclib-3rd-bgfx INTERFACE)

    # there are three warnings on gcc that we need to ignore
    set_property(TARGET bgfx PROPERTY COMPILE_WARNING_AS_ERROR OFF)

    target_link_libraries(vclib-3rd-bgfx INTERFACE bx bgfx bimg)
    target_include_directories(vclib-3rd-bgfx
        INTERFACE ${VCLIB_BGFX_DIR}/bgfx/3rdparty)

    set(VCLIB_BGFX_CMAKE_DIR "${VCLIB_BGFX_DIR}/cmake")

    set_target_properties(vclib-3rd-bgfx PROPERTIES
        BGFX_SHADER_INCLUDE_PATH ${VCLIB_BGFX_DIR}/bgfx/src)

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-bgfx)
else()
    message(FATAL_ERROR
        "bgfx is required - be sure to clone recursively the vclib repository.")
endif()

if (TARGET vclib-3rd-bgfx)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bgfx_config.cmake)
endif()
