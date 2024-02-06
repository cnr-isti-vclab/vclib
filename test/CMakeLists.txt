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

cmake_minimum_required(VERSION 3.24)
project(vclib-render-test)

option(VCLIB_RENDER_COMPILE_WARNINGS_AS_ERRORS "Compile warnings as errors" ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_RENDER_COMPILE_WARNINGS_AS_ERRORS})

include(cmake/cmake_functions.cmake)

# examples
if (VCLIB_BUILD_EXAMPLES)
    add_subdirectory(examples/mesh_io_stl)
    add_subdirectory(examples/mesh_principal_curvature)
    add_subdirectory(examples/mesh_sampling)
    add_subdirectory(examples/mesh_sphere_intersection)

    if (TARGET vclib-external-qglviewer)
        add_subdirectory(examples/qglviewer)
    endif()
endif()

# tests
if (VCLIB_BUILD_TESTS)

endif()

