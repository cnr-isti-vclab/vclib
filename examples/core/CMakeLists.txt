#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
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
project(vclib-core-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

add_subdirectory(800-mesh-basic)
add_subdirectory(801-polymesh-basic)
add_subdirectory(802-mesh-iterators)
add_subdirectory(803-mesh-copy)
add_subdirectory(804-mesh-normal)
add_subdirectory(805-mesh-topology)
add_subdirectory(806-mesh-stat)
add_subdirectory(807-mesh-conversion)
add_subdirectory(808-mesh-transform)
add_subdirectory(809-mesh-custom-components)
add_subdirectory(810-mesh-new-user-component)
add_subdirectory(811-mesh-export-to-matrix)
add_subdirectory(812-mesh-io-obj)
add_subdirectory(813-mesh-io-off)
add_subdirectory(814-mesh-io-ply)
add_subdirectory(815-mesh-io-stl)
add_subdirectory(816-mesh-clean)
add_subdirectory(817-mesh-create)
add_subdirectory(818-mesh-principal-curvature)
add_subdirectory(819-mesh-sampling)
add_subdirectory(820-mesh-smooth)
add_subdirectory(821-mesh-sphere-intersection)
add_subdirectory(822-edge-mesh-basic)
add_subdirectory(823-polygon-earcut)
add_subdirectory(824-distance)
add_subdirectory(825-grid)
add_subdirectory(826-crease-edges-mesh)
add_subdirectory(827-convex-hull)
add_subdirectory(828-topology-patch)
add_subdirectory(998-misc-no-lib)
add_subdirectory(999-misc)


