#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

cmake_minimum_required(VERSION 3.13)
project(vclib-test)

# examples
if (VCLIB_BUILD_EXAMPLES)
    add_subdirectory(examples/mesh_io_stl)
    add_subdirectory(examples/mesh_principal_curvature)
    add_subdirectory(examples/mesh_sampling)
    add_subdirectory(examples/mesh_sphere_intersection)
    if (TARGET vclib-external-qglviewer)
        add_subdirectory(examples/ext/qglviewer/viewer)
    endif()
endif()

# tests
if (VCLIB_BUILD_AND_RUN_TESTS)

endif()

