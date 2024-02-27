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

function(vclib_render_add_example name include_vclib_example_dir)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    set(TARGET_NAME "vclib-render-${name}-example")

    add_executable(${TARGET_NAME} ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${TARGET_NAME} PRIVATE
        vclib-render vclib-examples-common)

    if (${include_vclib_example_dir})
        string(REPLACE "-" "_" out_name ${name})
        set(VCLIB_INCLUDE_EXAMPLES_DIR ${VCLIB_EXAMPLES_DIR}/${out_name})

        target_include_directories(${TARGET_NAME} PUBLIC
                ${VCLIB_INCLUDE_EXAMPLES_DIR})
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")
endfunction()
