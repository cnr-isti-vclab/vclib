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
function(target_add_assets target_name)
    list(REMOVE_AT ARGV 0)

    source_group("Asset Files" FILES ${ARGV})
    target_sources(${target_name} PRIVATE ${ARGV})
endfunction()

# Function to make available the assets defined by vclib to the given target
function(target_expose_vclib_assets target_name)
    get_property(VCLIB_ASSETS TARGET vclib-render PROPERTY VCLIB_RENDER_ASSETS)
    get_property(VCLIB_RENDER_DIR TARGET vclib-render PROPERTY VCLIB_RENDER_INCLUDE_DIR)

    foreach(ASSET ${VCLIB_ASSETS})
        get_filename_component(DIR_PATH ${ASSET} DIRECTORY)
        get_property(TARGET_BIN_DIR TARGET ${target_name} PROPERTY BINARY_DIR)

        # copy file from "${VCLIB_RENDER_DIR}/../${ASSET}" to "${TARGET_BIN_DIR}/${ASSET}"
        add_custom_command(
            TARGET ${target_name}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${VCLIB_RENDER_DIR}/../${ASSET}" "${TARGET_BIN_DIR}/${ASSET}"
            COMMENT "Copying asset ${ASSET}"
        )
    endforeach()
endfunction()

function (target_expose_vclib_assets_and_shaders target_name)
    target_expose_vclib_assets(${target_name})
    # if (TARGET vclib-external-bgfx)
    #     target_expose_vclib_bgfx_shaders(${target_name})
    # endif()
endfunction()
