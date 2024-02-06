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

set(VCLIB_NLOHMANN_JSON_DIR ${CMAKE_CURRENT_LIST_DIR}/nlohmann-json-3.11.2)

if (VCLIB_ALLOW_BUNDLED_NLOHMANN_JSON AND
        EXISTS ${VCLIB_NLOHMANN_JSON_DIR}/include/nlohmann/json.hpp)
    message(STATUS "- Nlohmann-json - using bundled source")
else()
    message(FATAL_ERROR "Nlohmann-json is required -"
        "VCLIB_ALLOW_BUNDLED_NLOHMANN_JSON must be enabled and found.")
endif()

set(NLOHMANN_JSON_INCLUDE_DIRS ${VCLIB_NLOHMANN_JSON_DIR}/include)

add_library(vclib-external-nlohmann-json INTERFACE)

target_include_directories(vclib-external-nlohmann-json
    INTERFACE ${NLOHMANN_JSON_INCLUDE_DIRS})

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-nlohmann-json)
