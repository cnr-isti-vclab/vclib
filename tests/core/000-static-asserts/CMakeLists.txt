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

set(TEST_NAME 000-static-asserts)
project(vclib-test-${TEST_NAME})

set(HEADERS
    meshes.h
    loggers.h
    ranges.h
    space.h
    meshes/components/adjacent_edges.h
    meshes/components/adjacent_faces.h
    meshes/components/adjacent_vertices.h
    meshes/components/bounding_box.h
    meshes/components/bit_flags.h
    meshes/components/color.h
    meshes/components/component.h
    meshes/components/coordinate.h
    meshes/components/custom_components.h
    meshes/components/mark.h
    meshes/components/name.h
    meshes/components/normal.h
    meshes/components/quality.h
    meshes/components/tex_coord.h
    meshes/components/textures.h
    meshes/components/transform_matrix.h
    meshes/components/vertex_references.h
    meshes/components/wedge_colors.h
    meshes/components/wedge_tex_coords.h
    meshes/elements/edge.h
    meshes/elements/element.h
    meshes/elements/face.h
    meshes/components.h
    meshes/edgemesh.h
    meshes/elements.h
    meshes/mesh_views.h
    meshes/pointcloud.h
    meshes/polymesh.h
    meshes/trimesh.h
    space/array.h
    space/bit_set.h
    space/box.h
    space/color.h
    space/image.h
    space/matrix.h
    space/plane.h
    space/point.h
    space/polygon.h
    space/principal_curvature.h
    space/sampler.h
    space/segment.h
    space/sphere.h
    space/tex_coord.h
    space/texture.h
    space/triangle.h
)

set(SOURCES
    main.cpp
)

vclib_add_test(
    ${TEST_NAME}
    SOURCES ${HEADERS} ${SOURCES}
    ${HEADER_ONLY_OPTION})

