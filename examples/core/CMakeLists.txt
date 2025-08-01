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

add_subdirectory(000-mesh-io)
add_subdirectory(001-mesh-basics)
add_subdirectory(002-mesh-iteration)
add_subdirectory(003-mesh-creation)
add_subdirectory(004-mesh-copy-and-transform)
add_subdirectory(005-mesh-normals)
add_subdirectory(006-mesh-topology)
add_subdirectory(007-mesh-statistics)
add_subdirectory(008-polymesh-specifics)
add_subdirectory(009-mesh-custom-components)
add_subdirectory(010-mesh-matrix-conversion)
add_subdirectory(011-mesh-texture-and-texcoords)
add_subdirectory(012-mesh-topology-statistics)
add_subdirectory(013-mesh-clean)
add_subdirectory(014-mesh-smoothing)
add_subdirectory(015-mesh-curvature)
add_subdirectory(016-face-flooding)
add_subdirectory(017-mesh-convex-hull)
add_subdirectory(018-mesh-distance)
add_subdirectory(019-mesh-point-sampling)
add_subdirectory(020-mesh-crease-edges)
add_subdirectory(021-mesh-sphere-intersection)
add_subdirectory(022-mesh-grid)
add_subdirectory(023-mesh-custom-user-component)

add_subdirectory(998-misc-no-lib)
add_subdirectory(999-misc)


