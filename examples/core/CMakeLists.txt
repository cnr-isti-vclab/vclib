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
project(vclib-core-examples)

set(CMAKE_COMPILE_WARNING_AS_ERROR ${VCLIB_COMPILE_WARNINGS_AS_ERRORS})

add_subdirectory(000-mesh-basic)
add_subdirectory(001-polymesh-basic)
add_subdirectory(002-mesh-iterators)
add_subdirectory(003-mesh-copy)
add_subdirectory(004-mesh-normal)
add_subdirectory(005-mesh-topology)
add_subdirectory(006-mesh-stat)
add_subdirectory(007-mesh-conversion)
add_subdirectory(008-mesh-transform)
add_subdirectory(009-mesh-custom-components)
add_subdirectory(010-mesh-new-user-component)
add_subdirectory(011-mesh-export-to-matrix)
add_subdirectory(012-mesh-io-obj)
add_subdirectory(013-mesh-io-off)
add_subdirectory(014-mesh-io-ply)
add_subdirectory(015-mesh-io-stl)
add_subdirectory(016-mesh-clean)
add_subdirectory(017-mesh-create)
add_subdirectory(018-mesh-principal-curvature)
add_subdirectory(019-mesh-sampling)
add_subdirectory(020-mesh-smooth)
add_subdirectory(021-mesh-sphere-intersection)
add_subdirectory(022-edge-mesh-basic)
add_subdirectory(023-polygon-earcut)
add_subdirectory(024-distance)
add_subdirectory(025-grid)
add_subdirectory(026-crease-edges-mesh)
add_subdirectory(027-convex-hull)
add_subdirectory(028-topology-patch)
add_subdirectory(998-misc-no-lib)
add_subdirectory(999-misc)


