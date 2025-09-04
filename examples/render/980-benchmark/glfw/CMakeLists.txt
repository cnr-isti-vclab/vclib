#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
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

cmake_minimum_required(VERSION 3.24)

set(EXAMPLE_NAME 980-benchmark)
project(vclib-render-example-${EXAMPLE_NAME})

### Build settings
set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    main.cpp)

# will create a target called 'vclib-render-example-${EXAMPLE_NAME}'
vclib_add_example(${EXAMPLE_NAME}
    VCLIB_MODULE render
    SOURCES ${SOURCES})

# after build, copy the target vclib-render-example-${EXAMPLE_NAME} into
# ${CMAKE_CURRENT_SOURCE_DIR}/.. directory
add_custom_command(TARGET vclib-render-example-${EXAMPLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:vclib-render-example-${EXAMPLE_NAME}>
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

