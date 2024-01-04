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
#* it under the terms of the GNU General Public License as published by      *
#* the Free Software Foundation; either version 3 of the License, or         *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
#* for more details.                                                         *
#****************************************************************************/

set(VCLIB_PSTLD_DIR ${CMAKE_CURRENT_LIST_DIR}/pstld-master)

if (VCLIB_ALLOW_BUNDLED_PSTLD AND EXISTS ${VCLIB_PSTLD_DIR}/pstld/pstld.h)
    message(STATUS "- pstld - using bundled source")
else()
    message(
        FATAL_ERROR
        "pstld is required on Apple Clang- VCLIB_ALLOW_BUNDLED_PSTLD must be enabled and found.")
endif()

set(PSTLD_INCLUDE_DIRS ${VCLIB_PSTLD_DIR})

add_library(vclib-external-pstld SHARED ${VCLIB_PSTLD_DIR}/pstld/pstld.cpp)

target_include_directories(vclib-external-pstld PUBLIC ${PSTLD_INCLUDE_DIRS})
target_compile_definitions(vclib-external-pstld PUBLIC PSTLD_HACK_INTO_STD)

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-pstld)
