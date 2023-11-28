include(${VCLIB_BGFX_DIR}/cmake/bgfxToolUtils.cmake)

# FILE: the absolute path of the shader file
# DIR: the relative path of the shader file w.r.t the CMakeLists file of the
#      current project
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
		set(COMMON FILE ${FILE} ${TYPE} INCLUDES ${BGFX_DIR}/src)
		set(OUTPUTS "")
		set(OUTPUTS_PRETTY "")

		if(WIN32)
			# dx9
			if(NOT "${TYPE}" STREQUAL "COMPUTE")
				set(DX9_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/dx9/${DIR}/${FILENAME}.bin)
				_bgfx_shaderc_parse(
					DX9 ${COMMON} WINDOWS
					PROFILE s_3_0
					O 3
					OUTPUT ${DX9_OUTPUT}
				)
				list(APPEND OUTPUTS "DX9")
				set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX9, ")
			endif()

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
			_bgfx_shaderc_parse(METAL ${COMMON} OSX PROFILE metal OUTPUT ${METAL_OUTPUT})
			list(APPEND OUTPUTS "METAL")
			set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}Metal, ")
		endif()

		# essl
		if(NOT "${TYPE}" STREQUAL "COMPUTE")
			set(ESSL_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/essl/${DIR}/${FILENAME}.bin)
			_bgfx_shaderc_parse(ESSL ${COMMON} ANDROID PROFILE 100_es OUTPUT ${ESSL_OUTPUT})
			list(APPEND OUTPUTS "ESSL")
			set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}ESSL, ")
		endif()

		# glsl
		set(GLSL_OUTPUT ${BGFX_SHADERS_OUTPUT_DIR}/glsl/${DIR}/${FILENAME}.bin)
		if(NOT "${TYPE}" STREQUAL "COMPUTE")
			_bgfx_shaderc_parse(GLSL ${COMMON} LINUX PROFILE 140 OUTPUT ${GLSL_OUTPUT})
		else()
			_bgfx_shaderc_parse(GLSL ${COMMON} LINUX PROFILE 430 OUTPUT ${GLSL_OUTPUT})
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

	target_sources(
		${TARGET} PRIVATE ${FILE})
endfunction()


# Function to add a list of shaders to a target
# When building tha target, the shaders will be compiled and placed in the
# following directory:
# <BINARY_DIR>/shaders/<platform>/<PATH_TO_/FILE_NAME>.bin
function(target_add_bgfx_shaders target_name)
    list(GET ARGV 0 target_name)

    list(REMOVE_AT ARGV 0)

    foreach(SHADER ${ARGV})
        get_filename_component(ABSOLUTE_PATH_SHADER ${SHADER} ABSOLUTE)
        get_filename_component(DIR_PATH ${SHADER} DIRECTORY)

        _add_bgfx_shader("${ABSOLUTE_PATH_SHADER}" "${DIR_PATH}" ${target_name})
    endforeach()
    source_group("Shader Files" FILES ${ARGV})
endfunction()

# Function to make available the bgfx shaders defined by vclib to the given
# target
function(target_expose_vclib_bgfx_shaders target_name)
    get_property(TARGET_BIN_DIR TARGET ${target_name} PROPERTY BINARY_DIR)

    get_property(VCLIB_BIN_DIR TARGET vclib-render PROPERTY BINARY_DIR)

    # set VCLIB_RELATIVE_SHADERS_PATH to be the relative path from
    # TARGET_BIN_DIR to VCLIB_BIN_DIR
    file(RELATIVE_PATH
        VCLIB_RELATIVE_SHADERS_PATH ${TARGET_BIN_DIR} ${VCLIB_BIN_DIR})

    target_compile_definitions(vclib-render INTERFACE
        VCLIB_RELATIVE_SHADERS_PATH="${VCLIB_RELATIVE_SHADERS_PATH}/")
endfunction()
