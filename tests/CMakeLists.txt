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
project(vclib-tests)

option(VCLIB_ALLOW_DOWNLOAD_CATCH2
    "Allow use of downloaded Catch2 source" ON)
option(VCLIB_ALLOW_SYSTEM_CATCH2 "Allow use of system-provided Catch2" ON)

option(VCLIB_CORE_BUILD_TESTS
    "If true, the tests of VCLib-core will be built." ON)

option(VCLIB_EXTERNAL_BUILD_TESTS
    "If true, the tests of VCLib-external will be built." ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

### Catch2
include(catch2.cmake)

# examples
if (VCLIB_CORE_BUILD_TESTS)
    add_subdirectory(core)
endif()

if (VCLIB_BUILD_MODULE_EXTERNAL AND VCLIB_EXTERNAL_BUILD_TESTS)
    add_subdirectory(external)
endif()

# if (VCLIB_BUILD_MODULE_PROCESSING)
#     add_subdirectory(processing)
# endif()


