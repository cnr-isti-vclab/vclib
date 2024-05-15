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

include(${VCLIB_BGFX_DIR}/cmake/bgfxToolUtils.cmake)

set(GLSL_PROFILE 140)
set(GLSL_COMPUTE_PROFILE 300)
set(ESSL_PROFILE 320_es)
set(SPIRV_PROFILE spirv)
set(DX_PROFILE s_5_0)
set(METAL_PROFILE metal22-11)

# bgfx_compile_shader_to_header(
# 	TYPE VERTEX|FRAGMENT|COMPUTE
# 	SHADERS filenames
# 	VARYING_DEF filename
# 	OUTPUT_DIR directory
# 	OUT_FILES_VAR variable name
# 	INCLUDE_DIRS directories
# )
#
function(_bgfx_compile_shader_to_header)
	set(options "")
	set(oneValueArgs TYPE VARYING_DEF OUTPUT_DIR OUT_FILES_VAR)
	set(multiValueArgs SHADERS INCLUDE_DIRS)
	cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

	set(PROFILES ${ESSL_PROFILE} ${SPIRV_PROFILE}) # pssl

	if (NOT "${ARGS_TYPE}" STREQUAL "COMPUTE")
		list(APPEND PROFILES ${GLSL_PROFILE})
	else()
		list(APPEND PROFILES ${GLSL_COMPUTE_PROFILE})
	endif()

	if(UNIX AND NOT APPLE)
		set(PLATFORM LINUX)
	elseif(APPLE)
		set(PLATFORM OSX)
		list(APPEND PROFILES ${METAL_PROFILE})
	elseif(
		WIN32
		OR MINGW
		OR MSYS
		OR CYGWIN
	)
		set(PLATFORM WINDOWS)
		list(APPEND PROFILES ${DX_PROFILE})
	else()
		message(error "shaderc: Unsupported platform")
	endif()

	set(ALL_OUTPUTS "")
	foreach(SHADER_FILE ${ARGS_SHADERS})
		source_group("Shaders" FILES "${SHADER}")
		get_filename_component(SHADER_FILE_BASENAME ${SHADER_FILE} NAME)
		get_filename_component(SHADER_FILE_NAME_WE ${SHADER_FILE} NAME_WE)
		get_filename_component(SHADER_FILE_ABSOLUTE ${SHADER_FILE} ABSOLUTE)

		# Build output targets and their commands
		set(OUTPUTS "")
		set(COMMANDS "")
		foreach(PROFILE ${PROFILES})
			_bgfx_get_profile_ext(${PROFILE} PROFILE_EXT)
			set(OUTPUT ${ARGS_OUTPUT_DIR}/${SHADER_FILE_BASENAME}.${PROFILE_EXT}.bin.h)
			set(PLATFORM_I ${PLATFORM})
			if(PROFILE STREQUAL "spirv")
				set(PLATFORM_I LINUX)
			endif()
			_bgfx_shaderc_parse(
				CLI #
				${ARGS_TYPE} ${PLATFORM_I} WERROR "$<$<CONFIG:debug>:DEBUG>$<$<CONFIG:relwithdebinfo>:DEBUG>"
				FILE ${SHADER_FILE_ABSOLUTE}
				OUTPUT ${OUTPUT}
				PROFILE ${PROFILE}
				O "$<$<CONFIG:debug>:0>$<$<CONFIG:release>:3>$<$<CONFIG:relwithdebinfo>:3>$<$<CONFIG:minsizerel>:3>"
				VARYINGDEF ${ARGS_VARYING_DEF}
				INCLUDES ${BGFX_SHADER_INCLUDE_PATH} ${ARGS_INCLUDE_DIRS}
				BIN2C BIN2C ${SHADER_FILE_NAME_WE}_${PROFILE_EXT}
			)
			list(APPEND OUTPUTS ${OUTPUT})
			list(APPEND ALL_OUTPUTS ${OUTPUT})
			list(APPEND COMMANDS COMMAND bgfx::shaderc ${CLI})
		endforeach()

		add_custom_command(
			OUTPUT ${OUTPUTS}
			COMMAND ${CMAKE_COMMAND} -E make_directory ${ARGS_OUTPUT_DIR} ${COMMANDS}
			MAIN_DEPENDENCY ${SHADER_FILE_ABSOLUTE}
			DEPENDS ${ARGS_VARYING_DEF}
		)
	endforeach()

	if(DEFINED ARGS_OUT_FILES_VAR)
		set(${ARGS_OUT_FILES_VAR} ${ALL_OUTPUTS} PARENT_SCOPE)
	endif()
endfunction()

# FILE: the absolute path of the shader file
# DIR: the portion of path where the binary output shader will be placed
#      the final output will be
#      <BINARY_DIR>/shaders/<platform>/<DIR>/<FILE_NAME>.bin
# TARGET: the target to add the shader to
#
# The compiled shader will be placed in the following directory:
# <BINARY_DIR>/shaders/<platform>/<DIR>/<FILE_NAME>.bin
function(_add_bgfx_shader FILE DIR TARGET)
	get_property(TARGET_BIN_DIR TARGET ${TARGET} PROPERTY BINARY_DIR)
	set(BGFX_SHADERS_OUTPUT_DIR "${TARGET_BIN_DIR}/shaders")

	get_filename_component(FILENAME "${FILE}" NAME_WE)
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

	if(NOT "${TYPE}" STREQUAL "")
		get_property(VCLIB_RENDER_DIR TARGET vclib-render PROPERTY VCLIB_RENDER_INCLUDE_DIR)
		get_property(VCLIB_RENDER_SHADER_DIR TARGET vclib-render PROPERTY VCLIB_RENDER_BGFX_SHADER_INCLUDE_DIR)
		set(COMMON FILE ${FILE} ${TYPE} INCLUDES "${BGFX_DIR}/src;${VCLIB_RENDER_DIR};${VCLIB_RENDER_SHADER_DIR}")
		set(OUTPUTS "")
		set(OUTPUTS_PRETTY "")

		if(WIN32)
			# dx11
			set(DX11_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/dx11/${DIR}/${FILENAME}.bin)
			if(NOT "${TYPE}" STREQUAL "COMPUTE")
				_bgfx_shaderc_parse(
					DX11 ${COMMON} WINDOWS
					PROFILE ${DX_PROFILE}
					O 3
					OUTPUT ${DX11_OUTPUT}
				)
			else()
				_bgfx_shaderc_parse(
					DX11 ${COMMON} WINDOWS
					PROFILE ${DX_PROFILE}
					O 1
					OUTPUT ${DX11_OUTPUT}
				)
			endif()
			list(APPEND OUTPUTS "DX11")
			set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX11, ")
		endif()

		if(APPLE)
			# metal
			set(METAL_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/metal/${DIR}/${FILENAME}.bin)
			_bgfx_shaderc_parse(METAL ${COMMON} OSX PROFILE ${METAL_PROFILE} OUTPUT ${METAL_OUTPUT})
			list(APPEND OUTPUTS "METAL")
			set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}Metal, ")
		endif()

		# essl
		if(NOT "${TYPE}" STREQUAL "COMPUTE")
			set(ESSL_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/essl/${DIR}/${FILENAME}.bin)
			_bgfx_shaderc_parse(ESSL ${COMMON} ANDROID PROFILE ${ESSL_PROFILE} OUTPUT ${ESSL_OUTPUT})
			list(APPEND OUTPUTS "ESSL")
			set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}ESSL, ")
		endif()

		# glsl
		set(GLSL_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/glsl/${DIR}/${FILENAME}.bin)
		if(NOT "${TYPE}" STREQUAL "COMPUTE")
			_bgfx_shaderc_parse(GLSL ${COMMON} LINUX PROFILE ${GLSL_PROFILE} OUTPUT ${GLSL_OUTPUT})
		else()
			_bgfx_shaderc_parse(GLSL ${COMMON} LINUX PROFILE ${GLSL_COMPUTE_PROFILE} OUTPUT ${GLSL_OUTPUT})
		endif()
		list(APPEND OUTPUTS "GLSL")
		set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}GLSL, ")

		# spirv
		if(NOT "${TYPE}" STREQUAL "COMPUTE")
			set(SPIRV_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/spirv/${DIR}/${FILENAME}.bin)
			_bgfx_shaderc_parse(SPIRV ${COMMON} LINUX PROFILE ${SPIRV_PROFILE} OUTPUT ${SPIRV_OUTPUT})
			list(APPEND OUTPUTS "SPIRV")
			set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}SPIRV")
			set(OUTPUT_FILES "")
			set(COMMANDS "")
		endif()

		foreach(OUT ${OUTPUTS})
			list(APPEND OUTPUT_FILES ${${OUT}_OUTPUT})
			list(APPEND COMMANDS COMMAND "bgfx::shaderc" ${${OUT}})
			get_filename_component(OUT_DIR ${${OUT}_OUTPUT} DIRECTORY)
			file(MAKE_DIRECTORY ${OUT_DIR})
		endforeach()

		file(RELATIVE_PATH PRINT_NAME ${CMAKE_CURRENT_SOURCE_DIR} ${FILE})

		add_custom_command(
			MAIN_DEPENDENCY ${FILE}
			OUTPUT ${OUTPUT_FILES}
			${COMMANDS}
			COMMENT "Compiling shader ${PRINT_NAME} for ${OUTPUTS_PRETTY}"
		)
	endif()
endfunction()

function(ide_add_bgfx_shaders target_name)
    list(REMOVE_AT ARGV 0)
    source_group("Shader Files" FILES ${ARGV})
    target_sources(${target_name} PRIVATE ${ARGV})
endfunction()

# Function to add a list of shaders to a target
# When building tha target, the shaders will be compiled and placed in the
# following directory:
# <BINARY_DIR>/shaders/<platform>/<PATH_TO_/FILE_NAME>.bin
# Note: if the path to the shader begins with "shaders/", it will be removed
# (the same is done in the "bgf::loadProgram" function)
function(target_add_bgfx_shaders target_name)
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

        _add_bgfx_shader("${ABSOLUTE_PATH_SHADER}" "${DIR_PATH}" ${target_name})
    endforeach()
    ide_add_bgfx_shaders(${target_name} ${ARGV})
endfunction()

function(build_bgfx_shaders_to_headers)
    get_property(TARGET_BIN_DIR TARGET vclib-render PROPERTY BINARY_DIR)
    get_property(VCLIB_RENDER_DIR TARGET vclib-render PROPERTY VCLIB_RENDER_INCLUDE_DIR)
    get_property(VCLIB_RENDER_SHADER_DIR TARGET vclib-render PROPERTY VCLIB_RENDER_BGFX_SHADER_INCLUDE_DIR)

    set(BGFX_SHADERS_OUTPUT_DIR "${TARGET_BIN_DIR}/include")

    message(STATUS "OutDir: ${BGFX_SHADERS_OUTPUT_DIR}")
    foreach(SHADER ${ARGV})
        get_filename_component(DIR_PATH ${SHADER} DIRECTORY)
        get_filename_component(FILENAME "${SHADER}" NAME_WE)
        get_filename_component(ABSOLUTE_PATH_SHADER ${SHADER} ABSOLUTE)
        get_filename_component(ABSOLUTE_DIR_PATH ${DIR_PATH} ABSOLUTE)

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

        if(NOT "${TYPE}" STREQUAL "")
            _bgfx_compile_shader_to_header(
                TYPE ${TYPE}
                SHADERS ${ABSOLUTE_PATH_SHADER}
                VARYING_DEF "${ABSOLUTE_DIR_PATH}/varying.def.sc"
                OUTPUT_DIR ${BGFX_SHADERS_OUTPUT_DIR}/${DIR_PATH}
                INCLUDE_DIRS "${BGFX_DIR}/src;${VCLIB_RENDER_DIR};${VCLIB_RENDER_SHADER_DIR}"
            )
        endif()
    endforeach()
endfunction()

# Function to make available the bgfx shaders defined by vclib to the given
# target
function(target_expose_vclib_bgfx_shaders target_name)
    get_property(VCLIB_SHADERS TARGET vclib-render PROPERTY VCLIB_RENDER_BGFX_SHADERS)
    get_property(VCLIB_RENDER_DIR TARGET vclib-render PROPERTY VCLIB_RENDER_INCLUDE_DIR)

    foreach(SHADER ${VCLIB_SHADERS})
        get_filename_component(DIR_PATH ${SHADER} DIRECTORY)

        # remove "shaders/" characters from DIR_PATH
        string(SUBSTRING ${DIR_PATH} 8 -1 DIR_PATH)

        _add_bgfx_shader("${VCLIB_RENDER_DIR}/../${SHADER}" "${DIR_PATH}" ${target_name})
        ide_add_bgfx_shaders(${target_name} "${VCLIB_RENDER_DIR}/../${SHADER}")
    endforeach()
endfunction()
