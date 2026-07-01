# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

include(${CMAKE_CURRENT_LIST_DIR}/vclib_shader_combinations.cmake)

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
    set(SRC_FILES "")
    set(BIN_FILES "")
    foreach(FILE ${ARGV})
        string(FIND "${FILE}" "${CMAKE_CURRENT_BINARY_DIR}" IS_IN_BIN_DIR)
        if(IS_IN_BIN_DIR EQUAL 0)
            list(APPEND BIN_FILES ${FILE})
        else()
            list(APPEND SRC_FILES ${FILE})
        endif()
    endforeach()
    if(SRC_FILES)
        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Shaders" FILES ${SRC_FILES})
    endif()
    if(BIN_FILES)
        source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} PREFIX "Shaders" FILES ${BIN_FILES})
    endif()
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
    set(multiValueArgs INCLUDE_DIRS)
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
            INCLUDE_DIRS "${VCLIB_RENDER_SHADER_DIR};${VCLIB_RENDER_DIR};${ARG_INCLUDE_DIRS}"
            ${PROFILES_ARGUMENT}
            ${AS_HEADER_OPTION}
            NO_SOURCE_GROUP
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
        string(FIND "${SHADER}" "${TARGET_BIN_DIR}" IS_IN_BIN_DIR)
        if(IS_IN_BIN_DIR EQUAL 0)
            file(RELATIVE_PATH SHADER_REL "${TARGET_BIN_DIR}/shaders/vclib/bgfx" ${SHADER})
            get_filename_component(DIR_PATH ${SHADER_REL} DIRECTORY)
            set(VARYING_DEF_FILE "${VCLIB_RENDER_SHADER_DIR}/vclib/bgfx/${DIR_PATH}/varying.def.sc")
        else()
            file(RELATIVE_PATH SHADER_REL "${VCLIB_RENDER_SHADER_DIR}/../shaders/vclib/bgfx" ${SHADER})
            get_filename_component(DIR_PATH ${SHADER_REL} DIRECTORY)
            set(VARYING_DEF_FILE "")
        endif()
        set(OUT_DIR ${BGFX_SHADERS_OUTPUT_DIR}/${DIR_PATH})

        if(VARYING_DEF_FILE AND EXISTS "${VARYING_DEF_FILE}")
            vclib_build_shader(
                SHADER ${SHADER}
                OUT_DIR ${OUT_DIR}
                VARYING_DEF "${VARYING_DEF_FILE}"
                INCLUDE_DIRS "${VCLIB_RENDER_SHADER_DIR}/vclib/bgfx/${DIR_PATH}"
                AS_HEADER
            )
        else()
            vclib_build_shader(
                SHADER ${SHADER}
                OUT_DIR ${OUT_DIR}
                AS_HEADER
            )
        endif()
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
        
        file(MAKE_DIRECTORY "${BGFX_ASSETS_OUTPUT_DIR}/${DIR_PATH}")

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

function(_vclib_embedded_program_validate_args ENUM_NAME TYPE FILES)
    if (NOT ENUM_NAME)
        message(FATAL_ERROR "vclib_add_embedded_program: missing ENUM_NAME argument")
    endif()

    if (NOT TYPE)
        message(FATAL_ERROR "vclib_add_embedded_program: missing TYPE argument")
    endif()

    if (NOT FILES)
        message(FATAL_ERROR "vclib_add_embedded_program: missing FILES argument")
    endif()
endfunction()

function(_vclib_embedded_program_resolve_type TYPE OUT_PROGRAM_TYPE OUT_PROGRAM_TYPE_LC)
    string(TOUPPER "${TYPE}" TYPE_UC)
    if (TYPE_UC STREQUAL "VERT_FRAG")
        set(PROGRAM_TYPE "VF")
        set(PROGRAM_TYPE_LC "vf")
    elseif (TYPE_UC STREQUAL "COMPUTE")
        set(PROGRAM_TYPE "C")
        set(PROGRAM_TYPE_LC "c")
    else()
        message(FATAL_ERROR "vclib_add_embedded_program: TYPE must be VERT_FRAG or COMPUTE")
    endif()

    set(${OUT_PROGRAM_TYPE} "${PROGRAM_TYPE}" PARENT_SCOPE)
    set(${OUT_PROGRAM_TYPE_LC} "${PROGRAM_TYPE_LC}" PARENT_SCOPE)
endfunction()

function(_vclib_embedded_program_prepare_output_dirs PROGRAM_TYPE_LC OUT_TARGET_BIN_DIR OUT_HEADER_DIR OUT_SOURCE_DIR)
    get_property(TARGET_BIN_DIR TARGET vclib-render PROPERTY BINARY_DIR)

    set(GENERATED_HEADER_DIR "${TARGET_BIN_DIR}/include/vclib/bgfx/programs/embedded_${PROGRAM_TYPE_LC}_programs")
    set(GENERATED_SOURCE_DIR "${TARGET_BIN_DIR}/src/vclib/bgfx/programs/embedded_${PROGRAM_TYPE_LC}_programs")

    file(MAKE_DIRECTORY ${GENERATED_HEADER_DIR})
    file(MAKE_DIRECTORY ${GENERATED_SOURCE_DIR})

    set(${OUT_TARGET_BIN_DIR} "${TARGET_BIN_DIR}" PARENT_SCOPE)
    set(${OUT_HEADER_DIR} "${GENERATED_HEADER_DIR}" PARENT_SCOPE)
    set(${OUT_SOURCE_DIR} "${GENERATED_SOURCE_DIR}" PARENT_SCOPE)
endfunction()

function(_vclib_embedded_program_collect_shader_info PROGRAM_TYPE FILES OUT_SHADER_PATH OUT_VS_NAME OUT_FS_NAME OUT_CS_NAME)
    set(SHADER_PATH "")
    set(VS_NAME "")
    set(FS_NAME "")
    set(CS_NAME "")

    if (PROGRAM_TYPE STREQUAL "VF")
        list(LENGTH FILES NUM_FILES)
        if (NUM_FILES GREATER 2)
            message(FATAL_ERROR "vclib_add_embedded_program: VERT_FRAG programs require exactly 2 files (vertex and fragment)")
        endif()

        list(GET FILES 0 VERTEX_SHADER_FILE)
        list(GET FILES 1 FRAGMENT_SHADER_FILE)

        get_filename_component(VS_NAME ${VERTEX_SHADER_FILE} NAME_WE)
        get_filename_component(FS_NAME ${FRAGMENT_SHADER_FILE} NAME_WE)

        get_filename_component(SHADER_DIR ${VERTEX_SHADER_FILE} DIRECTORY)
    else()
        list(GET FILES 0 COMPUTE_SHADER_FILE)
        get_filename_component(CS_NAME ${COMPUTE_SHADER_FILE} NAME_WE)

        get_filename_component(SHADER_DIR ${COMPUTE_SHADER_FILE} DIRECTORY)
    endif()

    get_filename_component(ABSOLUTE_SHADER_DIR ${SHADER_DIR} ABSOLUTE)
    file(RELATIVE_PATH SHADER_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${ABSOLUTE_SHADER_DIR})

    set(${OUT_SHADER_PATH} "${SHADER_PATH}" PARENT_SCOPE)
    set(${OUT_VS_NAME} "${VS_NAME}" PARENT_SCOPE)
    set(${OUT_FS_NAME} "${FS_NAME}" PARENT_SCOPE)
    set(${OUT_CS_NAME} "${CS_NAME}" PARENT_SCOPE)
endfunction()

function(_vclib_embedded_program_ensure_template_exists TEMPLATE_PATH)
    if (NOT EXISTS ${TEMPLATE_PATH})
        message(FATAL_ERROR "Template file not found: ${TEMPLATE_PATH}")
    endif()
endfunction()

function(_vclib_embedded_program_update_main_header PROGRAM_TYPE PROGRAM_TYPE_LC TARGET_BIN_DIR ENUM_NAME_LC)
    set(MAIN_HEADER_FILE "${TARGET_BIN_DIR}/include/vclib/bgfx/programs/embedded_${PROGRAM_TYPE_LC}_programs.h")

    set(INCLUDES_LIST "")
    if (EXISTS ${MAIN_HEADER_FILE})
        file(READ ${MAIN_HEADER_FILE} EXISTING_HEADER_CONTENT)
        string(REGEX MATCHALL "#include \"[^\"]+\"" EXISTING_INCLUDES "${EXISTING_HEADER_CONTENT}")
        foreach(INC ${EXISTING_INCLUDES})
            list(FIND INCLUDES_LIST "${INC}" INC_INDEX)
            if (INC_INDEX EQUAL -1)
                list(APPEND INCLUDES_LIST "${INC}")
            endif()
        endforeach()
    endif()

    set(NEW_INCLUDE "#include \"embedded_${PROGRAM_TYPE_LC}_programs/${ENUM_NAME_LC}.h\"")
    list(FIND INCLUDES_LIST "${NEW_INCLUDE}" NEW_INC_INDEX)
    if (NEW_INC_INDEX EQUAL -1)
        list(APPEND INCLUDES_LIST "${NEW_INCLUDE}")
    endif()

    list(JOIN INCLUDES_LIST "\n" INCLUDES_STR)

    if (PROGRAM_TYPE STREQUAL "VF")
        set(VF_INCLUDES "${INCLUDES_STR}")
        configure_file(
            "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/embedded_vf_programs.h.in"
            ${MAIN_HEADER_FILE}
            @ONLY
        )
    else()
        set(C_INCLUDES "${INCLUDES_STR}")
        configure_file(
            "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/embedded_c_programs.h.in"
            ${MAIN_HEADER_FILE}
            @ONLY
        )
    endif()
endfunction()

function(_vclib_embedded_program_update_enum_header PROGRAM_TYPE TARGET_BIN_DIR ENUM_NAME)
    if (PROGRAM_TYPE STREQUAL "VF")
        set(ENUM_HEADER_FILE "${TARGET_BIN_DIR}/include/vclib/bgfx/programs/vert_frag_program.h")
    else()
        set(ENUM_HEADER_FILE "${TARGET_BIN_DIR}/include/vclib/bgfx/programs/compute_program.h")
    endif()

    set(EXISTING_ENUM_ENTRIES "")
    if (EXISTS ${ENUM_HEADER_FILE})
        file(READ ${ENUM_HEADER_FILE} EXISTING_ENUM_CONTENT)
        string(REGEX MATCHALL "    [A-Z_][A-Z0-9_]*," EXISTING_ENUM_ENTRIES "${EXISTING_ENUM_CONTENT}")
    endif()

    set(NEW_ENUM_ENTRY "    ${ENUM_NAME},")
    list(FIND EXISTING_ENUM_ENTRIES "${NEW_ENUM_ENTRY}" ENTRY_FOUND)
    if (ENTRY_FOUND EQUAL -1)
        if (EXISTING_ENUM_ENTRIES STREQUAL "")
            set(EXISTING_ENUM_ENTRIES "${NEW_ENUM_ENTRY}")
        else()
            list(APPEND EXISTING_ENUM_ENTRIES "${NEW_ENUM_ENTRY}")
        endif()
    endif()

    list(JOIN EXISTING_ENUM_ENTRIES "\n" ENUM_ENTRIES_STR)

    if (PROGRAM_TYPE STREQUAL "VF")
        set(VF_ENUM_ENTRIES "${ENUM_ENTRIES_STR}")
        configure_file(
            "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/embedded_vf_programs/vert_frag_program_enum.h.in"
            ${ENUM_HEADER_FILE}
            @ONLY
        )
    else()
        set(C_ENUM_ENTRIES "${ENUM_ENTRIES_STR}")
        configure_file(
            "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/embedded_c_programs/compute_program_enum.h.in"
            ${ENUM_HEADER_FILE}
            @ONLY
        )
    endif()
endfunction()

# vclib_add_embedded_program(
#   ENUM_NAME <enum_name>
#   TYPE [VERT_FRAG|COMPUTE]
#   FILES <file1> [<file2> ...]
# )
# Function to add an embedded program that will be generated at CMake configure time.
# This function generates the header and source files for embedded shaders in the build directory.
function(vclib_add_embedded_program)
    # Parse arguments
    set(options)
    set(oneValueArgs ENUM_NAME TYPE)
    set(multiValueArgs FILES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")

    _vclib_embedded_program_validate_args("${ARG_ENUM_NAME}" "${ARG_TYPE}" "${ARG_FILES}")
    _vclib_embedded_program_resolve_type("${ARG_TYPE}" PROGRAM_TYPE PROGRAM_TYPE_LC)
    _vclib_embedded_program_prepare_output_dirs(
        "${PROGRAM_TYPE_LC}"
        TARGET_BIN_DIR
        GENERATED_HEADER_DIR
        GENERATED_SOURCE_DIR
    )

    # Get the lowercase enum name for naming files
    string(TOLOWER ${ARG_ENUM_NAME} ENUM_NAME_LC)

    _vclib_embedded_program_collect_shader_info(
        "${PROGRAM_TYPE}"
        "${ARG_FILES}"
        SHADER_PATH
        VERTEX_SHADER_NAME
        FRAGMENT_SHADER_NAME
        COMPUTE_SHADER_NAME
    )

    # Generate header file using configure_file(@ONLY) to preserve semicolons
    set(HEADER_TEMPLATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/embedded_${PROGRAM_TYPE_LC}_programs/header.h.in")
    set(HEADER_OUTPUT "${GENERATED_HEADER_DIR}/${ENUM_NAME_LC}.h")

    _vclib_embedded_program_ensure_template_exists(${HEADER_TEMPLATE})

    # Set variables for configure_file(@ONLY) - must use @VAR@ syntax in templates
    set(PR_NAME_UC ${ARG_ENUM_NAME})
    set(PR_NAME_LC ${ENUM_NAME_LC})
    set(PR_PATH ${SHADER_PATH})

    if (PROGRAM_TYPE STREQUAL "VF")
        set(PR_VS_NAME ${VERTEX_SHADER_NAME})
        set(PR_FS_NAME ${FRAGMENT_SHADER_NAME})
        set(PR_CS_NAME "")
    else()
        set(PR_CS_NAME ${COMPUTE_SHADER_NAME})
        set(PR_VS_NAME "")
        set(PR_FS_NAME "")
    endif()

    configure_file(${HEADER_TEMPLATE} ${HEADER_OUTPUT} @ONLY)

    # Generate source file using configure_file(@ONLY) to preserve semicolons
    set(SOURCE_TEMPLATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/templates/embedded_${PROGRAM_TYPE_LC}_programs/source.cpp.in")
    set(SOURCE_OUTPUT "${GENERATED_SOURCE_DIR}/${ENUM_NAME_LC}.cpp")

    _vclib_embedded_program_ensure_template_exists(${SOURCE_TEMPLATE})

    configure_file(${SOURCE_TEMPLATE} ${SOURCE_OUTPUT} @ONLY)

    _vclib_embedded_program_update_main_header(
        "${PROGRAM_TYPE}"
        "${PROGRAM_TYPE_LC}"
        "${TARGET_BIN_DIR}"
        "${ENUM_NAME_LC}"
    )

    _vclib_embedded_program_update_enum_header(
        "${PROGRAM_TYPE}"
        "${TARGET_BIN_DIR}"
        "${ARG_ENUM_NAME}"
    )

    # Add generated files to vclib::render target
    target_sources(vclib-render PRIVATE ${HEADER_OUTPUT} ${SOURCE_OUTPUT})

endfunction()

# vclib_add_embedded_programs_from_file(<file_path> <TYPE>)
# Reads a .config file and adds all embedded programs listed in it.
# The config file format is:
#   ENUM_NAME
#       <shader_path1>
#       <shader_path2>
#
# Lines with both '/' and '.' are treated as file paths, others as ENUM names.
# TYPE must be VERT_FRAG or COMPUTE
function(vclib_add_embedded_programs_from_file FILE_PATH TYPE)
    if(NOT EXISTS ${FILE_PATH})
        message(WARNING "Embedded programs config file not found: ${FILE_PATH}")
        return()
    endif()

    string(FIND "${FILE_PATH}" "${CMAKE_CURRENT_BINARY_DIR}" IS_IN_BIN_DIR)
    if(IS_IN_BIN_DIR EQUAL 0)
        source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} PREFIX "Shaders" FILES ${FILE_PATH})
    else()
        source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Shaders" FILES ${FILE_PATH})
    endif()
    target_sources(vclib-render PRIVATE ${FILE_PATH})

    # Read file line by line using file(STRINGS ...) which returns a proper CMake list
    file(STRINGS ${FILE_PATH} FILE_LINES)

    set(CURRENT_ENUM_NAME "")
    set(CURRENT_FILES_LIST "")
    set(FILE_COUNT 0)

    foreach(LINE ${FILE_LINES})
        string(STRIP "${LINE}" STRIPPED_LINE)

        # Skip empty lines - they signal end of current entry
        if("${STRIPPED_LINE}" STREQUAL "")
            if(NOT CURRENT_ENUM_NAME STREQUAL "" AND FILE_COUNT GREATER 0)
                vclib_add_embedded_program(
                    ENUM_NAME ${CURRENT_ENUM_NAME}
                    TYPE ${TYPE}
                    FILES ${CURRENT_FILES_LIST}
                )
                # Reset for next entry
                set(CURRENT_ENUM_NAME "")
                set(CURRENT_FILES_LIST "")
                set(FILE_COUNT 0)
            endif()
            continue()
        endif()

        # Skip comments
        if("${STRIPPED_LINE}" MATCHES "^#")
            continue()
        endif()

        # Check if this line is a file path (contains both '/' and '.')
        string(FIND "${STRIPPED_LINE}" "/" HAS_SLASH)
        string(FIND "${STRIPPED_LINE}" "." HAS_DOT)

        if(${HAS_SLASH} GREATER -1 AND ${HAS_DOT} GREATER -1)
            # This is a file path
            list(APPEND CURRENT_FILES_LIST "${STRIPPED_LINE}")
            list(LENGTH CURRENT_FILES_LIST FILE_COUNT)
        else()
            # This is an ENUM_NAME - finalize current entry if any
            if(NOT CURRENT_ENUM_NAME STREQUAL "" AND FILE_COUNT GREATER 0)
                vclib_add_embedded_program(
                    ENUM_NAME ${CURRENT_ENUM_NAME}
                    TYPE ${TYPE}
                    FILES ${CURRENT_FILES_LIST}
                )
            endif()
            set(CURRENT_ENUM_NAME "${STRIPPED_LINE}")
            set(CURRENT_FILES_LIST "")
            set(FILE_COUNT 0)
        endif()
    endforeach()

    # Process the last entry (file may not end with blank line)
    if(NOT CURRENT_ENUM_NAME STREQUAL "" AND FILE_COUNT GREATER 0)
        vclib_add_embedded_program(
            ENUM_NAME ${CURRENT_ENUM_NAME}
            TYPE ${TYPE}
            FILES ${CURRENT_FILES_LIST}
        )
    endif()
endfunction()
