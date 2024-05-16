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

if (TARGET vclib-external-bgfx)
    include(${CMAKE_CURRENT_LIST_DIR}/bgfx_config.cmake)
endif()

# Function to add a list of assets to a target
function(target_ide_add_assets target_name)
    list(REMOVE_AT ARGV 0)

    source_group("Asset Files" FILES ${ARGV})
    target_sources(${target_name} PRIVATE ${ARGV})
endfunction()

function(build_assets_to_headers)
    target_ide_add_assets(vclib-render ${ARGV})

    get_property(TARGET_BIN_DIR TARGET vclib-render PROPERTY BINARY_DIR)

    set(BGFX_ASSETS_OUTPUT_DIR "${TARGET_BIN_DIR}/include")

    message(STATUS "OutDir: ${BGFX_ASSETS_OUTPUT_DIR}")
    foreach(ASSET ${ARGV})
        get_filename_component(DIR_PATH ${ASSET} DIRECTORY)
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
