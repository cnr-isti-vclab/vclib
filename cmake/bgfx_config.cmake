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
					PROFILE s_5_0
					O 3
					OUTPUT ${DX11_OUTPUT}
				)
			else()
				_bgfx_shaderc_parse(
					DX11 ${COMMON} WINDOWS
					PROFILE s_5_0
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
			_bgfx_shaderc_parse(METAL ${COMMON} OSX PROFILE metal22-11 OUTPUT ${METAL_OUTPUT})
			list(APPEND OUTPUTS "METAL")
			set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}Metal, ")
		endif()

		# essl
		if(NOT "${TYPE}" STREQUAL "COMPUTE")
			set(ESSL_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/essl/${DIR}/${FILENAME}.bin)
			_bgfx_shaderc_parse(ESSL ${COMMON} ANDROID PROFILE 320_es OUTPUT ${ESSL_OUTPUT})
			list(APPEND OUTPUTS "ESSL")
			set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}ESSL, ")
		endif()

		# glsl
		set(GLSL_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/glsl/${DIR}/${FILENAME}.bin)
		if(NOT "${TYPE}" STREQUAL "COMPUTE")
			_bgfx_shaderc_parse(GLSL ${COMMON} LINUX PROFILE 140 OUTPUT ${GLSL_OUTPUT})
		else()
			_bgfx_shaderc_parse(GLSL ${COMMON} LINUX PROFILE 300 OUTPUT ${GLSL_OUTPUT})
		endif()
		list(APPEND OUTPUTS "GLSL")
		set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}GLSL, ")

		# spirv
		if(NOT "${TYPE}" STREQUAL "COMPUTE")
			set(SPIRV_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/spirv/${DIR}/${FILENAME}.bin)
			_bgfx_shaderc_parse(SPIRV ${COMMON} LINUX PROFILE spirv OUTPUT ${SPIRV_OUTPUT})
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
