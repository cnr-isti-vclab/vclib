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
project(vclib-core)

### Options
option(VCLIB_BUILD_CORE_WITH_3RDPARTY_IO_LIBRARIES
    "Build the core module with 3rdparty I/O libraries" ON)

# 3rdparty libraries
message(STATUS "VCLib-Core: 3rdparty libraries")
add_subdirectory(3rdparty)

### vclib-core target
set(VCLIB_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/*.h")

add_library(${PROJECT_NAME} INTERFACE ${HEADERS})
add_library(vclib::core ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ${VCLIB_CORE_INCLUDE_DIR})
target_compile_definitions(${PROJECT_NAME} INTERFACE VCLIB_CORE_MODULE)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VCLIB_CORE_INCLUDE_DIR ${VCLIB_CORE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} INTERFACE ${VCLIB_CORE_3RDPARTY_LIBRARIES})

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS})

if (MSVC)
    target_compile_options(${PROJECT_NAME} INTERFACE "/bigobj")
endif()

# Install
install(DIRECTORY ${VCLIB_CORE_INCLUDE_DIR}/vclib
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
