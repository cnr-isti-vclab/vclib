#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
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

set(VCLIB_BGFX_DIR ${CMAKE_CURRENT_LIST_DIR}/bgfx)

if (VCLIB_ALLOW_BUNDLED_BGFX AND EXISTS ${VCLIB_BGFX_DIR})
    message(STATUS "- bgfx - using bundled source")

    # leave the option to build bgfx examples, but set it to OFF by default
    option(BGFX_BUILD_EXAMPLES "Build bgfx examples" OFF)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    add_subdirectory(${VCLIB_BGFX_DIR})

    add_library(vclib-external-bgfx INTERFACE)

    # there are three warnings on gcc that we need to ignore
    set_property(TARGET bgfx PROPERTY COMPILE_WARNING_AS_ERROR OFF)

    target_link_libraries(vclib-external-bgfx INTERFACE bx bgfx bimg)

    set_target_properties(vclib-external-bgfx PROPERTIES BGFX_DIR ${VCLIB_BGFX_DIR})

    list(APPEND VCLIB_RENDER_EXTERNAL_LIBRARIES vclib-external-bgfx)
elseif(VCLIB_ALLOW_BUNDLED_BGFX)
    message(STATUS "- bgfx - not found, skipping")
endif()
