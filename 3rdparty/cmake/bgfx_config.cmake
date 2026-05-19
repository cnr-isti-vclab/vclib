#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2026                                                    *
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

# Replace original bgfx profile names with the ones used by vclib
function(_bgfx_get_profile_path_ext PROFILE PROFILE_PATH_EXT)
    string(REPLACE 100_es essl PROFILE ${PROFILE})
    string(REPLACE 300_es essl PROFILE ${PROFILE})
    string(REPLACE 120 glsl PROFILE ${PROFILE})
    string(REPLACE 430 glsl PROFILE ${PROFILE})
    string(REPLACE s_5_0 dxbc PROFILE ${PROFILE})
    string(REPLACE s_6_0 dxil PROFILE ${PROFILE})

    string(REPLACE 140 glsl PROFILE ${PROFILE})
    string(REPLACE 400 glsl PROFILE ${PROFILE})
    string(REPLACE 320_es essl PROFILE ${PROFILE})
    string(REPLACE metal22-11 mtl PROFILE ${PROFILE})

    # after replacing metal22-11
    string(REPLACE metal mtl PROFILE ${PROFILE})

    set(${PROFILE_PATH_EXT} ${PROFILE} PARENT_SCOPE)
endfunction()

# Replace original bgfx profile names with the ones used by vclib
function(_bgfx_get_profile_ext PROFILE PROFILE_EXT)
    string(REPLACE 100_es essl PROFILE ${PROFILE})
    string(REPLACE 300_es essl PROFILE ${PROFILE})
    string(REPLACE 120 glsl PROFILE ${PROFILE})
    string(REPLACE 430 glsl PROFILE ${PROFILE})
    string(REPLACE spirv spv PROFILE ${PROFILE})
    string(REPLACE s_5_0 dxbc PROFILE ${PROFILE})
    string(REPLACE s_6_0 dxil PROFILE ${PROFILE})

    string(REPLACE 140 glsl PROFILE ${PROFILE})
    string(REPLACE 400 glsl PROFILE ${PROFILE})
    string(REPLACE 320_es essl PROFILE ${PROFILE})
    string(REPLACE metal22-11 mtl PROFILE ${PROFILE})

    # after replacing metal22-11
    string(REPLACE metal mtl PROFILE ${PROFILE})

    set(${PROFILE_EXT} ${PROFILE} PARENT_SCOPE)
endfunction()

function(_vclib_target_ide_add_shaders target_name)
    list(REMOVE_AT ARGV 0)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Shaders" FILES ${ARGV})
    target_sources(${target_name} PRIVATE ${ARGV})
endfunction()

# Function to add a list of assets to a target
function(_vclib_target_ide_add_assets target_name)
    list(REMOVE_AT ARGV 0)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Asset Files" FILES ${ARGV})
    target_sources(${target_name} PRIVATE ${ARGV})
endfunction()

# vclib_build_shader(
#   SHADER <shader_file>
#   OUT_DIR <output_directory>
#   [VARYING_DEF <varying_def_file>]
#   [AS_HEADER]
# )
function(vclib_build_shader)
    # single value arguments: SHADER, OUT_DIR, [VARYING_DEF]
    # options: AS_HEADER
    set(options AS_HEADER)
    set(oneValueArgs SHADER OUT_DIR VARYING_DEF)
    set(multiValueArgs)
    cmake_parse_arguments(ARG
        "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

    if (NOT ARG_SHADER)
        message(FATAL_ERROR "vclib_build_shader: missing SHADER argument")
    endif()
    if (NOT ARG_OUT_DIR)
        message(FATAL_ERROR "vclib_build_shader: missing OUT_DIR argument")
    endif()

    set(BGFX_VF_PROFILES
        140    # glsl
        320_es # essl
        spirv  # spirv
    )
    set(BGFX_COMPUTE_PROFILES
        400    # glsl
        320_es # essl
        spirv  # spirv
    )
    if (APPLE)
        list(APPEND BGFX_VF_PROFILES
            metal22-11 # metal
        )
        list(APPEND BGFX_COMPUTE_PROFILES
            metal22-11 # metal
        )
    endif()
    if (WIN32)
        list(APPEND BGFX_VF_PROFILES
            s_5_0 # dxbc, dx11
        )
        list(APPEND BGFX_COMPUTE_PROFILES
            s_5_0 # dxbc, dx11
        )
    endif()

    if (ARG_AS_HEADER)
        set(AS_HEADER_OPTION AS_HEADERS)
    else()
        set(AS_HEADER_OPTION "")
    endif()

    get_property(VCLIB_RENDER_DIR
        TARGET vclib::render
        PROPERTY VCLIB_RENDER_INCLUDE_DIR)
    get_property(VCLIB_RENDER_SHADER_DIR
        TARGET vclib::render
        PROPERTY VCLIB_RENDER_SHADER_INCLUDE_DIR)

    get_filename_component(FILENAME "${ARG_SHADER}" NAME_WE)
    get_filename_component(ABSOLUTE_PATH_SHADER ${ARG_SHADER} ABSOLUTE)
    get_filename_component(ABSOLUTE_DIR_PATH ${ABSOLUTE_PATH_SHADER} DIRECTORY)

    # used by bgfx_compile_shaders to resolve includes in shaders
    get_property(BGFX_SHADER_INCLUDE_PATH
        TARGET vclib-3rd-bgfx
        PROPERTY BGFX_SHADER_INCLUDE_PATH)

    string(SUBSTRING "${FILENAME}" 0 2 TYPE)
    if("${TYPE}" STREQUAL "fs")
        set(TYPE "FRAGMENT")
    elseif("${TYPE}" STREQUAL "vs")
        set(TYPE "VERTEX")
    elseif("${TYPE}" STREQUAL "cs")
        set(TYPE "COMPUTE")
    else()
        set(TYPE "")
    endif()

    if ("${TYPE}" STREQUAL "COMPUTE")
        set(VARYING_DEF_ARGUMENT "")
        set(PROFILES_ARGUMENT PROFILES ${BGFX_COMPUTE_PROFILES})
    else()
        if(ARG_VARYING_DEF)
            set(VARYING_DEF_ARGUMENT VARYING_DEF ${ARG_VARYING_DEF})
        else()
            set(VARYING_DEF_ARGUMENT VARYING_DEF ${ABSOLUTE_DIR_PATH}/varying.def.sc)
        endif()
        set(PROFILES_ARGUMENT PROFILES ${BGFX_VF_PROFILES})
    endif()

    if(NOT "${TYPE}" STREQUAL "")
        bgfx_compile_shaders(
            TYPE ${TYPE}
            SHADERS ${ARG_SHADER}
            ${VARYING_DEF_ARGUMENT}
            OUTPUT_DIR ${ARG_OUT_DIR}
            INCLUDE_DIRS "${VCLIB_RENDER_SHADER_DIR};${VCLIB_RENDER_DIR}"
            ${PROFILES_ARGUMENT}
            ${AS_HEADER_OPTION}
            # NO_SOURCE_GROUP
        )
    endif()
endfunction()

# vclib_build_shaders_to_headers(<shader_file1> <shader_file2> ...)
function(vclib_build_shaders_to_headers)
    _vclib_target_ide_add_shaders(vclib-render ${ARGV})

    get_property(TARGET_BIN_DIR TARGET vclib::render PROPERTY BINARY_DIR)
    get_property(VCLIB_RENDER_SHADER_DIR
        TARGET vclib::render
        PROPERTY VCLIB_RENDER_SHADER_INCLUDE_DIR)

    set(BGFX_SHADERS_OUTPUT_DIR "${TARGET_BIN_DIR}/include/vclib/shaders")

    foreach(SHADER ${ARGV})
        file(RELATIVE_PATH SHADER_REL "${VCLIB_RENDER_SHADER_DIR}/../shaders/vclib/bgfx" ${SHADER})
        get_filename_component(DIR_PATH ${SHADER_REL} DIRECTORY)
        set(OUT_DIR ${BGFX_SHADERS_OUTPUT_DIR}/${DIR_PATH})

        vclib_build_shader(
            SHADER ${SHADER}
            OUT_DIR ${OUT_DIR}
            AS_HEADER
        )
    endforeach()
endfunction()

# vclib_build_assets_to_headers(<asset_file1> <asset_file2> ...)
function(vclib_build_assets_to_headers)
    _vclib_target_ide_add_assets(vclib-render ${ARGV})

    get_property(VCLIB_RENDER_DIR TARGET vclib::render PROPERTY VCLIB_RENDER_INCLUDE_DIR)
    get_property(TARGET_BIN_DIR TARGET vclib::render PROPERTY BINARY_DIR)

    set(BGFX_ASSETS_OUTPUT_DIR "${TARGET_BIN_DIR}/include/vclib")

    foreach(ASSET ${ARGV})
        file(RELATIVE_PATH ASSET_REL "${VCLIB_RENDER_DIR}/.." ${ASSET})
        get_filename_component(DIR_PATH ${ASSET_REL} DIRECTORY)
        get_filename_component(FILENAME "${ASSET}" NAME)
        get_filename_component(FILENAME_WE "${ASSET}" NAME_WE)
        get_filename_component(ABSOLUTE_PATH_ASSET ${ASSET} ABSOLUTE)
        get_filename_component(ABSOLUTE_DIR_PATH ${DIR_PATH} ABSOLUTE)

        bgfx_compile_binary_to_header(
            INPUT_FILE ${ABSOLUTE_PATH_ASSET}
            OUTPUT_FILE ${BGFX_ASSETS_OUTPUT_DIR}/${DIR_PATH}/${FILENAME}.bin.h
            ARRAY_NAME ${FILENAME_WE}
        )
    endforeach()
endfunction()

# vclib_target_add_shaders(<target_name> <shader_file1> <shader_file2> ...)
# Function to add a list of shaders to a target.
# When building tha target, the shaders will be compiled and placed in the
# following directory:
# <BINARY_DIR>/shaders/<platform>/<PATH_TO_/FILE_NAME>.bin
# Note: if the path to the shader begins with "shaders/", it will be removed
# (the same is done in the "bgf::loadProgram" function)
function(vclib_target_add_shaders target_name)
    get_property(TARGET_BIN_DIR TARGET ${target_name} PROPERTY BINARY_DIR)

    list(REMOVE_AT ARGV 0)

    foreach(SHADER ${ARGV})
        get_filename_component(ABSOLUTE_PATH_SHADER ${SHADER} ABSOLUTE)
        get_filename_component(DIR_PATH ${SHADER} DIRECTORY)

        # if DIR_PATH begins with "shaders/", remove it
        if (DIR_PATH MATCHES "^shaders/")
            string(SUBSTRING ${DIR_PATH} 8 -1 DIR_PATH)
        endif()
        # if DIR_PATH is "shaders", replace it with "./"
        if (DIR_PATH STREQUAL "shaders")
            set(DIR_PATH "./")
        endif()

        set(OUT_DIR ${TARGET_BIN_DIR}/shaders/${DIR_PATH})

        vclib_build_shader(
            SHADER "${ABSOLUTE_PATH_SHADER}"
            OUT_DIR "${OUT_DIR}")
    endforeach()
    _vclib_target_ide_add_shaders(${target_name} ${ARGV})
endfunction()
