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
    add_subdirectory(kd_tree)
    add_subdirectory(load_save_mesh_obj)
    add_subdirectory(load_save_mesh_off)
    add_subdirectory(load_save_mesh_ply)
    add_subdirectory(load_save_mesh_stl)
    add_subdirectory(mesh_conversions)
    add_subdirectory(mesh_clean)
    add_subdirectory(mesh_custom_components)
    add_subdirectory(mesh_filter)
    add_subdirectory(mesh_new_user_component)
    add_subdirectory(mesh_topology)
    add_subdirectory(static_asserts)
    add_subdirectory(trimesh_base)

    if (TARGET vclib-external-vcg)
        add_subdirectory(ext/vcg/mesh_import_vcg)
    endif()
endif()

