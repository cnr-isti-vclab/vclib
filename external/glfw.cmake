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

find_package(glfw3 QUIET)

if (VCLIB_ALLOW_SYSTEM_GLFW)
    if(glfw3_FOUND)
        message(STATUS "- GLFW - using system-provided library")

        add_library(vclib-external-glfw INTERFACE)
        target_include_directories(vclib-external-glfw INTERFACE ${GLFW3_INCLUDE_DIRS})
        target_link_libraries(vclib-external-glfw INTERFACE ${GLFW3_LIBRARIES})
        target_compile_definitions(vclib-external-glfw INTERFACE ${GLFW3_DEFINITIONS})

        list(APPEND VCLIB_RENDER_EXTERNAL_LIBRARIES vclib-external-glfw)
    else()
        message(STATUS "- GLFW - not found, skipping")
    endif()
endif()
