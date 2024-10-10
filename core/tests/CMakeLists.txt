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

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

# tests
if (VCLIB_BUILD_TESTS)
    add_subdirectory(000-static-asserts)
    add_subdirectory(001-trimesh-base)
    add_subdirectory(002-mesh-topology)
    add_subdirectory(003-mesh-conversions)
    add_subdirectory(004-mesh-custom-components)
    add_subdirectory(005-mesh-new-user-component)
    add_subdirectory(006-load-save-mesh-obj)
    add_subdirectory(007-load-save-mesh-off)
    add_subdirectory(008-load-save-mesh-ply)
    add_subdirectory(009-load-save-mesh-stl)
    add_subdirectory(010-mesh-clean)
    add_subdirectory(011-mesh-filter)
    add_subdirectory(012-kd-tree)
    add_subdirectory(013-mesh-update-normal)
    add_subdirectory(014-polymesh-base)
    add_subdirectory(015-mesh-copy-and-append)
    add_subdirectory(016-mesh-crease-edges)
    add_subdirectory(017-serialization)
    add_subdirectory(018-polygon)


    if (TARGET vclib-external-vcg)
        add_subdirectory(900-mesh-import-vcg)
    endif()
endif()

