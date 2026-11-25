# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

# Writes file only if content differs, avoiding timestamp updates that trigger recompilations.
function(_vclib_write_file_if_different FILE_PATH CONTENT)
    if(EXISTS "${FILE_PATH}")
        file(READ "${FILE_PATH}" EXISTING_CONTENT)
        if("${EXISTING_CONTENT}" STREQUAL "${CONTENT}")
            return()
        endif()
    endif()
    file(WRITE "${FILE_PATH}" "${CONTENT}")
endfunction()

# Formats an integer index into a 4-digit zero-padded string (e.g. 7 -> "0007").
function(_vclib_format_4digit_id INDEX OUT_VAR)
    string(LENGTH "${INDEX}" _LEN)
    math(EXPR _PAD_LEN "4 - ${_LEN}")
    set(_PAD "")
    if(_PAD_LEN GREATER 0)
        string(SUBSTRING "0000" 0 ${_PAD_LEN} _PAD)
    endif()
    set(${OUT_VAR} "${_PAD}${INDEX}" PARENT_SCOPE)
endfunction()

##
# @brief Recursively generates shader files for all combinations of the remaining dimensions.
#
# This function is used internally to recursively build combinations of macro definitions,
# generate the corresponding fragment shader (.sc) files, and append the combinations to the combo registry.
#
# @param DIMS_REMAINING A list of dimensions yet to be processed.
function(
    vclib_generate_shader_combinations_recursive
    DIMS_REMAINING
    PROGRAM_SUFFIX
    VS_COMBINATION
    FS_COMBINATION
    VS_DEFINES
    FS_DEFINES
    CONFIG_VARS
)
    list(LENGTH DIMS_REMAINING NUM_DIMS)

    # Base case: all dimensions have been processed -> generate the leaf shaders and register the combination
    if(NUM_DIMS EQUAL 0)
        # CONFIG_VARS format: CONFIG_DIR;FS_IN;FS_PREFIX;DEFINE_PREFIX;COMBO_OUT_FILE;ENUM_PREFIX;VS_FILE;VS_IN;VS_PREFIX;FS_FILE;BIN_CONFIG_DIR
        list(GET CONFIG_VARS 0 CONFIG_DIR)
        list(GET CONFIG_VARS 1 FS_IN)
        list(GET CONFIG_VARS 2 FS_PREFIX)
        list(GET CONFIG_VARS 3 DEFINE_PREFIX)
        list(GET CONFIG_VARS 4 COMBO_OUT_FILE)
        list(GET CONFIG_VARS 5 ENUM_PREFIX)
        list(GET CONFIG_VARS 6 VS_FILE)
        list(GET CONFIG_VARS 7 VS_IN)
        list(GET CONFIG_VARS 8 VS_PREFIX)
        list(GET CONFIG_VARS 9 FS_FILE)
        list(GET CONFIG_VARS 10 BIN_CONFIG_DIR)

        string(TOLOWER "${PROGRAM_SUFFIX}" PROGRAM_SUFFIX_LC)
        string(TOUPPER "${PROGRAM_SUFFIX}" PROGRAM_SUFFIX_UC)

        get_property(VCLIB_RUN_ID GLOBAL PROPERTY VCLIB_RUN_ID)

        # Assign a 4-digit ID for this program combination (used for compact .cpp/.h filenames, e.g. drawable_mesh_surface_0000)
        set(PROG_INDEX_KEY
            "VCLIB_R${VCLIB_RUN_ID}_PROG_INDEX_${BIN_CONFIG_DIR}_${ENUM_PREFIX}"
        )
        get_property(PROG_INDEX GLOBAL PROPERTY "${PROG_INDEX_KEY}")
        if("${PROG_INDEX}" STREQUAL "")
            set(PROG_INDEX 0)
        endif()
        _vclib_format_4digit_id("${PROG_INDEX}" PROG_ID)
        math(EXPR NEXT_PROG_INDEX "${PROG_INDEX} + 1")
        set_property(GLOBAL PROPERTY "${PROG_INDEX_KEY}" "${NEXT_PROG_INDEX}")

        string(TOLOWER "${ENUM_PREFIX}" ENUM_PREFIX_LC)
        set(PROG_FILE_NAME "${ENUM_PREFIX_LC}_${PROG_ID}")

        # --- Vertex Shader Handling ---
        if(VS_FILE)
            # Static VS: compute relative path from CMAKE_CURRENT_SOURCE_DIR/shaders
            file(
                RELATIVE_PATH
                VS_REL_PATH
                "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
                "${CONFIG_DIR}/${VS_FILE}"
            )
            string(
                REPLACE "vclib/bgfx/"
                "vclib/shaders/"
                VS_REL_PATH
                "${VS_REL_PATH}"
            )
        else()
            # Dynamic VS: reuse existing ID if this combination was already generated, otherwise assign the next 4-digit ID
            set(VS_MAP_KEY
                "VCLIB_R${VCLIB_RUN_ID}_VS_ID_${BIN_CONFIG_DIR}_${ENUM_PREFIX}_${VS_PREFIX}_${VS_COMBINATION}"
            )
            get_property(VS_ID GLOBAL PROPERTY "${VS_MAP_KEY}")
            if("${VS_ID}" STREQUAL "")
                set(VS_INDEX_KEY
                    "VCLIB_R${VCLIB_RUN_ID}_VS_INDEX_${BIN_CONFIG_DIR}_${ENUM_PREFIX}_${VS_PREFIX}"
                )
                get_property(VS_INDEX GLOBAL PROPERTY "${VS_INDEX_KEY}")
                if("${VS_INDEX}" STREQUAL "")
                    set(VS_INDEX 0)
                endif()
                _vclib_format_4digit_id("${VS_INDEX}" VS_ID)
                math(EXPR NEXT_VS_INDEX "${VS_INDEX} + 1")
                set_property(
                    GLOBAL
                    PROPERTY "${VS_INDEX_KEY}" "${NEXT_VS_INDEX}"
                )
                set_property(GLOBAL PROPERTY "${VS_MAP_KEY}" "${VS_ID}")
            endif()

            set(VS_OUT_NAME "${VS_PREFIX}_${VS_ID}.sc")
            set(VS_OUT_PATH "${BIN_CONFIG_DIR}/${VS_OUT_NAME}")

            # Write generated VS with macro defines and base file inclusion (only if content changed)
            set(VS_CONTENT
                "// Auto-generated by vclib_generate_shaders_from_config\n"
            )
            if(NOT "${VS_COMBINATION}" STREQUAL "")
                string(APPEND VS_CONTENT "// Combination: ${VS_COMBINATION}\n")
            endif()
            string(APPEND VS_CONTENT "${VS_DEFINES}\n")
            string(APPEND VS_CONTENT "#include \"${VS_IN}\"\n")
            _vclib_write_file_if_different("${VS_OUT_PATH}" "${VS_CONTENT}")
            set_property(
                GLOBAL
                APPEND
                PROPERTY VCLIB_GENERATED_SC_FILES "${VS_OUT_PATH}"
            )

            file(
                RELATIVE_PATH
                VS_REL_PATH
                "${CMAKE_CURRENT_BINARY_DIR}/shaders"
                "${VS_OUT_PATH}"
            )
            string(
                REPLACE "vclib/bgfx/"
                "vclib/shaders/"
                VS_REL_PATH
                "${VS_REL_PATH}"
            )
        endif()

        # --- Fragment Shader Handling ---
        if(FS_FILE)
            # Static FS: compute relative path from CMAKE_CURRENT_SOURCE_DIR/shaders
            file(
                RELATIVE_PATH
                FS_REL_PATH
                "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
                "${CONFIG_DIR}/${FS_FILE}"
            )
            string(
                REPLACE "vclib/bgfx/"
                "vclib/shaders/"
                FS_REL_PATH
                "${FS_REL_PATH}"
            )
        else()
            # Dynamic FS: reuse existing ID if this combination was already generated, otherwise assign the next 4-digit ID
            set(FS_MAP_KEY
                "VCLIB_R${VCLIB_RUN_ID}_FS_ID_${BIN_CONFIG_DIR}_${ENUM_PREFIX}_${FS_PREFIX}_${FS_COMBINATION}"
            )
            get_property(FS_ID GLOBAL PROPERTY "${FS_MAP_KEY}")
            if("${FS_ID}" STREQUAL "")
                set(FS_INDEX_KEY
                    "VCLIB_R${VCLIB_RUN_ID}_FS_INDEX_${BIN_CONFIG_DIR}_${ENUM_PREFIX}_${FS_PREFIX}"
                )
                get_property(FS_INDEX GLOBAL PROPERTY "${FS_INDEX_KEY}")
                if("${FS_INDEX}" STREQUAL "")
                    set(FS_INDEX 0)
                endif()
                _vclib_format_4digit_id("${FS_INDEX}" FS_ID)
                math(EXPR NEXT_FS_INDEX "${FS_INDEX} + 1")
                set_property(
                    GLOBAL
                    PROPERTY "${FS_INDEX_KEY}" "${NEXT_FS_INDEX}"
                )
                set_property(GLOBAL PROPERTY "${FS_MAP_KEY}" "${FS_ID}")
            endif()

            set(FS_OUT_NAME "${FS_PREFIX}_${FS_ID}.sc")
            set(FS_OUT_PATH "${BIN_CONFIG_DIR}/${FS_OUT_NAME}")

            # Write generated FS with macro defines and base file inclusion (only if content changed)
            set(FS_CONTENT
                "// Auto-generated by vclib_generate_shaders_from_config\n"
            )
            if(NOT "${FS_COMBINATION}" STREQUAL "")
                string(APPEND FS_CONTENT "// Combination: ${FS_COMBINATION}\n")
            endif()
            string(APPEND FS_CONTENT "${FS_DEFINES}\n")
            string(APPEND FS_CONTENT "#include \"${FS_IN}\"\n")
            _vclib_write_file_if_different("${FS_OUT_PATH}" "${FS_CONTENT}")
            set_property(
                GLOBAL
                APPEND
                PROPERTY VCLIB_GENERATED_SC_FILES "${FS_OUT_PATH}"
            )

            file(
                RELATIVE_PATH
                FS_REL_PATH
                "${CMAKE_CURRENT_BINARY_DIR}/shaders"
                "${FS_OUT_PATH}"
            )
            string(
                REPLACE "vclib/bgfx/"
                "vclib/shaders/"
                FS_REL_PATH
                "${FS_REL_PATH}"
            )
        endif()

        # Append combination entry to registry buffer: ENUM_NAME [FILE_NAME] \n VS_PATH \n FS_PATH
        set_property(
            GLOBAL
            APPEND_STRING
            PROPERTY
                VCLIB_COMBO_CONFIG_CONTENT
                    "${ENUM_PREFIX}${PROGRAM_SUFFIX_UC} ${PROG_FILE_NAME}\n    ${VS_REL_PATH}\n    ${FS_REL_PATH}\n\n"
        )

        return()
    endif()

    # Recursive step: pop the first dimension and branch on each of its values
    list(GET DIMS_REMAINING 0 FIRST_DIM)
    list(REMOVE_AT DIMS_REMAINING 0)

    set(VALUES "${VALUES_${FIRST_DIM}}")
    set(DIM_TYPE "${DIM_TYPE_${FIRST_DIM}}")
    list(GET CONFIG_VARS 3 DEFINE_PREFIX)

    foreach(VAL IN LISTS VALUES)
        string(TOLOWER "${FIRST_DIM}" DIM_LC)
        string(TOLOWER "${VAL}" VAL_LC)
        string(TOUPPER "${FIRST_DIM}" DIM_UC)
        string(TOUPPER "${VAL}" VAL_UC)

        # Suffix for the C++ enum identifier
        set(NEW_PROGRAM_SUFFIX "${PROGRAM_SUFFIX}_${DIM_LC}_${VAL_LC}")
        set(DEFINE_STR "#define ${DEFINE_PREFIX}_${DIM_UC}_${VAL_UC} 1\n")

        set(NEW_VS_COMBINATION "${VS_COMBINATION}")
        set(NEW_FS_COMBINATION "${FS_COMBINATION}")
        set(NEW_VS_DEFINES "${VS_DEFINES}")
        set(NEW_FS_DEFINES "${FS_DEFINES}")

        # Append macro define to Vertex Shader if applicable
        if(DIM_TYPE STREQUAL "VS" OR DIM_TYPE STREQUAL "BOTH")
            set(NEW_VS_COMBINATION "${VS_COMBINATION}_${DIM_LC}_${VAL_LC}")
            set(NEW_VS_DEFINES "${VS_DEFINES}${DEFINE_STR}")
        endif()

        # Append macro define to Fragment Shader if applicable
        if(DIM_TYPE STREQUAL "FS" OR DIM_TYPE STREQUAL "BOTH")
            set(NEW_FS_COMBINATION "${FS_COMBINATION}_${DIM_LC}_${VAL_LC}")
            set(NEW_FS_DEFINES "${FS_DEFINES}${DEFINE_STR}")
        endif()

        # Recurse for the remaining dimensions
        vclib_generate_shader_combinations_recursive(
            "${DIMS_REMAINING}"
            "${NEW_PROGRAM_SUFFIX}"
            "${NEW_VS_COMBINATION}"
            "${NEW_FS_COMBINATION}"
            "${NEW_VS_DEFINES}"
            "${NEW_FS_DEFINES}"
            "${CONFIG_VARS}"
        )
    endforeach()
endfunction()

##
# @brief Parses a shader configuration file to generate shader permutations.
#
# Reads the given `.config` file to extract dimensions (e.g., SHADING, COLOR), their values,
# and prefixes. It generates a new fragment shader file for each possible combination of the
# dimensions, and appends the new configuration entry to the `embedded_vf_combo_programs.config` registry.
#
# A single `.config` file can contain multiple configuration blocks separated by `ENUM_PREFIX` lines.
# Each block will independently generate its permutations and append them to the registry.
#
# @param CONFIG_FILE The absolute path to the `.config` file to be parsed.
function(vclib_generate_shaders_from_config CONFIG_FILE)
    if(NOT EXISTS "${CONFIG_FILE}")
        message(FATAL_ERROR "Config file not found: ${CONFIG_FILE}")
    endif()

    # Register dependency so CMake re-runs automatically when this config file is edited
    set_property(
        DIRECTORY
        APPEND
        PROPERTY CMAKE_CONFIGURE_DEPENDS "${CONFIG_FILE}"
    )

    get_filename_component(CONFIG_DIR "${CONFIG_FILE}" DIRECTORY)

    # Mirror the config file's source folder structure in the binary output directory
    file(
        RELATIVE_PATH
        CONFIG_REL_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CONFIG_DIR}"
    )
    set(BIN_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_REL_PATH}")
    file(MAKE_DIRECTORY "${BIN_CONFIG_DIR}")

    set(COMBO_OUT_FILE
        "${CMAKE_CURRENT_BINARY_DIR}/shaders/embedded_vf_combo_programs.config"
    )

    # Append header comment to registry property
    set_property(
        GLOBAL
        APPEND_STRING
        PROPERTY
            VCLIB_COMBO_CONFIG_CONTENT
                "# Auto-generated combinations from ${CONFIG_FILE}\n\n"
    )

    # Initialize variables for the active combo block
    set(ENUM_PREFIX "")
    set(DEFINE_PREFIX "")
    set(VS_FILE "")
    set(FS_FILE "")
    set(FS_IN "")
    set(FS_PREFIX "")
    set(VS_IN "")
    set(VS_PREFIX "")

    set(DIMS "")
    set(CURRENT_DIM "")

    file(STRINGS "${CONFIG_FILE}" FILE_LINES)

    # Line-by-line parser for .config files
    foreach(LINE IN LISTS FILE_LINES)
        # Skip empty or whitespace-only lines
        if("${LINE}" MATCHES "^[ \t]*$")
            set(CURRENT_DIM "")
            continue()
        endif()

        # Skip comments
        if("${LINE}" MATCHES "^[ \t]*#")
            continue()
        endif()

        # Lines with leading whitespace define allowed values for CURRENT_DIM
        if("${LINE}" MATCHES "^[ \t]+([^ \t].*)")
            string(STRIP "${LINE}" VAL)
            if(NOT "${CURRENT_DIM}" STREQUAL "")
                list(APPEND VALUES_${CURRENT_DIM} "${VAL}")
            else()
                message(
                    WARNING
                    "Found value '${VAL}' outside of any dimension in ${CONFIG_FILE}"
                )
            endif()
        else()
            string(STRIP "${LINE}" STRIPPED_LINE)

            # ENUM_PREFIX marks the start of a block; if a previous block exists, generate it first
            if("${STRIPPED_LINE}" MATCHES "^ENUM_PREFIX (.+)")
                if(NOT "${ENUM_PREFIX}" STREQUAL "")
                    if(NOT FS_FILE AND (NOT FS_IN OR NOT FS_PREFIX))
                        message(
                            FATAL_ERROR
                            "Must provide either FS_FILE or (FS_IN and FS_PREFIX) in ${CONFIG_FILE}"
                        )
                    endif()
                    if(NOT VS_FILE AND (NOT VS_IN OR NOT VS_PREFIX))
                        message(
                            FATAL_ERROR
                            "Must provide either VS_FILE or (VS_IN and VS_PREFIX) in ${CONFIG_FILE}"
                        )
                    endif()

                    set(CONFIG_VARS
                        "${CONFIG_DIR}"
                        "${FS_IN}"
                        "${FS_PREFIX}"
                        "${DEFINE_PREFIX}"
                        "${COMBO_OUT_FILE}"
                        "${ENUM_PREFIX}"
                        "${VS_FILE}"
                        "${VS_IN}"
                        "${VS_PREFIX}"
                        "${FS_FILE}"
                        "${BIN_CONFIG_DIR}"
                    )

                    # Generate permutations for the completed block
                    vclib_generate_shader_combinations_recursive(
                        "${DIMS}"
                        ""
                        ""
                        ""
                        ""
                        ""
                        "${CONFIG_VARS}"
                    )

                    # Reset variables for the new block
                    set(DEFINE_PREFIX "")
                    set(VS_FILE "")
                    set(FS_FILE "")
                    set(FS_IN "")
                    set(FS_PREFIX "")
                    set(VS_IN "")
                    set(VS_PREFIX "")
                    set(DIMS "")
                    set(CURRENT_DIM "")
                endif()
                set(ENUM_PREFIX "${CMAKE_MATCH_1}")
                # Directive: prefix for #define macros (e.g. SURFACE -> #define SURFACE_SHADING_FLAT 1)
            elseif("${STRIPPED_LINE}" MATCHES "^DEFINE_PREFIX (.+)")
                set(DEFINE_PREFIX "${CMAKE_MATCH_1}")
                # Directives: static vertex/fragment shader files
            elseif("${STRIPPED_LINE}" MATCHES "^VS_FILE (.+)")
                set(VS_FILE "${CMAKE_MATCH_1}")
                # Directives: dynamic vertex shader template input and output prefix
            elseif("${STRIPPED_LINE}" MATCHES "^VS_IN (.+)")
                set(VS_IN "${CMAKE_MATCH_1}")
            elseif("${STRIPPED_LINE}" MATCHES "^VS_PREFIX (.+)")
                set(VS_PREFIX "${CMAKE_MATCH_1}")
                # Directives: static fragment shader file
            elseif("${STRIPPED_LINE}" MATCHES "^FS_FILE (.+)")
                set(FS_FILE "${CMAKE_MATCH_1}")
                # Directives: dynamic fragment shader template input and output prefix
            elseif("${STRIPPED_LINE}" MATCHES "^FS_IN (.+)")
                set(FS_IN "${CMAKE_MATCH_1}")
            elseif("${STRIPPED_LINE}" MATCHES "^FS_PREFIX (.+)")
                set(FS_PREFIX "${CMAKE_MATCH_1}")
                # Directives: declare a dimension applying to VS only, FS only, or BOTH
            elseif("${STRIPPED_LINE}" MATCHES "^DIM_VS (.+)")
                set(CURRENT_DIM "${CMAKE_MATCH_1}")
                list(APPEND DIMS "${CURRENT_DIM}")
                set(DIM_TYPE_${CURRENT_DIM} "VS")
                set(VALUES_${CURRENT_DIM} "")
            elseif("${STRIPPED_LINE}" MATCHES "^DIM_FS (.+)")
                set(CURRENT_DIM "${CMAKE_MATCH_1}")
                list(APPEND DIMS "${CURRENT_DIM}")
                set(DIM_TYPE_${CURRENT_DIM} "FS")
                set(VALUES_${CURRENT_DIM} "")
            elseif("${STRIPPED_LINE}" MATCHES "^DIM_BOTH (.+)")
                set(CURRENT_DIM "${CMAKE_MATCH_1}")
                list(APPEND DIMS "${CURRENT_DIM}")
                set(DIM_TYPE_${CURRENT_DIM} "BOTH")
                set(VALUES_${CURRENT_DIM} "")
            else()
                message(
                    FATAL_ERROR
                    "Unrecognized line or missing DIM_ prefix: ${STRIPPED_LINE}"
                )
            endif()
        endif()
    endforeach()

    # Validate and generate the final block at end of file
    if(NOT FS_FILE AND (NOT FS_IN OR NOT FS_PREFIX))
        message(
            FATAL_ERROR
            "Must provide either FS_FILE or (FS_IN and FS_PREFIX) in ${CONFIG_FILE}"
        )
    endif()
    if(NOT VS_FILE AND (NOT VS_IN OR NOT VS_PREFIX))
        message(
            FATAL_ERROR
            "Must provide either VS_FILE or (VS_IN and VS_PREFIX) in ${CONFIG_FILE}"
        )
    endif()

    set(CONFIG_VARS
        "${CONFIG_DIR}"
        "${FS_IN}"
        "${FS_PREFIX}"
        "${DEFINE_PREFIX}"
        "${COMBO_OUT_FILE}"
        "${ENUM_PREFIX}"
        "${VS_FILE}"
        "${VS_IN}"
        "${VS_PREFIX}"
        "${FS_FILE}"
        "${BIN_CONFIG_DIR}"
    )

    vclib_generate_shader_combinations_recursive(
        "${DIMS}"
        ""
        ""
        ""
        ""
        ""
        "${CONFIG_VARS}"
    )

    message(STATUS "Generated shaders from ${CONFIG_FILE}")

    # Propagate the processed config file path so IDE targets can show it
    set(VCLIB_GENERATED_SHADER_CONFIGS
        ${VCLIB_GENERATED_SHADER_CONFIGS}
        "${CONFIG_FILE}"
        PARENT_SCOPE
    )
endfunction()

##
# @brief Finds and processes all shader `.config` files in a given directory.
#
# Recursively searches for `.config` files within `DIR_PATH`, initializes the common output
# combination registry `embedded_vf_combo_programs.config`, and calls `vclib_generate_shaders_from_config`
# on each found file.
#
# @param DIR_PATH The absolute path to the directory containing shader `.config` files.
function(vclib_generate_all_shaders_from_configs DIR_PATH)
    set(COMBO_OUT_FILE
        "${CMAKE_CURRENT_BINARY_DIR}/shaders/embedded_vf_combo_programs.config"
    )

    # Increment configure run counter to isolate global combo/shader mappings between reconfigures
    get_property(VCLIB_RUN_ID GLOBAL PROPERTY VCLIB_RUN_ID)
    if("${VCLIB_RUN_ID}" STREQUAL "")
        set(VCLIB_RUN_ID 0)
    endif()
    math(EXPR VCLIB_RUN_ID "${VCLIB_RUN_ID} + 1")
    set_property(GLOBAL PROPERTY VCLIB_RUN_ID "${VCLIB_RUN_ID}")

    # Output directory for registry and generated shaders
    get_filename_component(COMBO_OUT_DIR "${COMBO_OUT_FILE}" DIRECTORY)

    # Initialize global accumulation buffers for this configure run
    set_property(
        GLOBAL
        PROPERTY VCLIB_COMBO_CONFIG_CONTENT "# Auto-generated combinations\n\n"
    )
    set_property(GLOBAL PROPERTY VCLIB_GENERATED_SC_FILES "")

    file(MAKE_DIRECTORY "${COMBO_OUT_DIR}")

    # Discover and process all .config files in the shaders directory
    file(GLOB_RECURSE ALL_CONFIGS "${DIR_PATH}/*.config")
    foreach(CONFIG_FILE ${ALL_CONFIGS})
        vclib_generate_shaders_from_config("${CONFIG_FILE}")
    endforeach()

    # Write the consolidated combination registry (only writes to disk if content changed)
    get_property(FINAL_COMBO_CONTENT GLOBAL PROPERTY VCLIB_COMBO_CONFIG_CONTENT)
    _vclib_write_file_if_different("${COMBO_OUT_FILE}" "${FINAL_COMBO_CONTENT}")

    # Clean up obsolete/orphaned generated .sc files from deleted combinations or previous builds
    file(GLOB_RECURSE EXISTING_SC_FILES "${COMBO_OUT_DIR}/*.sc")
    get_property(CURRENT_GENERATED_SC GLOBAL PROPERTY VCLIB_GENERATED_SC_FILES)
    foreach(EXISTING_FILE ${EXISTING_SC_FILES})
        if(NOT "${EXISTING_FILE}" IN_LIST CURRENT_GENERATED_SC)
            file(REMOVE "${EXISTING_FILE}")
        endif()
    endforeach()

    # Propagate the collected config files to parent scope for IDE source groups
    set(VCLIB_GENERATED_SHADER_CONFIGS
        ${VCLIB_GENERATED_SHADER_CONFIGS}
        PARENT_SCOPE
    )
endfunction()
