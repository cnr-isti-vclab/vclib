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

get_filename_component(TEST_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(vclib-test-${TEST_NAME})

set(HEADERS
    const_correctness.h
    loggers.h
    iterators.h
    mesh.h
    meshes.h
    ranges.h
    space.h
    mesh/components.h
    mesh/containers.h
    mesh/elements.h
    mesh/mesh_concept.h
    mesh/per_edge.h
    mesh/per_face.h
    mesh/per_mesh.h
    mesh/per_vertex.h
    mesh/components/adjacent_edges.h
    mesh/components/adjacent_faces.h
    mesh/components/adjacent_vertices.h
    mesh/components/bounding_box.h
    mesh/components/bit_flags.h
    mesh/components/color.h
    mesh/components/component.h
    mesh/components/custom_components.h
    mesh/components/mark.h
    mesh/components/name.h
    mesh/components/normal.h
    mesh/components/position.h
    mesh/components/quality.h
    mesh/components/tex_coord.h
    mesh/components/textures.h
    mesh/components/transform_matrix.h
    mesh/components/vertex_references.h
    mesh/components/wedge_colors.h
    mesh/components/wedge_tex_coords.h
    mesh/containers/edge_container.h
    mesh/containers/element_container.h
    mesh/containers/face_container.h
    mesh/containers/vertex_container.h
    mesh/elements/edge.h
    mesh/elements/element.h
    mesh/elements/face.h
    mesh/elements/vertex.h
    meshes/edgemesh.h
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

target_sources(vclib-core-test-000-static-asserts
  PRIVATE

)
