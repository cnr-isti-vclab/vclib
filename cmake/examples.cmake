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

# declare a function vclib_render_add_example:
# vclib_render_add_example(<example_name> SOURCES <source_files>... [VCLIB_EXAMPLE <vclib example>])
# where:
# example_name: the name of the example
# source_files: the source files of the example
# vclib_example: the name of the vclib example associated with the example
function(vclib_render_add_example name)
    cmake_parse_arguments(ARG "" "VCLIB_EXAMPLE" "SOURCES" ${ARGN})

    set(TARGET_NAME "vclib-render-example-${name}")

    add_executable(${TARGET_NAME} ${ARG_SOURCES})
    target_link_libraries(${TARGET_NAME} PRIVATE
        vclib-render vclib-examples-common vclib-render-examples-common)

    if (ARG_VCLIB_EXAMPLE)
        set(VCLIB_INCLUDE_EXAMPLES_DIR ${VCLIB_EXAMPLES_DIR}/${ARG_VCLIB_EXAMPLE})
        target_include_directories(${TARGET_NAME} PUBLIC
                ${VCLIB_INCLUDE_EXAMPLES_DIR})
    endif()

    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples")
endfunction()
