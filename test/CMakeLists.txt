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

cmake_minimum_required(VERSION 3.24)
project(vclib-test)

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

