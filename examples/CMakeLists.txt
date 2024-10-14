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

cmake_minimum_required(VERSION 3.24)
project(vclib-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

option(VCLIB_CORE_TESTS_AND_EXAMPLES_HEADER_ONLY
    "If true, the examples and tests of VCLib-core will be built just setting
    the INCLUDE_PATH of the library (no cmake targets)." OFF)

if (VCLIB_CORE_TESTS_AND_EXAMPLES_HEADER_ONLY)
    set(HEADER_ONLY_OPTION HEADER_ONLY)
endif()

# examples
add_subdirectory(core)

# if (VCLIB_BUILD_PROCESSING_MODULE)
#     add_subdirectory(processing)
# endif()

if (VCLIB_BUILD_RENDER_MODULE)
    add_subdirectory(render)
endif()

